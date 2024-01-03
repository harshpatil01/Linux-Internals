//Output:
//Parent is running...
//Parent is running...
//Parent is running...
//Parent is running...
//Parent is running...
//Child 8829 terminated normally with exit status 3 



#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
	//Declaring variables
	int cpid;
	int status;
	
	//Creation  of a child process
	cpid =fork();
	
	//If the child process is not created successfully or not
	if(cpid < 0)
	{
		printf("Error: fork() failed");
		return 1;
	}
	//If the child process has been successfully created by the parent process
	else if (cpid == 0)
	{
		//Child Process
		//Sleep for five seconds
		sleep(5);
		//Exit the child process 
		exit(3);
		
	}
	else
	{
		//Parent Process
		//Declaring variables
		int wpid;
		do
		{
			
   //pid_t waitpid(pid_t pid, int *wstatus, int options);
//The waitpid() system call suspends execution of the calling thread until  a child specified by pid argument has changed state.  By default,waitpid() waits only for terminated children,  but  this  behavior  is modifiable via the options argument, as described below.	    
 //Checking if the child  process is terminated or not
//cpid is the child process id of the process to which we have to wait for the termination
//WNOHANG is the option that returns immediately if no child has exited.
		    
			wpid = waitpid(cpid, &status, WNOHANG);
			if(wpid == 0)
			{
				printf("Parent is running...\n");
			}
			sleep(1);
			
		}while(wpid == 0);	
		





// WIFEXITED(wstatus)returns true if the child  terminated  normally,  that  is, by calling exit(3) or _exit(2), or by returning from main().
//WEXITSTATUS(wstatus)returns  the  exit  status  of the child.  This consists of the least significant 8 bits of the status argument that the  child specified  in  a call to exit(3) or _exit(2) or as the argument for a return statement in main().  This  macro  should  be  employed only if WIFEXITED returned true.

		
//Checking exit status of the Child Process 
if(WIFEXITED(status))
{
printf("Child %d terminated normally with exit status %d \n", cpid, WEXITSTATUS(status));
}
exit(1);
}
return 0;
}
