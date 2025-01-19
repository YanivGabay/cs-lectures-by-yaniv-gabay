## I. Introduction: What Is a Cooperating Process?
### Definition:
A cooperating process (or cooperating thread) is one that can affect or be affected by other processes in the system.
Such processes may share data by using a common address space (e.g., shared memory)
 or may communicate by sending messages through communication channels (like pipes, message queues, etc.).

בקיצור תהליך משתפ   
זה תהליך שיכול להשפיע או להישפע על תהליכים אחרים במערכת.
כמו כל התוכניות שרצות במערכת הפעלה שלנו. כל תהליך יכול להשפיע על תהליך אחר באמצעות שיתוף זיכרון או באמצעות תקשורת בין התהליכים.

All IPCS methods, Data sharing.


## II. Race Conditions - The problem with unsynchronized cooperating processes
### Definition
A situation where the outcome depends on the unpredictable timing
or interleaving of opeartions performed by multiple processes (or threads).

### Example
Consider the following code:
```c
int x = 0;
void increment() {
    x++;
}
void decrement() {
    x--;
}
```
If two processes run concurrently, one incrementing and the other decrementing `x`, the final value of `x` is unpredictable.

or non code example:

* Two processes updating a shared counter
* Process A reads the counter value (register)
* Process B reads the counter value (register)
* Process A increments the counter
* Process B increments the counter (it has the old value in his register)
* Process A writes the new value
* Process B writes the new value (it has the old value in his register)
* The final value of the counter is 1, not 2 as expected.