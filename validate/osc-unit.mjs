// Unit check for the OSC splitter used by the site's terminal (site/src/lib/osc.ts):
// every way of cutting a stream into two messages must still yield every event exactly once.
//   node --experimental-strip-types osc-unit.mjs
import { check, assert, finish } from './lib.mjs';
const { OSC, takeComplete } = await import('../site/src/lib/osc.ts');

const stream = 'gcc -o p p.c\r\n\x1b]777;cs;exec\x07compiled\r\n\x1b]777;cs;prompt;0\x07student:~$ ./p\r\n\x1b]777;cs;prompt;139\x07';
const events = (chunks) => {
  let pending = '';
  const out = [];
  for (const c of chunks) {
    const [complete, rest] = takeComplete(pending, c);
    pending = rest;
    for (const m of complete.matchAll(OSC)) out.push(m[1]);
  }
  return out.join(',');
};

await check('U01', 'OSC events survive being split at every possible position', async () => {
  const want = events([stream]);
  assert(want === 'exec,prompt;0,prompt;139', `whole stream gave ${want}`);
  for (let i = 1; i < stream.length; i++) {
    const got = events([stream.slice(0, i), stream.slice(i)]);
    assert(got === want, `split at ${i} gave ${got}`);
  }
  for (let size = 1; size <= 7; size++) {
    const parts = stream.match(new RegExp(`[\\s\\S]{1,${size}}`, 'g'));
    assert(events(parts) === want, `${size}-byte chunks gave ${events(parts)}`);
  }
  return `${stream.length - 1} split points + 1..7-byte chunks`;
});

finish();
