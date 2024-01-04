//Output:
//Child 4904 terminated with exit code 0 






/*
By using a signal handler: The parent process installs a signal handler for the SIGCHLD signal. The signal handler calls wait() system call within it. In this scenario, when the child terminated, the SIGCHLD is delivered to the parent. On receipt of SIGCHLD, the corresponding handler is activated, which in turn calls the wait() system call. Hence, the parent collects the exit status almost immediately and the child entry in the process table is cleared. Thus no zombie is created.
*/

/*Method 1 
 	1. Create a child process. 
 	2. Write a signal handler function for  SIGCHLD to avoid child become zombie       
 	(Do man 7 signal for SIGCHLD) . 
 	3. Write code inside handler to avoid zombie and print child exit status.
*/

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
