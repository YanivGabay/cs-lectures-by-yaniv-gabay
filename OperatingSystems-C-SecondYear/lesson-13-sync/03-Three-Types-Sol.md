## Introducting Three Types of Synchronization Solutions

### A. Software-Level Protocols (Low level synchronization)
#### Description
* Rules or algorithms (busy waiting, Test and Set etc )
* Analogy - Turn based systems, each process takes turns to access the shared resource.

### B. Hardware-Level Protocols (Low level synchronization)
#### Description
* Will help us achieve the wanted results.

### C. Operating System-Level Protocols (High level synchronization)
#### Description
* Will help us achieve the wanted results.
* Will be supported by the compiler and the OS.
* Will be usually much simpler to use than the other two types of synchronization solutions.
* Semaphores, Mutexes, Monitors, Collections (java) - already built in thread safe data structures.