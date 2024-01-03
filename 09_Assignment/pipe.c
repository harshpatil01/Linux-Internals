//Output :
//harsh@harsh-VirtualBox:~$ ./a.out
//Error: No arguments passed
//Usage: ./pipe <command 1> | <command 2>
//harsh@harsh-VirtualBox:~$ ./a.out ls '|' wc
//     24      27     240
//harsh@harsh-VirtualBox:~$ ./a.out ls -l -a '|' wc -l -w
//     51    455 



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc < 4 || argc % 2 != 1) {
        fprintf(stderr, "Error: Insufficient or incorrect arguments passed\n");
        fprintf(stderr, "Usage: %s <command1> '|' <command2> ...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Calculate the number of pipes needed
    int num_pipes = (argc - 1) / 2;

    // Create an array to store pipe file descriptors
    int pipes[num_pipes][2];

    // Create pipes
    for (int i = 0; i < num_pipes; ++i) {
        if (pipe(pipes[i]) == -1) {
            perror("pipe");
            exit(EXIT_FAILURE);
        }
    }

    // Create child processes
    for (int i = 0; i < num_pipes + 1; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            exit(EXIT_FAILURE);
        }

        // Code for child processes
        if (pid == 0) {
            // Close unnecessary pipe ends
            for (int j = 0; j < num_pipes; ++j) {
                close(pipes[j][1]); // Close write end of the pipe
                if (j != i)
                    close(pipes[j][0]); // Close read end of other pipes
            }

            // Redirect input if not the first command
            if (i > 0) {
                dup2(pipes[i - 1][0], STDIN_FILENO);
            }

            // Redirect output if not the last command
            if (i < num_pipes) {
                dup2(pipes[i][1], STDOUT_FILENO);
            }

            // Execute the command
            execvp(argv[i * 2 + 1], &argv[i * 2 + 1]);
            perror("execvp");
            exit(EXIT_FAILURE);
        }
    }

    // Close all pipe ends in the parent process
    for (int i = 0; i < num_pipes; ++i) {
        close(pipes[i][0]);
        close(pipes[i][1]);
    }

    // Wait for all child processes to finish
    for (int i = 0; i < num_pipes + 1; ++i) {
        wait(NULL);
    }

    return EXIT_SUCCESS;
}

