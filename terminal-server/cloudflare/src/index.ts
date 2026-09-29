import { Container } from "@cloudflare/containers";

interface Env {
  TERMINAL: DurableObjectNamespace;
  ALLOWED_ORIGINS: string;
}

export class TerminalContainer extends Container {
  defaultPort = 8080;
  sleepAfter = "10m";
  enableInternet = false;
}

export default {
  async fetch(request: Request, env: Env): Promise<Response> {
    const url = new URL(request.url);

    // CORS preflight
    if (request.method === "OPTIONS") {
      return new Response(null, { headers: corsHeaders(env) });
    }

    // Health check
    if (url.pathname === "/health") {
      return Response.json({ status: "ok" }, { headers: corsHeaders(env) });
    }

    // WebSocket upgrade — each connection gets its own container by session ID
    if (request.headers.get("Upgrade") === "websocket") {
      const sessionId = crypto.randomUUID();
      const container = env.TERMINAL.getByName(sessionId);
      await container.startAndWaitForPorts();
      return container.fetch(request);
    }

    return new Response("CS Lectures Terminal Server", { status: 200, headers: corsHeaders(env) });
  },
};

function corsHeaders(env: Env): HeadersInit {
  return {
    "Access-Control-Allow-Origin": "https://cs-lectures.pages.dev",
    "Access-Control-Allow-Methods": "GET, OPTIONS",
    "Access-Control-Allow-Headers": "Content-Type, Upgrade",
  };
}
