//Output :
//harsh@harsh-VirtualBox:~$ ./a.out date
//Valid arguments have been passed
//This is the CHILD process, with id 4502
//Saturday 13 November 2021 10:47:24 AM IST
//Child exited with status 0 
//
//harsh@harsh-VirtualBox:~$ ./a.out wc
//Valid arguments have been passed
//This is the CHILD process, with id 4562
//
//harsh@harsh-VirtualBox:~$ ./a.out cat
//Valid arguments have been passed
//This is the CHILD process, with id 4598
//
//harsh@harsh-VirtualBox:~$ ./a.out pwd
//Valid arguments have been passed
//This is the CHILD process, with id 4643
// /home/harsh
//Child exited with status 0 

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char **argv)
{
	//Checking the argument count
	if (argc < 2)
	{
		printf(" No arguments have been  passed \n ./exe_child Usage:  ./exe_child args...\n");	
		return 0;
	}
	else
	{
		printf("Valid arguments have been passed\n");
		//To fetch the command passed through the command line
		char *commandarray[argc];
		//Declaring variables
		int i;
		for (i = 0; i < argc-1; i++)
		{
			//Storing the arguments passed in the command line into the commandarray
			commandarray[i] = argv[i+1];
		}
		commandarray[i] = NULL;
			
		//Create a child process
		int cpid = fork();
		
		//Check the creation of the child process whether it was successful or not
		if(cpid < 0)
		{
			printf("Error: fork() failed");
			return 1;
		}
		//If the child process creation is successful
		else if (cpid == 0)
		{
			//Fetching the child process id
			int c_pid = getpid();
			//Printing the Child Process with its id 
			printf("This is the CHILD process, with id %d\n", c_pid);
		        //int execvp(const char *file, char *const argv[]);			   //The exec() family of functions replaces the current 			   process image with a new process image.	
			//These functions duplicate the actions of the shell in searching for an executable file if the specified filename does not contain a slash (/) character.The  file is sought in the colon-separated list of directory pathnames specified in the PATH environment  variable.			//Execute the command sent as argument
			if(execvp(argv[1], commandarray) < 0)
			{ 
				perror(commandarray[0]);
				exit(1);
			} 
			else
			{
				printf("execvp not executed\n");
			}
		}
		else
		{
			//Parent process
			int status;
			int wpid = wait(&status);
			
			// WIFEXITED(wstatus)returns true if the child  terminated  normally,  that  is, by calling exit(3) or _exit(2), or by returning from main().
			//WEXITSTATUS(wstatus)returns  the  exit  status  of the child.  This consists of the least significant 8 bits of the status argument that the  child specified  in  a call to exit(3) or _exit(2) or as the argument for a return statement in main().  This  macro  should  be  employed only if WIFEXITED returned true.

		
			//Checking exit status of the Child Process 
			//Check exit status of child
			if(WIFEXITED(status))
			{
				printf("Child exited with status %d \n", WEXITSTATUS(status));
			}
			else
			{
			printf("Child terminated abnormally \n");
			printf("Child %d exited with code %d \n",wpid,WEXITSTATUS(status));
			}
					
		exit(1);	
	
		}
	}	


}
