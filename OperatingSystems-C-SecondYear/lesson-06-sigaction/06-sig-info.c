/*
 * 06-sig-info.c — SA_SIGINFO — getting detailed signal information
 *
 * Key concepts: SA_SIGINFO, siginfo_t, si_pid, si_uid, si_signo
 * Compile: gcc -o siginfo 06-sig-info.c
 * Run:     ./prog
 */
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

// Enhanced handler for SIGUSR1 using SA_SIGINFO

// YOU DONT NEED TO USE SA_SIGINFO in the course/assigments
// it can be helpful in some cases, but its not mandatory!!
// (also the sets are not mandatory, but whatever)

//to use this program we need to send the signal with the kill command
// kill -SIGUSR1 <pid>

void handle_sigusr1(int signum, siginfo_t *info, void *context) {
    printf("\n  ── Signal Received ──────────────────\n");
    printf("  Signal:       %d (SIGUSR1)\n", signum);
    printf("  Sender PID:   %d\n", info->si_pid);
    printf("  Sender UID:   %d\n", info->si_uid);
    printf("  Signal code:  %d\n", info->si_code);
    printf("  Value (int):  %d\n", info->si_value.sival_int);
    printf("  Errno:        %d\n", info->si_errno);
    printf("  Address:      %p\n", info->si_addr);
    printf("  User time:    %ld\n", info->si_utime);
    printf("  System time:  %ld\n", info->si_stime);
    printf("  ────────────────────────────────────\n\n");
}

int main() {
    printf("\n");
    printf("══════════════════════════════════════\n");
    printf("  SA_SIGINFO — Detailed Signal Info\n");
    printf("  Access siginfo_t fields in handler\n");
    printf("══════════════════════════════════════\n\n");
    struct sigaction sa;

    // Setup handler for SIGUSR1 with SA_SIGINFO flag
    sa.sa_sigaction = handle_sigusr1;
    sa.sa_flags = SA_SIGINFO; // Enable extended signal information
    sigemptyset(&sa.sa_mask); // No additional signals blocked

    if (sigaction(SIGUSR1, &sa, NULL) == -1) {
        perror("Error: sigaction for SIGUSR1 failed");
        exit(EXIT_FAILURE);
    }

    printf("[Setup] Using SA_SIGINFO flag — handler gets siginfo_t struct\n");
    printf("[Setup] siginfo_t provides: sender PID, UID, signal code, etc.\n\n");
    printf("[Info]  Process PID: %d\n", getpid());
    printf("[Test]  Run: kill -SIGUSR1 %d\n\n", getpid());

    // Infinite loop to keep the program running
    int tick = 0;
    while (1) {
        printf("  [Tick %d] Waiting for SIGUSR1...\n", ++tick);
        sleep(3);
    }

    return 0;
}
