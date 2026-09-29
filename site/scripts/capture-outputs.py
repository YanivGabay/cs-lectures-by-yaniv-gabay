#!/usr/bin/env python3
"""Compile and run non-interactive C programs, capture output as JSON."""

import json
import os
import shutil
import subprocess
import sys
import tempfile
import re

REPO_ROOT = os.path.abspath(os.path.join(os.path.dirname(__file__), '..', '..'))
OS_DIR = os.path.join(REPO_ROOT, 'OperatingSystems-C-SecondYear')
OUTPUT_FILE = os.path.join(REPO_ROOT, 'site', 'src', 'data', 'captured-outputs.json')
EXAMPLES_FILE = '/tmp/examples.json'

# Programs that need a peer process — can't run standalone
PAIRED_PATTERNS = [
    'consumer.c', 'producer.c', 'reader.c', 'writer.c',
    'fifo-reader.c', 'fifo-writer.c', 'multi-writer.c',
    'mw_reader.c', 'process_a.c', 'process_b.c',
    'sender.c', 'receiver.c', 'calc_sender.c', 'calc_receiver.c',
    'creator.c', 'updater.c', 'viewer.c',
    'echo-server.c', 'echo-client.c',
    'chat-server.c', 'chat-client.c',
    'server.c', 'client.c',
    'alarm_handler.c', 'alarm_manager.c',
    'parent-program.c', 'child.c',
    'position_creator.c', 'position_updater.c', 'position_viewer.c',
    'scoreboard_creator.c', 'scoreboard_updater.c', 'scoreboard_viewer.c',
    'arr_creator.c', 'arr_producer.c', 'arr_consumer.c',
]

# Long-running animations
LONGRUN_PATTERNS = ['01-cli.c', 'cli.c', '09-cool-canvas.c', '10-spinners.c']

def is_paired(filepath):
    basename = os.path.basename(filepath)
    for p in PAIRED_PATTERNS:
        if basename == p:
            return True
    # Also check subdirectory patterns
    for p in PAIRED_PATTERNS:
        if filepath.endswith(p):
            return True
    return False

def is_longrun(filepath):
    basename = os.path.basename(filepath)
    return basename in LONGRUN_PATTERNS

def main():
    with open(EXAMPLES_FILE) as f:
        examples = json.load(f)

    print(f"=== Capturing program outputs ===")
    print(f"Total examples: {len(examples)}")
    print()

    results = {}
    errors = []
    succeeded = 0
    skipped = 0
    failed = 0

    for ex in examples:
        folder = ex['folder']
        filename = ex['file']
        compile_cmd = ex['compileCmd']
        needs_input = ex.get('needsInput', False)
        key = f"{folder}/{filename}"

        # Skip interactive
        if needs_input:
            print(f"  SKIP (interactive): {key}")
            skipped += 1
            continue

        # Skip paired
        if is_paired(filename):
            print(f"  SKIP (needs peer):  {key}")
            skipped += 1
            continue

        # Skip long-running
        if is_longrun(filename):
            print(f"  SKIP (long-run):    {key}")
            skipped += 1
            continue

        lesson_dir = os.path.join(OS_DIR, folder)
        src_file = os.path.join(lesson_dir, filename)

        if not os.path.isfile(src_file):
            print(f"  SKIP (not found):   {key}")
            skipped += 1
            continue

        # Create temp workdir, copy all .c and .h files from lesson
        workdir = tempfile.mkdtemp(prefix='cs-capture-')
        try:
            for root, dirs, files in os.walk(lesson_dir):
                for f in files:
                    if f.endswith('.c') or f.endswith('.h'):
                        src = os.path.join(root, f)
                        rel = os.path.relpath(src, lesson_dir)
                        dst = os.path.join(workdir, rel)
                        os.makedirs(os.path.dirname(dst), exist_ok=True)
                        shutil.copy2(src, dst)

            # Extract output binary name
            m = re.search(r'-o\s+(\S+)', compile_cmd)
            output_bin = m.group(1) if m else 'a.out'

            # Compile
            try:
                subprocess.run(
                    compile_cmd, shell=True, cwd=workdir,
                    capture_output=True, text=True, timeout=10,
                    check=True
                )
            except (subprocess.CalledProcessError, subprocess.TimeoutExpired) as e:
                stderr = getattr(e, 'stderr', str(e))
                print(f"  FAIL (compile):     {key}")
                errors.append(f"{key}: {stderr[:100]}")
                failed += 1
                continue

            # Run
            try:
                result = subprocess.run(
                    f'./{output_bin}', shell=True, cwd=workdir,
                    capture_output=True, text=True, timeout=5
                )
                output = result.stdout
                if result.stderr:
                    output += result.stderr
            except subprocess.TimeoutExpired:
                output = "[Program timed out after 5 seconds]"

            # Clean up IPC
            subprocess.run('ipcrm -a', shell=True, capture_output=True)

            if output and output.strip():
                results[key] = output.rstrip()
                lines = len(output.strip().split('\n'))
                print(f"  OK ({lines:3d} lines):    {key}")
                succeeded += 1
            else:
                print(f"  SKIP (no output):   {key}")
                skipped += 1

        finally:
            shutil.rmtree(workdir, ignore_errors=True)

    # Add errors
    if errors:
        results['_errors'] = '\n'.join(errors)

    # Write JSON
    with open(OUTPUT_FILE, 'w') as f:
        json.dump(results, f, indent=2, ensure_ascii=False)

    print()
    print(f"=== Done ===")
    print(f"Succeeded: {succeeded}")
    print(f"Skipped:   {skipped}")
    print(f"Failed:    {failed}")
    print(f"Output:    {OUTPUT_FILE}")

if __name__ == '__main__':
    main()
