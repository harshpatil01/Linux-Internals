#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define BUFF_SIZE 255
#define SUCCESS 0
#define FAILURE -1

int my_copy(int source_fd, int dest_fd);
int main(int argc, char *argv[])
{
    int fd1, fd2, read_len;
    char choice;

    // For -p to copy the permission mode
    struct stat sb;

    // Checking the argument count
    if (argc < 3)
    {
        printf("Insufficient arguments\n");
        printf("Usage: %s [option] <source file> <destination file>\n", argv[0]);
        return FAILURE;
    }

    // Checking for -p option in the argument
    if (strcmp(argv[1], "-p") == 0)
    {
        printf("Permissions also copied from source file to destination file\n");
        // Opening the source file to read the file hence in read-only mode
        // If the fd is equal to -1 then print error message
        if ((fd1 = open(argv[2], O_RDONLY)) == -1)
        {
            perror("open");
            return FAILURE;
        }

        // Fetching the permission of the source file
        if (stat(argv[2], &sb))
        {
            perror("stat");
            close(fd1);
            return FAILURE;
        }

        // Overwriting Process
        // Creating a file with the same name if the user decides to overwrite the already existing file
        if ((fd2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666)) != -1)
        {
            printf("File %s is already exists.\nDo you want to overwrite (y/n): ", argv[3]);
            scanf("\n%c", &choice);

            if (choice == 'N' || choice == 'n')
            {
                close(fd1);
                close(fd2);
                return 0;
            }
            else if (choice == 'Y' || choice == 'y')
            {
                // Overwrite the file
                if ((fd2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
                {
                    perror("open");
                    close(fd1);
                    return FAILURE;
                }
            }
        }
        else
        {
            // To create and open the destination file to copy the data
            // Opening the file in write-only mode
            // Creating a new file if the file does not exist
            // Using O_TRUNC to truncate the file
            // The file length will be reduced to zero
            // Giving the permissions to the destination file with initial permissions of read and write to the initial destination file (owner, group, and user)
            fd2 = open(argv[3], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        }

        // Changing the permissions of the destination file
        // Use chmod to change the permissions of the file
        // Permissions of the file are stored in the mode hence using the structure variable access method changing it to read, write, and execute for the owner, group, and user
        if (chmod(argv[3], sb.st_mode & 07777))
        {
            perror("chmod");
            close(fd1);
            close(fd2);
            return FAILURE;
        }
    }
    // If no -p option is passed in the command line i.e the user does not want to copy the source file permissions to the destination file
    else
    {
        // Opening the source file to read the file hence in read-only mode
        // If the fd is equal to -1 then print error message
        if ((fd1 = open(argv[1], O_RDONLY)) == -1)
        {
            perror("open");
            return FAILURE;
        }

        // Overwriting Process
        // Creating a file with the same name if the user decides to overwrite the already existing file
        if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) != -1)
        {
            printf("File %s is already exists.\nDo you want to overwrite (y/n): ", argv[2]);
            scanf("\n%c", &choice);

            if (choice == 'N' || choice == 'n')
            {
                close(fd1);
                close(fd2);
                return 0;
            }
            else if (choice == 'Y' || choice == 'y')
            {
                // Overwrite the file
                if ((fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666)) == -1)
                {
                    perror("open");
                    close(fd1);
                    return FAILURE;
                }
            }
        }
        else
        {
            // To create and open the destination file to copy the data
            // Opening the file in write-only mode
            // Creating a new file if the file does not exist
            // Using O_TRUNC to truncate the file
            // The file length will be reduced to zero
            // Giving the permissions to the destination file with initial permissions of read and write to the initial destination file (owner, group, and user)
            fd2 = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0666);
        }
    }

    // Calling the copy function for copying the data from the source file to the destination file after the files have been created successfully
    if (my_copy(fd1, fd2) == FAILURE)
    {
        close(fd1);
        close(fd2);
        return FAILURE;
    }

    // Close the source file
    close(fd1);
    // Close the destination file
    close(fd2);

    return SUCCESS;
}

// Definition of the copy function
int my_copy(int source_fd, int destination_fd)
{
    // Initializing the variable
    int read_length = 0;
    // Declaring a buffer array
    char buff[BUFF_SIZE];

    // Operations: Reading the data from the source file and copying it to the buffer array till the source file reaches the end of the file
    while (((read_length = read(source_fd, buff, BUFF_SIZE)) != -1) && (read_length != 0))
    {
        // Operations: Writing the text from the buffer array and copying it to the destination file till the end of the buffer array
        if (write(destination_fd, buff, read_length) == -1)
        {
            // Printing the error message if the operation is not successful
            perror("write");
            return FAILURE;
        }
    }

    return SUCCESS;
}
