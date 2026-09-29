# Known Issues & Open Questions

## Open Questions

### Syscall tooltip — only first match per text node
The TreeWalker-based syscall wrapping in `CodeBlock.astro` processes only the first syscall name per text node. In practice this works because Shiki splits code into many small `<span>` elements, but a line like `wait(fork())` might miss the inner call. Low priority since it rarely occurs.

### Hardcoded localhost fallback in CodeBlock
`CodeBlock.astro` line ~302 shows `ws://localhost:8090` as a developer hint when the terminal URL isn't configured. This only appears if `PUBLIC_TERMINAL_URL` is empty (production builds always set it). Not a bug but could confuse contributors.

### DiffView uses positional line diff
The diff computation in `DiffView.astro` compares lines by position (line N vs line N). This works for the known mistake/fix pairs but would break for files with inserted/deleted lines. A proper LCS diff algorithm would be more robust if more pairs are added.

### Concept graph is hardcoded SVG
`ConceptGraph.astro` has node positions and edges hardcoded. Adding a new lesson requires manually updating the SVG layout. Consider generating from `os-lessons.ts` data in the future.

### ForkVisualizer is lesson 02 specific
`ForkVisualizer.astro` shows a generic fork lifecycle diagram, not dynamically derived from the actual C code. Adding visualizations for other lessons would require new components.

### Pre-captured outputs may go stale
The `captured-outputs.json` file needs to be re-run after C code changes. Run `python3 site/scripts/capture-outputs.py` to regenerate.

## Resolved

_(Issues fixed during audit are listed here for reference)_

- `.astro/dev.log` files were committed to git — removed from tracking, added `.astro/` to gitignore
