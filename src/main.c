#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int* global_childs;
int global_childs_count;

void sigterm_handler(int signum)
{
    for (int i = 0; i < global_childs_count; i++) {
        if (global_childs[i] > 0) {
            kill(global_childs[i], SIGTERM);
        }
    }
    free(global_childs);
    global_childs = NULL;
    global_childs_count = 0;
    exit(1);
}

void container(const char* process, int child_process_number)
{
    pid_t pid;
    int status;

    pid = fork();

    if (pid == -1) {
        perror("fork failed");
        return;
    }

    if (pid == 0) {
        execlp(process, process);
        perror("execlp failed");
        exit(1);
    } else {
        global_childs[child_process_number] = pid;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return;
        }

        printf("%s process exited, starting again\n", process);
        container(process, child_process_number);
    }
}

int main(int argc, const char** argv)
{
    if (argc >= 2) {
        int pid;
        int status;

        global_childs_count = argc - 1;
        global_childs = (int*)malloc(global_childs_count * sizeof(int));

        if (signal(SIGTERM, sigterm_handler) == SIG_ERR) {
            perror("signal registration failed for SIGTERM");
            return 1;
        }
        if (signal(SIGINT, sigterm_handler) == SIG_ERR) {
            perror("signal registration failed for SIGINT");
            return 1;
        }

        for (int i = 1; i < argc; i++) {
            pid = fork();
            if (pid == -1) {
                perror("failed to fork");
            } else if (pid == 0) {
                container(argv[i], i - 1);
            }
        }
        // wait for the last child process to exit
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return -1;
        }

        free(global_childs);
        global_childs = NULL;
        global_childs_count = 0;
    } else {
        printf("Usage: %s program1 program2 program3\n", argv[0]);
    }
}
