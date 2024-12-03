/*

this is a pure text file, no code here
we are talking about pipes

Introduction to Pipes


What is a Pipe?

Definition: A pipe is a unidirectional communication channel that allows data to flow from one process to another.

Purpose: Facilitates IPC by enabling the output of one process to be used as the input of another.

Types:
Anonymous Pipes: Typically used between parent and child processes. (the pipe() system call)
Named Pipes (FIFOs): Persist in the filesystem and can be used between unrelated processes. (mkfifo() system call)

Pipe Characteristics

Unidirectional: Data flows in one direction. (meaning the other way around is closed while the data is flowing)

Byte Streams: Data is treated as a continuous stream of bytes

Limited Buffer Size: POSIX defines a minimum buffer size (PIPE_BUF) - anything less and equal to PIPE_BUF happens atomically (guaranteed to be written/read in one go)

The Pipe | Operator

Purpose: Connects the standard output (stdout) of one command to the standard input (stdin) of another.

Example:
$ ls -l | grep "file"

or:
cat file.txt | sort | uniq
Displays unique sorted lines from file.txt.



*/