// Shell-integration events arrive as invisible OSC 777 sequences: ESC ] 777;cs;<event> BEL
export const OSC = /\x1b\]777;cs;([^\x07]*)\x07/g;

// The PTY may split an escape sequence across two messages. Returns [text safe to scan, tail to
// keep]: an unfinished "ESC ]..." (or a lone trailing ESC) waits for the next message.
export function takeComplete(pending: string, data: string): [string, string] {
  const text = pending + data;
  if (text.endsWith('\x1b')) return [text.slice(0, -1), '\x1b'];
  const start = text.lastIndexOf('\x1b]');
  if (start !== -1 && text.indexOf('\x07', start) === -1 && text.length - start < 256) return [text.slice(0, start), text.slice(start)];
  return [text, ''];
}
