// Course challenges: edit the starter code, press Run, and the site checks how the program
// ended — the same exit status the terminal chip shows (128 + N means killed by signal N).
// Every challenge is about a mechanism from the syllabus, and is safe inside the sandbox.

export interface ChallengeRule {
  exitCode?: number;   // required status of the run
  output?: string;     // regex the program's output must match
}

export interface Challenge {
  id: string;
  lesson: string;
  title: string;
  goal: string;
  starter: string;          // file in src/data/challenges/
  compileCmd: string;
  interruptAfterMs?: number;
  stopAfterMs?: number;
  stopSignal?: 'SIGINT' | 'SIGQUIT';
  pass: ChallengeRule;
  hints: { exitCode?: number; output?: string; text: string }[]; // first match wins
}

export const challenges: Challenge[] = [
  {
    id: 'fork-limit',
    lesson: '02',
    title: 'The fork() that fails',
    goal: 'The loop asks for 200 children but your user may own only 64 processes. Detect the failing fork(), print why with perror("fork") and stop the loop.',
    starter: 'fork-limit.c',
    compileCmd: 'gcc -Wall -o fork_limit fork-limit.c',
    pass: { exitCode: 0, output: 'fork: Resource temporarily unavailable' },
    hints: [
      { exitCode: 0, text: 'It finished, but never said why fork() failed. fork() returns -1 on failure — check for it and call perror("fork").' },
    ],
  },
  {
    id: 'survive-ctrl-c',
    lesson: '06',
    title: 'Survive Ctrl+C',
    goal: 'The site presses Ctrl+C after 3 seconds. Install a SIGINT handler with sigaction() so the program prints "Caught Ctrl+C" and keeps counting (the site then stops it with Ctrl+\\).',
    starter: 'survive-ctrl-c.c',
    compileCmd: 'gcc -Wall -o survive survive-ctrl-c.c',
    interruptAfterMs: 3000,
    stopAfterMs: 6000,
    stopSignal: 'SIGQUIT',
    pass: { exitCode: 131, output: 'Caught Ctrl\\+C' },
    hints: [
      { exitCode: 130, text: 'Killed by SIGINT — no handler was installed when Ctrl+C arrived. Fill in struct sigaction and call sigaction(SIGINT, &sa, NULL) before the loop.' },
      { exitCode: 131, text: 'It survived Ctrl+C — now make the handler print "Caught Ctrl+C" so we can see it ran.' },
    ],
  },
  {
    id: 'sigpipe',
    lesson: '08',
    title: 'Write into a pipe nobody reads',
    goal: 'Make the write() hit SIGPIPE: once no process holds the read end, writing kills the writer. The parent still holds one — close the end it does not use.',
    starter: 'sigpipe.c',
    compileCmd: 'gcc -Wall -o sigpipe sigpipe.c',
    pass: { exitCode: 141 },
    hints: [
      { exitCode: 0, text: 'The write succeeded, so a reader still exists: the parent itself. Close fd[0] in the parent before writing.' },
    ],
  },
  {
    id: 'deadlock',
    lesson: '14',
    title: 'Detect the deadlock',
    goal: 'Two philosophers lock two forks in opposite order and wait forever. Make philosopher B give up after 2 seconds with pthread_mutex_timedlock(), print "deadlock detected" and release its fork.',
    starter: 'deadlock.c',
    compileCmd: 'gcc -Wall -o deadlock deadlock.c -lpthread',
    stopAfterMs: 10000,
    pass: { exitCode: 0, output: 'deadlock detected' },
    hints: [
      { exitCode: 130, text: 'It hung until the site stopped it — that is the deadlock. Replace B\'s second pthread_mutex_lock() with pthread_mutex_timedlock() and a 2-second deadline.' },
      { exitCode: 0, text: 'It finished, but did not print "deadlock detected".' },
    ],
  },
];

export function judge(c: Challenge, exitCode: number, output: string) {
  const ok = (r: ChallengeRule) => (r.exitCode === undefined || r.exitCode === exitCode) && (!r.output || new RegExp(r.output).test(output));
  if (ok(c.pass)) return { passed: true, text: 'Challenge complete!' };
  const hint = c.hints.find((h) => ok(h));
  return { passed: false, text: hint?.text || `Not yet — the program ended with status ${exitCode}.` };
}
