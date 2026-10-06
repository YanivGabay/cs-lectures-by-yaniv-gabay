import { Container } from "@cloudflare/containers";

interface Env {
  TERMINAL: DurableObjectNamespace<TerminalContainer>;
  PRESENTER: DurableObjectNamespace<PresenterContainer>;
  CONNECT_LIMITER: { limit(opts: { key: string }): Promise<{ success: boolean }> };
  PRESENTER_KEY?: string;
}

const POOL_SIZE = 3;
const SESSIONS_PER_CONTAINER = 6;

// Student sandboxes: small instances, shared by up to 6 students each
export class TerminalContainer extends Container {
  defaultPort = 8080;
  sleepAfter = "5m";
  enableInternet = false;
}

// Yaniv's reserved sandbox for live lectures: faster instance, never shared with students
export class PresenterContainer extends Container {
  defaultPort = 8080;
  sleepAfter = "30m";
  enableInternet = false;
}

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

function isPresenter(url: URL, env: Env): boolean {
  const key = url.searchParams.get("presenter");
  return !!env.PRESENTER_KEY && !!key && key === env.PRESENTER_KEY;
}

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

const json = (request: Request, body: unknown, status = 200) => {
  const origin = request.headers.get("Origin");
  return new Response(JSON.stringify(body), {
    status,
    headers: {
      "Content-Type": "application/json",
      "Access-Control-Allow-Origin": allowedOrigin(origin) ? origin! : "https://cs-lectures.pages.dev",
      Vary: "Origin",
    },
  });
};

export default {
  async fetch(request: Request, env: Env): Promise<Response> {
    const url = new URL(request.url);
    const ip = request.headers.get("CF-Connecting-IP") || "unknown";

    if (url.pathname === "/health") return json(request, { status: "ok" });

    // Wake the presenter sandbox ahead of a lecture (no cold start in front of the class)
    if (url.pathname === "/warm") {
      if (!isPresenter(url, env)) return json(request, { error: "forbidden" }, 403);
      await start(env.PRESENTER.getByName("presenter"), "presenter");
      return json(request, { warm: true });
    }

    if (request.headers.get("Upgrade") !== "websocket") {
      return new Response("CS Lectures terminal", { status: 200 });
    }

    if (!allowedOrigin(request.headers.get("Origin"))) {
      return new Response("Forbidden origin", { status: 403 });
    }
    const { success } = await env.CONNECT_LIMITER.limit({ key: ip });
    if (!success) return new Response("Too many connections — wait a minute", { status: 429 });

    if (isPresenter(url, env)) {
      const stub = env.PRESENTER.getByName("presenter");
      await start(stub, "presenter");
      return stub.fetch(request);
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
