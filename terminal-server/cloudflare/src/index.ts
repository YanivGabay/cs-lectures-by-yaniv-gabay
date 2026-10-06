import { Container } from "@cloudflare/containers";
import { DurableObject } from "cloudflare:workers";

interface Env {
  TERMINAL: DurableObjectNamespace<TerminalContainer>;
  PRESENTER: DurableObjectNamespace<PresenterContainer>;
  IP_COUNTER: DurableObjectNamespace<IpCounter>;
  PRESENTER_PASSWORD?: string; // chosen by the lecturer: wrangler secret put PRESENTER_PASSWORD
  TOKEN_SECRET?: string;       // random HMAC key that signs presenter passes
}

const POOL_SIZE = 3;
const SESSIONS_PER_CONTAINER = 6;
const PASS_DAYS = 90;
// Per client IP. A classroom shares one public IP: 10 students clicking Run a few times a
// minute stay far below this; a script opening connections in a loop does not.
const CONNECTS_PER_MINUTE = 150;
const LOGINS_PER_MINUTE = 5;

// One tiny Durable Object per client IP counts requests in a fixed one-minute window.
// A Durable Object handles one request at a time, so the count is exact — unlike the
// built-in rate limiter, which is approximate and per data centre.
export class IpCounter extends DurableObject {
  windows = new Map<string, { start: number; count: number }>();

  async hit(kind: string, limit: number, windowMs: number): Promise<boolean> {
    const now = Date.now();
    let w = this.windows.get(kind);
    if (!w || now - w.start >= windowMs) {
      w = { start: now, count: 0 };
      this.windows.set(kind, w);
    }
    w.count++;
    return w.count <= limit;
  }
}

const allow = (env: Env, ip: string, kind: string, limit: number) =>
  env.IP_COUNTER.getByName(ip).hit(kind, limit, 60_000);

// Shared by both kinds: report state for the status page WITHOUT waking a sleeping container
// or resetting its sleep timer (a status page left open must not keep containers running)
class SandboxContainer extends Container {
  defaultPort = 8080;
  enableInternet = false;

  async peek(): Promise<Record<string, unknown>> {
    const state = await this.getState();
    const awake = state.status === "running" || state.status === "healthy";
    if (!awake || !this.ctx.container?.running) return { state: "asleep" };
    try {
      const res = await this.ctx.container.getTcpPort(8080).fetch("http://container/health");
      return { state: "awake", ...((await res.json()) as object) };
    } catch {
      return { state: "starting" };
    }
  }
}

// Student sandboxes: small instances, shared by up to 6 students each
export class TerminalContainer extends SandboxContainer {
  sleepAfter = "5m";
}

// The lecturer's reserved sandbox for live lectures: faster instance, never shared with students
export class PresenterContainer extends SandboxContainer {
  sleepAfter = "30m";
}

// ---------- presenter passes: password -> signed token stored in the lecturer's browser ----------

const enc = new TextEncoder();
const b64url = (bytes: Uint8Array) =>
  btoa(String.fromCharCode(...bytes)).replace(/\+/g, "-").replace(/\//g, "_").replace(/=+$/, "");
const fromB64url = (s: string) =>
  Uint8Array.from(atob(s.replace(/-/g, "+").replace(/_/g, "/") + "=".repeat((4 - (s.length % 4)) % 4)), (c) => c.charCodeAt(0));

async function sha256(text: string) {
  return new Uint8Array(await crypto.subtle.digest("SHA-256", enc.encode(text)));
}

async function hmac(secret: string, data: string) {
  const key = await crypto.subtle.importKey("raw", enc.encode(secret), { name: "HMAC", hash: "SHA-256" }, false, ["sign"]);
  return new Uint8Array(await crypto.subtle.sign("HMAC", key, enc.encode(data)));
}

function sameBytes(a: Uint8Array, b: Uint8Array) {
  return a.length === b.length && crypto.subtle.timingSafeEqual(a, b);
}

// Changing the password changes this fingerprint, which invalidates every existing pass
async function passwordFingerprint(env: Env) {
  return b64url((await sha256(env.PRESENTER_PASSWORD!)).slice(0, 9));
}

async function issuePass(env: Env) {
  const expiresAt = Date.now() + PASS_DAYS * 24 * 3600 * 1000;
  const payload = b64url(enc.encode(JSON.stringify({ exp: expiresAt, pw: await passwordFingerprint(env) })));
  return { token: `${payload}.${b64url(await hmac(env.TOKEN_SECRET!, payload))}`, expiresAt };
}

async function validPass(env: Env, token: string | null) {
  if (!token || !env.PRESENTER_PASSWORD || !env.TOKEN_SECRET) return false;
  const [payload, sig] = token.split(".");
  if (!payload || !sig) return false;
  try {
    if (!sameBytes(fromB64url(sig), await hmac(env.TOKEN_SECRET, payload))) return false;
    const data = JSON.parse(new TextDecoder().decode(fromB64url(payload)));
    return data.exp > Date.now() && data.pw === (await passwordFingerprint(env));
  } catch {
    return false;
  }
}

// ---------- helpers ----------

function allowedOrigin(origin: string | null): boolean {
  if (!origin) return false;
  try {
    const { protocol, hostname } = new URL(origin);
    if (hostname === "localhost" || hostname === "127.0.0.1") return true;
    return protocol === "https:" && (hostname === "cs-lectures.pages.dev" || hostname.endsWith(".cs-lectures.pages.dev"));
  } catch {
    return false;
  }
}

function corsHeaders(request: Request): Record<string, string> {
  const origin = request.headers.get("Origin");
  return {
    "Access-Control-Allow-Origin": allowedOrigin(origin) ? origin! : "https://cs-lectures.pages.dev",
    "Access-Control-Allow-Methods": "GET, POST, OPTIONS",
    "Access-Control-Allow-Headers": "Content-Type",
    "Access-Control-Max-Age": "86400",
    Vary: "Origin",
  };
}

const json = (request: Request, body: unknown, status = 200) =>
  new Response(JSON.stringify(body), { status, headers: { "Content-Type": "application/json", ...corsHeaders(request) } });

async function start(stub: any, name: string) {
  await stub.startAndWaitForPorts({
    startOptions: { envVars: { CONTAINER_NAME: name, MAX_SESSIONS: String(SESSIONS_PER_CONTAINER) } },
  });
}

async function hasRoom(stub: any): Promise<boolean> {
  try {
    const res = await stub.fetch(new Request("http://container/health"));
    const h = (await res.json()) as { activeSessions: number; maxSessions: number };
    return h.activeSessions < h.maxSessions;
  } catch {
    return true; // if health is unreadable, let the container answer for itself
  }
}

// ---------- routes ----------

export default {
  async fetch(request: Request, env: Env): Promise<Response> {
    const url = new URL(request.url);
    const ip = request.headers.get("CF-Connecting-IP") || "unknown";

    if (request.method === "OPTIONS") return new Response(null, { status: 204, headers: corsHeaders(request) });
    if (url.pathname === "/health") return json(request, { status: "ok" });

    // Status page: every container's state, read without waking any of them
    if (url.pathname === "/status") {
      const names = [...Array.from({ length: POOL_SIZE }, (_, i) => `pool-${i}`), "presenter"];
      const containers = await Promise.all(names.map(async (name) => {
        const stub: any = name === "presenter" ? env.PRESENTER.getByName(name) : env.TERMINAL.getByName(name);
        try { return { name, ...(await stub.peek()) }; } catch (e) { return { name, state: "unknown", error: String(e) }; }
      }));
      return json(request, { containers, checkedAt: Date.now() });
    }

    // Lecturer signs in on any computer: password -> 90-day presenter pass for that browser
    if (url.pathname === "/presenter/login" && request.method === "POST") {
      if (!allowedOrigin(request.headers.get("Origin"))) return json(request, { error: "forbidden" }, 403);
      if (!env.PRESENTER_PASSWORD || !env.TOKEN_SECRET) return json(request, { error: "not configured" }, 503);
      if (!(await allow(env, ip, "login", LOGINS_PER_MINUTE))) return json(request, { error: "too many attempts" }, 429);
      let password = "";
      try { password = String(((await request.json()) as { password?: unknown }).password ?? ""); } catch {}
      if (!sameBytes(await sha256(password), await sha256(env.PRESENTER_PASSWORD))) {
        await new Promise((r) => setTimeout(r, 400)); // slow down guessing
        return json(request, { error: "wrong password" }, 401);
      }
      return json(request, await issuePass(env));
    }

    // Wake the presenter sandbox ahead of a lecture; also tells the site whether the pass is still valid
    if (url.pathname === "/warm") {
      if (!(await validPass(env, url.searchParams.get("presenter")))) return json(request, { error: "invalid pass" }, 403);
      await start(env.PRESENTER.getByName("presenter"), "presenter");
      return json(request, { warm: true });
    }

    if (request.headers.get("Upgrade") !== "websocket") {
      return new Response("CS Lectures terminal", { status: 200 });
    }

    if (!allowedOrigin(request.headers.get("Origin"))) {
      return new Response("Forbidden origin", { status: 403 });
    }
    // The lecturer's pass skips the limit: a live demo must never be refused
    if (await validPass(env, url.searchParams.get("presenter"))) {
      const stub = env.PRESENTER.getByName("presenter");
      await start(stub, "presenter");
      return stub.fetch(request);
    }

    if (!(await allow(env, ip, "connect", CONNECTS_PER_MINUTE))) {
      return new Response("Too many connections — wait a minute", { status: 429 });
    }

    // Start at a random student container; if it is full, try the others
    const first = Math.floor(Math.random() * POOL_SIZE);
    for (let i = 0; i < POOL_SIZE; i++) {
      const name = `pool-${(first + i) % POOL_SIZE}`;
      const stub = env.TERMINAL.getByName(name);
      await start(stub, name);
      if (i === POOL_SIZE - 1 || (await hasRoom(stub))) return stub.fetch(request);
    }
    return new Response("unreachable", { status: 500 });
  },
};
