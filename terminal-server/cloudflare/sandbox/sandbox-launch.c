/*
 * sandbox-launch.c — How this website runs YOUR code safely
 *
 * Every terminal you open on the course site starts with this program.
 * The server runs as root and starts one copy per terminal session.
 * In a few system calls it locks itself into a sandbox and then
 * *becomes* your bash shell — using only tools from this course:
 *
 *   unshare() / setns()    — private System V IPC, /tmp, /dev/shm and network (Lessons 08-11)
 *   setrlimit()            — cap processes, CPU time, memory, file size, open files
 *   setsid()               — own session + process group (Lessons 03/06: Ctrl+C)
 *   setgid() / setuid()    — give up root, forever
 *   execve()               — replace this program with bash (Lesson 04: exec)
 *
 * Limits survive fork() and exec(), so every program you run inherits them.
 * That is why a fork bomb here only hurts the person who started it:
 * fork() starts failing with EAGAIN once YOUR user has 64 processes.
 *
 * Compile: gcc -Wall -Wextra -O2 -o sandbox-launch sandbox-launch.c
 * Usage:   sandbox-launch <uid> <gid> <workdir> <lesson|example> [join-pid]
 *          join-pid: a second terminal of the same session joins the first one's namespaces
 *
 * Try it yourself (Lesson 16 on the course site): compile and run it as a normal user, e.g.
 *     ./sandbox-launch 1234 1234 . example
 * setsid() fails with EPERM: bash made it a process-group leader, and a leader may not
 * start a new session (that is why daemons fork first). Run through `sh -c './sandbox-launch ...; exit $?'` it gets
 * further — the namespaces are quietly skipped, the limits only lowered — until setgroups()
 * fails with "Operation not permitted": only root may change who it is.
 * That failure IS the security model.
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <grp.h>
#include <fcntl.h>
#include <sched.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/mount.h>
#include <sys/resource.h>
#include <sys/socket.h>
#include <sys/stat.h>

#define MAX_PROCESSES   64                 /* RLIMIT_NPROC  -> fork() fails with EAGAIN      */
#define MAX_CPU_SECONDS 120                /* RLIMIT_CPU    -> SIGXCPU, then SIGKILL          */
#define MAX_MEMORY      (512UL << 20)      /* RLIMIT_AS     -> malloc() returns NULL          */
#define MAX_FILE_SIZE   (20UL << 20)       /* RLIMIT_FSIZE  -> SIGXFSZ when a file grows past */
#define MAX_OPEN_FILES  128                /* RLIMIT_NOFILE -> open()/pipe() fail with EMFILE */

static void die(const char *what)
{
    fprintf(stderr, "[sandbox] %s failed: %s\n", what, strerror(errno));
    exit(1);
}

/* soft limit == hard limit, so nobody inside the sandbox can raise it again */
static void set_limit(int resource, rlim_t value, const char *name)
{
    struct rlimit rl = { value, value };
    if (setrlimit(resource, &rl) == -1)
        die(name);
}

/* A fresh network namespace has only a loopback device, and it starts DOWN.
 * Same as `ip link set lo up`: read the interface flags, add IFF_UP, write them back. */
static void loopback_up(void)
{
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1)
        return;
    struct ifreq ifr;
    memset(&ifr, 0, sizeof ifr);
    strncpy(ifr.ifr_name, "lo", IFNAMSIZ - 1);
    if (ioctl(sock, SIOCGIFFLAGS, &ifr) == 0) {
        ifr.ifr_flags |= IFF_UP | IFF_RUNNING;
        ioctl(sock, SIOCSIFFLAGS, &ifr);
    }
    close(sock);
}

/* An empty, private tmpfs on top of a shared directory — visible only in our mount namespace */
static void private_tmpfs(const char *dir)
{
    mkdir(dir, 01777);
    mount("tmpfs", dir, "tmpfs", MS_NOSUID | MS_NODEV, "size=64m,mode=1777");
}

/* 0. Namespaces: the kernel gives this session its own copy of
 *      IPC  — System V message queues, shared memory, semaphores (Lessons 09/10):
 *             key 0x1234 here is not another student's key 0x1234
 *      NET  — its own loopback: your echo server on port 3879 does not collide with
 *             another student's, and there is no route to the internet
 *      MNT  — its own /tmp and /dev/shm (FIFOs, named semaphores), gone when you leave
 *    Each unshare() is tried on its own; without CAP_SYS_ADMIN they fail and the session
 *    simply shares them (the server cleans up after it instead). */
static void private_namespaces(void)
{
    if (unshare(CLONE_NEWIPC) == -1)
        return;
    if (unshare(CLONE_NEWNET) == 0)
        loopback_up();
    if (unshare(CLONE_NEWNS) == 0 &&
        mount(NULL, "/", NULL, MS_REC | MS_PRIVATE, NULL) == 0) { /* our mounts stay ours */
        private_tmpfs("/tmp");
        private_tmpfs("/dev/shm");
    }
}

/* A second terminal of the same session: enter the first terminal's namespaces with setns(),
 * so both programs of a pair (server + client, reader + writer) see the same IPC, ports and /tmp */
static void join_namespaces(const char *pid)
{
    const char *kinds[] = { "ipc", "net", "mnt" };
    for (int i = 0; i < 3; i++) {
        char path[64];
        snprintf(path, sizeof path, "/proc/%s/ns/%s", pid, kinds[i]);
        int fd = open(path, O_RDONLY | O_CLOEXEC);
        if (fd == -1)
            continue;
        setns(fd, 0);
        close(fd);
    }
}

int main(int argc, char *argv[])
{
    if (argc != 5 && argc != 6) {
        fprintf(stderr, "usage: %s <uid> <gid> <workdir> <lesson|example> [join-pid]\n", argv[0]);
        return 2;
    }

    uid_t uid = (uid_t)strtoul(argv[1], NULL, 10);
    gid_t gid = (gid_t)strtoul(argv[2], NULL, 10);
    const char *workdir = argv[3];
    const char *mode = strcmp(argv[4], "lesson") == 0 ? "lesson" : "example";

    if (uid == 0 || gid == 0) {
        fprintf(stderr, "[sandbox] refusing to start a session as root\n");
        return 2;
    }

    /* 0. Private namespaces (needs root — so it happens before we drop it) */
    if (argc == 6)
        join_namespaces(argv[5]);
    else
        private_namespaces();

    /* 1. Resource limits — set while we are still root; inherited by every child */
    set_limit(RLIMIT_NPROC, MAX_PROCESSES, "setrlimit(RLIMIT_NPROC)");
    set_limit(RLIMIT_CPU, MAX_CPU_SECONDS, "setrlimit(RLIMIT_CPU)");
    set_limit(RLIMIT_AS, MAX_MEMORY, "setrlimit(RLIMIT_AS)");
    set_limit(RLIMIT_FSIZE, MAX_FILE_SIZE, "setrlimit(RLIMIT_FSIZE)");
    set_limit(RLIMIT_NOFILE, MAX_OPEN_FILES, "setrlimit(RLIMIT_NOFILE)");
    set_limit(RLIMIT_CORE, 0, "setrlimit(RLIMIT_CORE)");

    /* 2. Own session + process group. The terminal layer (forkpty) usually made us a
     *    session leader already — then setsid() would fail with EPERM, so only call it
     *    if we are not one. The terminal sends Ctrl+C (SIGINT) to the FOREGROUND
     *    process group of this session — that is how Ctrl+C reaches your program. */
    if (getsid(0) != getpid() && setsid() == -1)
        die("setsid");

    /* 3. Into the student's private directory (mode 0700, owned by the session user) */
    if (chdir(workdir) == -1)
        die("chdir");

    /* 4. Drop root. Order matters: supplementary groups, then gid, then uid —
     *    after setuid() we no longer have permission to change our groups. */
    if (setgroups(0, NULL) == -1)
        die("setgroups");
    if (setgid(gid) == -1)
        die("setgid");
    if (setuid(uid) == -1)
        die("setuid");

    /* 5. Prove it: asking for root back must fail */
    if (setuid(0) == 0) {
        fprintf(stderr, "[sandbox] regained root after dropping it — refusing to continue\n");
        return 1;
    }

    /* 6. Become bash, with a clean environment (nothing from the server leaks in) */
    char home[512], cs_mode[32];
    snprintf(home, sizeof home, "HOME=%s", workdir);
    snprintf(cs_mode, sizeof cs_mode, "CS_MODE=%s", mode);
    char *envp[] = { "PATH=/usr/local/bin:/usr/bin:/bin", home, "USER=student",
                     "TERM=xterm-256color", "LANG=C.UTF-8", cs_mode, NULL };
    char *args[] = { "bash", "--rcfile", "/etc/cs-bashrc", "-i", NULL };

    execve("/bin/bash", args, envp);
    die("execve");   /* only reached if exec failed */
}
