# Shared Memory

## Important Points

- Gives severa; processes the ability to read and write to the same memory location.
- They can read/write on every memory location given.
- When we talk about a memory segement its an array of bytes (can be struct etc)
- Its fast, cause there isnt a kernel buffer in between like several other IPC we have seen.

### Downsides
- No protection, so if one process writes to a memory location, the other process can read it.
- In the future you will need to use semaphores to protect the memory location.
- BUT currently, we will just try to syncronize the processes ourselves.