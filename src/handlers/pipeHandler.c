#include "../../headers/handlers/pipeHandler.h"

void ipipe(struct cmd *cmd_1, struct cmd *cmd_2, struct cmd *cmd_3, int count, int redirect, char *endPath) {
    int fd_1[2];

    pid_t pid_1, pid_2, pid_3;
    if (pipe(fd_1) < 0) {
        perror("pipe");
        return;
    }

    pid_1 = fork();
    if (pid_1 < 0) {
        perror("fork 1");
        return;
    }
    if ((pid_1 == 0) && (count > 0)) {  // -------------> Execute first command
        if (count > 1)
            if (dup2(fd_1[1], STDOUT_FILENO) == -1) {
                perror("dup2 failed (write) 1");
            }
        close(fd_1[0]);
        close(fd_1[1]);

        if ((count < 2) && (redirect == 1)) {
            int endFD = open(endPath, O_WRONLY | O_CREAT | O_TRUNC, 0600);
            if (endFD == -1) {
                perror("open dir 1");
            }
            if (dup2(endFD, STDOUT_FILENO) == -1) {
                perror("dup2 open file failed 1");
            }
        }

        if (!handle(cmd_1)) {
            execvp(cmd_1->exe_path, cmd_1->arg);
            fprintf(stderr, "No such command: %s\n", cmd_1->exe_path);
        }

        exit(0);
    }
    int fd_2[2];
    if (pipe(fd_2) < 0) {
        perror("pipe");
        return;
    }
    if ((cmd_2 != NULL) && (count > 1)) {  // -------------> Execute second command
        pid_2 = fork();
        if (pid_2 < 0) {
            perror("fork 2");
            return;
        }
        if (pid_2 == 0) {
            if (dup2(fd_1[0], STDIN_FILENO) == -1) {
                perror("dup2 failed  (read) 2");
            }
            if (cmd_3 != NULL) {
                if (dup2(fd_2[1], STDOUT_FILENO) == -1) {
                    perror("dup2 failed  (write) 2");
                }
                close(fd_2[0]);
                close(fd_2[1]);
            }
            if ((count < 3) && redirect == 1) {
                int endFD = open(endPath, O_WRONLY | O_CREAT | O_TRUNC, 0600);
                if (endFD == -1) {
                    perror("open dir 1");
                }
                if (dup2(endFD, STDOUT_FILENO) == -1) {
                    perror("dup2 open file failed 2");
                }
            }

            close(fd_1[0]);
            close(fd_1[1]);

            if (!handle(cmd_2)) {
                execvp(cmd_2->exe_path, cmd_2->arg);
                fprintf(stderr, "No such command: %s\n", cmd_2->exe_path);
            }
            exit(0);
        }
    }
    if ((cmd_3 != NULL) && (count > 2)) {  // -------------> Execute third command
        pid_3 = fork();
        if (pid_3 < 0) {
            perror("fork 3");
            return;
        }
        if (pid_3 == 0) {
            if (dup2(fd_2[0], STDIN_FILENO) == -1) {
                perror("dup2 failed  (read) 3");
            }
            if (redirect == 1) {
                int endFD = open(endPath, O_WRONLY | O_CREAT | O_TRUNC, 0600);
                if (endFD == -1) {
                    perror("open dir 1");
                }
                if (dup2(endFD, STDOUT_FILENO) == -1) {
                    perror("dup2 open file failed 3");
                }
            }

            close(fd_2[0]);
            close(fd_2[1]);
            if (!handle(cmd_3)) {
                execvp(cmd_3->exe_path, cmd_3->arg);
                fprintf(stderr, "No such command: %s\n", cmd_3->exe_path);
            }
            exit(0);
        }
    }
    close(fd_1[0]);
    close(fd_1[1]);
    close(fd_2[0]);
    close(fd_2[1]);
    wait(NULL);
    wait(NULL);
    wait(NULL);

    return;
}
