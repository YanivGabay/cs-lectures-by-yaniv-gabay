#!/usr/bin/env bash
# Build and run the terminal container locally for security probes.
# Hard caps keep destructive probes (fork bombs, kill -1) inside the container:
# an uncapped fork bomb once froze the whole WSL VM.
# SYS_ADMIN/NET_ADMIN/SYS_PTRACE mirror production (Cloudflare gives the container root all capabilities),
# so the per-session namespaces in sandbox-launch.c are exercised locally too.
#   bash validate/local-container.sh        # build + start on ws://localhost:8099
#   bash validate/local-container.sh stop
set -euo pipefail
NAME=cs-terminal-test
cd "$(dirname "$0")/../terminal-server/cloudflare"

if [[ "${1:-}" == "stop" ]]; then
  docker rm -f "$NAME" >/dev/null 2>&1 || true
  exit 0
fi

docker build -q -t "$NAME" . >/dev/null
docker rm -f "$NAME" >/dev/null 2>&1 || true
docker run -d --rm --name "$NAME" \
  --pids-limit 512 --memory 1g --memory-swap 1g --cpus 1 \
  --cap-add SYS_ADMIN --cap-add NET_ADMIN --cap-add SYS_PTRACE \
  -p 8099:8080 "$NAME" >/dev/null

for _ in $(seq 1 30); do
  curl -sf localhost:8099/health >/dev/null && { echo "ready: ws://localhost:8099 (capped: 512 pids, 1 GiB, 1 CPU)"; exit 0; }
  sleep 1
done
echo "container did not become healthy" >&2
docker logs "$NAME" >&2
exit 1
