#!/bin/bash
# Compile and run non-interactive C programs, capture output as JSON.
# Usage: bash scripts/capture-outputs.sh
#
# Requires: gcc, jq, timeout

set -euo pipefail

REPO_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
OS_DIR="$REPO_ROOT/OperatingSystems-C-SecondYear"
OUTPUT_FILE="$(cd "$(dirname "$0")/.." && pwd)/src/data/captured-outputs.json"
EXAMPLES_JSON="/tmp/examples.json"

# Programs that need a peer process (client/server, reader/writer)
# These can't run standalone — skip them
PAIRED_PROGRAMS=(
  "02-pipe-operator-with-programs/consumer.c"
  "08-two-ways-with-dupes/child.c"
  "09-mkfifo/reader.c"
  "09-mkfifo/writer.c"
  "02-mkfifo-example/fifo-reader.c"
  "02-mkfifo-example/fifo-writer.c"
  "03-mk-fifo-multi-writers/multi-writer.c"
  "03-mk-fifo-multi-writers/reader.c"
  "04-mk-fifo-two-way/process_a.c"
  "04-mk-fifo-two-way/process_b.c"
  "07-msg-que-two-progs/sender.c"
  "07-msg-que-two-progs/receiver.c"
  "08-calc-que/calc_sender.c"
  "08-calc-que/calc_receiver.c"
  "02-basic-example/creator.c"
  "02-basic-example/consumer.c"
  "03-positions/position_creator.c"
  "03-positions/position_updater.c"
  "03-positions/position_viewer.c"
  "04-array/creator.c"
  "04-array/producer.c"
  "04-array/consumer.c"
  "05-scoreboard-struct/scoreboard_creator.c"
  "05-scoreboard-struct/scoreboard_updater.c"
  "05-scoreboard-struct/scoreboard_viewer.c"
  "01-example/echo-server.c"
  "01-example/echo-client.c"
  "02-chat/chat-server.c"
  "02-chat/chat-client.c"
  "03-arithmetic/server.c"
  "03-arithmetic/client.c"
  "alarm_manager/alarm_handler.c"
  "08-two-ways-with-dupes/parent-program.c"
)

# Programs with long-running loops (animations, spinners, canvases)
LONGRUN_PROGRAMS=(
  "01-cli.c"
  "02-bit-more-cli/cli.c"
  "09-cool-canvas.c"
  "10-spinners.c"
)

is_paired() {
  local file="$1"
  for p in "${PAIRED_PROGRAMS[@]}"; do
    if [[ "$file" == *"$p" ]]; then
      return 0
    fi
  done
  return 1
}

is_longrun() {
  local file="$1"
  for p in "${LONGRUN_PROGRAMS[@]}"; do
    if [[ "$file" == *"$p" ]]; then
      return 0
    fi
  done
  return 1
}

echo "=== Capturing program outputs ==="
echo "OS dir: $OS_DIR"
echo "Output: $OUTPUT_FILE"
echo ""

# Start building JSON
echo "{" > "$OUTPUT_FILE.tmp"
FIRST=true
SUCCEEDED=0
SKIPPED=0
FAILED=0
ERRORS=""

# Read examples from the JSON file
EXAMPLE_COUNT=$(jq length "$EXAMPLES_JSON")

for i in $(seq 0 $((EXAMPLE_COUNT - 1))); do
  FOLDER=$(jq -r ".[$i].folder" "$EXAMPLES_JSON")
  FILE=$(jq -r ".[$i].file" "$EXAMPLES_JSON")
  COMPILE=$(jq -r ".[$i].compileCmd" "$EXAMPLES_JSON")
  NEEDS_INPUT=$(jq -r ".[$i].needsInput" "$EXAMPLES_JSON")
  KEY="$FOLDER/$FILE"

  # Skip interactive programs
  if [[ "$NEEDS_INPUT" == "true" ]]; then
    echo "SKIP (interactive): $KEY"
    SKIPPED=$((SKIPPED + 1))
    continue
  fi

  # Skip paired programs
  if is_paired "$FILE"; then
    echo "SKIP (needs peer): $KEY"
    SKIPPED=$((SKIPPED + 1))
    continue
  fi

  # Skip long-running programs
  if is_longrun "$FILE"; then
    echo "SKIP (long-running): $KEY"
    SKIPPED=$((SKIPPED + 1))
    continue
  fi

  LESSON_DIR="$OS_DIR/$FOLDER"
  if [[ ! -d "$LESSON_DIR" ]]; then
    echo "SKIP (no dir): $KEY"
    SKIPPED=$((SKIPPED + 1))
    continue
  fi

  SRC_FILE="$LESSON_DIR/$FILE"
  if [[ ! -f "$SRC_FILE" ]]; then
    echo "SKIP (no file): $KEY"
    SKIPPED=$((SKIPPED + 1))
    continue
  fi

  # Work in a temp directory to avoid polluting the source tree
  WORKDIR=$(mktemp -d)
  # Copy all .c and .h files from the lesson dir (some examples include local headers)
  find "$LESSON_DIR" -name '*.c' -o -name '*.h' | while read -r f; do
    REL=$(realpath --relative-to="$LESSON_DIR" "$f")
    mkdir -p "$WORKDIR/$(dirname "$REL")"
    cp "$f" "$WORKDIR/$REL"
  done

  cd "$WORKDIR"

  # Extract output binary name from compile command
  OUTPUT_BIN=$(echo "$COMPILE" | grep -oP '(?<=-o\s)\S+' || echo "a.out")

  # Compile
  COMPILE_OUT=$(eval "$COMPILE" 2>&1) || {
    echo "FAIL (compile): $KEY"
    ERRORS="$ERRORS\n  $KEY: compile error"
    FAILED=$((FAILED + 1))
    rm -rf "$WORKDIR"
    cd "$REPO_ROOT/site"
    continue
  }

  # Run with timeout
  RUN_OUTPUT=$(timeout 5 "./$OUTPUT_BIN" 2>&1) || true

  # Clean up IPC resources that might have been created
  ipcrm -a 2>/dev/null || true

  # Only save if we got output
  if [[ -n "$RUN_OUTPUT" ]]; then
    # Escape for JSON
    ESCAPED=$(echo "$RUN_OUTPUT" | python3 -c 'import sys,json; print(json.dumps(sys.stdin.read()))')

    if [[ "$FIRST" == "true" ]]; then
      FIRST=false
    else
      echo "," >> "$OUTPUT_FILE.tmp"
    fi
    echo "  \"$KEY\": $ESCAPED" >> "$OUTPUT_FILE.tmp"
    echo "  OK: $KEY ($(echo "$RUN_OUTPUT" | wc -l) lines)"
    SUCCEEDED=$((SUCCEEDED + 1))
  else
    echo "SKIP (no output): $KEY"
    SKIPPED=$((SKIPPED + 1))
  fi

  rm -rf "$WORKDIR"
  cd "$REPO_ROOT/site"
done

# Add errors key if any
if [[ -n "$ERRORS" ]]; then
  if [[ "$FIRST" != "true" ]]; then
    echo "," >> "$OUTPUT_FILE.tmp"
  fi
  echo "  \"_errors\": \"$(echo -e "$ERRORS" | sed 's/"/\\"/g')\"" >> "$OUTPUT_FILE.tmp"
fi

echo "" >> "$OUTPUT_FILE.tmp"
echo "}" >> "$OUTPUT_FILE.tmp"

mv "$OUTPUT_FILE.tmp" "$OUTPUT_FILE"

echo ""
echo "=== Done ==="
echo "Succeeded: $SUCCEEDED"
echo "Skipped:   $SKIPPED"
echo "Failed:    $FAILED"
echo "Output:    $OUTPUT_FILE"
