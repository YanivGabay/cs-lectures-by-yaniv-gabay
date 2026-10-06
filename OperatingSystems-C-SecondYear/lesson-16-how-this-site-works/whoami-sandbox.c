/*
 * whoami-sandbox.c — Look around: what kind of process is running YOUR code?
 *
 * Demonstrates: everything sandbox-launch.c set up before it became your bash shell
 * Key concepts: getuid(), getrlimit(), getsid()/getpgrp()/tcgetpgrp(), /proc/self
 * Compile: gcc -Wall -o whoami-sandbox whoami-sandbox.c
 * Run:     ./whoami-sandbox
 *
 * Every value printed here was inherited through fork() + exec() from the launcher:
 * limits, user, session and namespaces all survive exec — Lesson 04.
 */

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/resource.h>

static void show_limit(const char *name, int resource, const char *what_happens)
{
    struct rlimit rl;
    getrlimit(resource, &rl);
    if (rl.rlim_cur == RLIM_INFINITY)
        printf("  %-14s unlimited\n", name);
    else
        printf("  %-14s %-10llu -> %s\n", name, (unsigned long long)rl.rlim_cur, what_happens);
}

/* /proc/self/status has one "Name:\tvalue" line per fact about this process */
static void show_status_line(const char *key)
{
    FILE *f = fopen("/proc/self/status", "r");
    char line[256];
    while (f && fgets(line, sizeof line, f))
        if (strncmp(line, key, strlen(key)) == 0)
            printf("  %s", line);
    if (f)
        fclose(f);
}

/* Each namespace has an id; two processes with the same id share that namespace */
static void show_namespace(const char *kind)
{
    char path[64], target[64] = "(hidden)";
    snprintf(path, sizeof path, "/proc/self/ns/%s", kind);
    ssize_t n = readlink(path, target, sizeof target - 1);
    if (n > 0)
        target[n] = '\0';
    printf("  %-4s %s\n", kind, target);
}

int main(void)
{
    printf("\n══════════════════════════════════════\n");
    printf("  Who am I inside the course sandbox?\n");
    printf("══════════════════════════════════════\n\n");

    printf("[Identity]  uid=%d gid=%d — not 0, so not root (setuid() in the launcher)\n",
           (int)getuid(), (int)getgid());
    show_status_line("CapEff");
    printf("            all zeros = no special powers left\n\n");

    printf("[Process group / session — Lessons 03 and 06]\n");
    printf("  my pid=%d  process group=%d  session=%d\n", (int)getpid(), (int)getpgrp(), (int)getsid(0));
    printf("  foreground group of this terminal=%d", (int)tcgetpgrp(STDIN_FILENO));
    printf("%s\n\n", tcgetpgrp(STDIN_FILENO) == getpgrp()
               ? "  <- that's me: Ctrl+C will be sent to MY group"
               : "");

    printf("[Limits — set with setrlimit() before the launcher dropped root]\n");
    show_limit("processes", RLIMIT_NPROC, "fork() fails with EAGAIN (Lesson 02)");
    show_limit("CPU seconds", RLIMIT_CPU, "SIGXCPU, then SIGKILL");
    show_limit("memory bytes", RLIMIT_AS, "malloc() returns NULL");
    show_limit("file blocks", RLIMIT_FSIZE, "SIGXFSZ when a file grows too big");
    show_limit("open files", RLIMIT_NOFILE, "open()/pipe() fail with EMFILE (Lesson 08)");
    printf("\n");

    printf("[Namespaces — this session's private copies]\n");
    show_namespace("ipc");
    show_namespace("net");
    show_namespace("mnt");
    printf("  Open a second terminal (Two Terminals) and run this again:\n");
    printf("  same ids there, different ids for every other student.\n");
    return 0;
}
