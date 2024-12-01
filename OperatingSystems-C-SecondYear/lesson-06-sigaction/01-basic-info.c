

//content of the sigaction struct
struct sigaction {
    void     (*sa_handler)(int);    // Pointer to the signal handling function
    void     (*sa_sigaction)(int, siginfo_t *, void *); // pointer to a function that takes three arguments, the signal number, a pointer to a siginfo_t, and a pointer to a void
    sigset_t sa_mask;               // Signals to be blocked during handler execution , the sigset_t is a specific type of masks.
    int      sa_flags;              // Special flags to modify behavior
    void     (*sa_restorer)(void);  // Obsolete, should be set to NULL (used to save the previous signal behavior)
};

struct sigaction sa;


/// 95% you wont need to know all that info that im showing, but im still showing it
// why? knowledge is power, and you never know when you will need it
// and cus i can

//sa_flags:

/* //// taken from
https://man7.org/linux/man-pages/man2/sigaction.2.html
      SA_NOCLDSTOP (since Linux 2.6.3)
              If signum is SIGCHLD, do not receive notification when
              child processes stop (i.e., when they receive one of
              SIGSTOP, SIGTSTP, SIGTTIN, or SIGTTOU) or resume (i.e.,
              they receive SIGCONT) (see wait(2)).  This flag is
              meaningful only when establishing a handler for SIGCHLD.

       SA_NOCLDWAIT (since Linux 2.6)
              If signum is SIGCHLD, do not transform children into
              zombies when they terminate.  See also waitpid(2).  This
              flag is meaningful only when establishing a handler for
              SIGCHLD, or when setting that signal's disposition to
              SIG_DFL.

              If the SA_NOCLDWAIT flag is set when establishing a
              handler for SIGCHLD, POSIX.1 leaves it unspecified whether
              a SIGCHLD signal is generated when a child process
              terminates.  On Linux, a SIGCHLD signal is generated in
              this case; on some other implementations, it is not.

       SA_NODEFER
              Do not add the signal to the thread's signal mask while
              the handler is executing, unless the signal is specified
              in act.sa_mask.  Consequently, a further instance of the
              signal may be delivered to the thread while it is
              executing the handler.  This flag is meaningful only when
              establishing a signal handler.

              SA_NOMASK is an obsolete, nonstandard synonym for this
              flag.

       SA_ONSTACK
              Call the signal handler on an alternate signal stack
              provided by sigaltstack(2).  If an alternate stack is not
              available, the default stack will be used.  This flag is
              meaningful only when establishing a signal handler.

       SA_RESETHAND
              Restore the signal action to the default upon entry to the
              signal handler.  This flag is meaningful only when
              establishing a signal handler.

              SA_ONESHOT is an obsolete, nonstandard synonym for this
              flag.

       SA_RESTART
              Provide behavior compatible with BSD signal semantics by
              making certain system calls restartable across signals.
              This flag is meaningful only when establishing a signal
              handler.  See signal(7) for a discussion of system call
              restarting.

       SA_RESTORER
              Not intended for application use.  This flag is used by C
              libraries to indicate that the sa_restorer field contains
              the address of a "signal trampoline".  See sigreturn(2)
              for more details.

       SA_SIGINFO (since Linux 2.2)
              The signal handler takes three arguments, not one.  In
              this case, sa_sigaction should be set instead of
              sa_handler.  This flag is meaningful only when
              establishing a signal handler.

       SA_UNSUPPORTED (since Linux 5.11)
              Used to dynamically probe for flag bit support.

              If an attempt to register a handler succeeds with this
              flag set in act->sa_flags alongside other flags that are
              potentially unsupported by the kernel, and an immediately
              subsequent sigaction() call specifying the same signal
              number and with a non-NULL oldact argument yields
              SA_UNSUPPORTED clear in oldact->sa_flags, then
              oldact->sa_flags may be used as a bitmask describing which
              of the potentially unsupported flags are, in fact,
              supported.  See the section "Dynamically probing for flag
              bit support" below for more details.

       SA_EXPOSE_TAGBITS (since Linux 5.11)
              Normally, when delivering a signal, an architecture-
              specific set of tag bits are cleared from the si_addr
              field of siginfo_t.  If this flag is set, an architecture-
              specific subset of the tag bits will be preserved in
              si_addr.

              Programs that need to be compatible with Linux versions
              older than 5.11 must use SA_UNSUPPORTED to probe for
              support.





*/

/*
IMPORTANT
A child created via fork(2) inherits a copy of its parent's
       signal dispositions.  During an execve(2), the dispositions of
       handled signals are reset to the default; the dispositions of
       ignored signals are left unchanged.
*/
