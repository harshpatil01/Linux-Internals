#include <stdio.h>
#include <unistd.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

//METHOD 1 : Catch the SIGCHLD using signal handler and wait function inside the signal handler
void handler_sigchld(int status)
{
    //Declaring pid of the type pid_t
    pid_t pid;
    //Making the parent process wait until the child process finishes execution and terminates
    pid = wait(&status);
    //Printing the pid of the child process with exit code
    printf("Child %d terminated with exit code %d \n", pid, WEXITSTATUS(status));
    exit(1);
}

int main()
{
    //Declaring variables
    pid_t pid;
    int status;
    //Create a child process
    pid = fork();

    //To catch SIGCHLD
    //void (*signal(int sig, void (*func)(int)))(int)
    //sig : This is the signal number to which a handling function is set.
    //func : This is a pointer to  the signal handler function
    //SIGCHLD is raised when a child process terminates
    signal(SIGCHLD, handler_sigchld);
    
    
    //Checking if the child process operation has been successfully created or not
    if(pid < 0)
    {
        printf("Creation of child process is not successful");
        return 1;
    }
    else if (pid == 0)
    {
        //Child process
        exit(0);
    }
    else
    {
        //Parent process
        sleep(2);
        exit(0);
    }
}

//Method 2
// Create a child process.
// Use sa_flag from struct sigaction to avoid zombie
// Prints the child exit status inside handler.


#include <stdio.h>
#include <unistd.h>
#include <sys/signal.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

//METHOD 2 : Catch using sigaction
void handler_sigchld(int signum, siginfo_t *info, void *dummy)
{
    //Printing the termination of the child process
    printf("Child %dterminated with exit code %d \n", info -> si_pid, info -> si_code);
    exit(1);
}

int main()
{
    //Declaring variables
    pid_t pid;
    int status;
    //Declaring a struct variable of type sigaction
    struct sigaction sa;
    //Prerequists for using sigaction
    //Memory allocation for the struct sigaction variable
    //void *memset(void *ptr, int x, size_t n);
    memset(&sa, 0, sizeof(sa));
    sa.sa_sigaction = handler_sigchld;
    //sigemptyset simply initializes the signalmask to empty, such that it is guaranteed that no signal would be masked. (that is, all signals will be received)
    sigemptyset(&sa.sa_mask);
    //If SA_SIGINFO is specified in sa_flags, then sa_sigaction(instead of sa_handler) specifies the signal-handling function for signum.
    //SA_NOCLDSTOP flag prevents SIGCHLD from being raised when a child process stops or continues (as opposed to terminating)   
    //SA_NOCLDWAIT flag prevents the child process from becoming a zombie after its termination
    sa.sa_flags = SA_SIGINFO | SA_NOCLDSTOP | SA_NOCLDWAIT;

    //Registering the signal
    //Checking if its successful or not or else giving the error
    if (sigaction(SIGCHLD, &sa, 0) == -1)
    {
        perror(0);
          exit(0);
    }

    //Create a child process
    pid = fork();
    //Checking if the child process operation has been successfully created or not
    if(pid < 0)
    {
        printf("Creation of child process is not successful");
        return 1;
    }
    else if (pid == 0)
    {

        //Child Process
        printf("Child Process: %d\n", getpid());
        exit(0);
    }
    else
    {
        //parent process
        printf("Parent Process: %d\n", getpid());
        sleep(2);
        exit(0);
    }
}
