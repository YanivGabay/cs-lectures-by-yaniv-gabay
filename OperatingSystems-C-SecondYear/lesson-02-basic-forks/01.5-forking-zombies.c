#include <stdio.h>

/*
In Unix-like operating systems (including Linux), the init process (with PID 1) plays a crucial role in process management. When a child process is left "orphaned" (because its parent process has terminated without waiting for it), the init process automatically adopts it. Here’s how this relates to zombies and waiting on child processes:

Understanding Zombies and Orphan Processes
Zombie Process:

A zombie process is a child process that has completed execution but remains in the process table because its parent hasn’t called wait() to read its exit status.
This process remains as a "zombie" until the parent either reaps it by calling wait() or the parent itself terminates, allowing the operating system to handle it.
Orphaned Process and init Adoption:

When a parent process terminates without waiting on its child processes, those children become "orphans."
The operating system automatically assigns these orphan processes to the init process (PID 1).
The init process is designed to continuously wait on all its children, so when it adopts an orphan process, it will eventually reap it, cleaning up any zombie state and freeing resources.
Automatic Cleanup by init:

This behavior allows zombie processes to be cleared even if the original parent process doesn’t explicitly call wait().
In effect, simply terminating the parent process will result in any zombie child processes being adopted and cleaned up by init.


Example: Creating and Observing Orphan and Zombie Processes
You can create zombie processes and let them be automatically cleaned up by init by ending the parent process without waiting on its children. Here’s an example:
*/


/// AT THIS POINT USE THE FOLLOWING:
// Use ps aux | grep Z
// to see the zombie process
