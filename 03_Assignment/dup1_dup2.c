#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    // Check if the correct number of command-line arguments is provided
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <output_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Open the specified file for writing, create it if it doesn't exist, and truncate it to zero length
    int file_descriptor = open(argv[1], O_WRONLY | O_CREAT | O_TRUNC, 0666);

    if (file_descriptor == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    // Save the current STDOUT file descriptor
    int original_stdout = dup(STDOUT_FILENO);

    // Use dup to duplicate the file descriptor
    int new_stdout = dup(file_descriptor);

    if (new_stdout == -1) {
        perror("dup");
        close(file_descriptor);
        exit(EXIT_FAILURE);
    }

    // Use dup2 to redirect STDOUT to the specified file descriptor
    // This will close the previous file descriptor associated with STDOUT
    if (dup2(file_descriptor, STDOUT_FILENO) == -1) {
        perror("dup2");
        close(file_descriptor);
        exit(EXIT_FAILURE);
    }

    // Now, printf will write to the file specified in the command line
    printf("Print this message into STDOUT and also redirect it to the file.\n");

    // Close the redirected STDOUT file descriptor
    close(file_descriptor);

    // Restore the original STDOUT file descriptor
    if (dup2(original_stdout, STDOUT_FILENO) == -1) {
        perror("dup2");
        exit(EXIT_FAILURE);
    }

    // Verify that printf now writes only to the original STDOUT
    printf("This message goes to the original STDOUT.\n");

    return EXIT_SUCCESS;
}
