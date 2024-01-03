//Output :
//Child 1 with pid 9238 created
//Child 2 with pid 9239 created
//Child 3 with pid 9240 created
//Child 9238 exited with status 0 
//Child 9239 exited with status 0 
//Child 9240 exited with status 0 




#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>





int main()
{

	//Creation of a Child
	int cpid1 = fork();
	
	//Checking if the Child Process 1 has been created successfully or not
	if(cpid1 < 0)
	{
		printf("Error: fork() failed");
		return 1;
	}
	//If the Child Process is created successfully 
	else if (cpid1 == 0)
	{
		//Child Process 1 successfully created
		printf("Child 1 with pid %d created\n", getpid());
		
		//Exit the Child Process 1
		exit(0);
	}
	else
	{
		//Parent Process
		
		//Creation of the new child process 2
		int cpid2 = fork();
		
		//Checking if the child process is created successfully or not
		if(cpid2 < 0)
		{
			printf("Error: fork() failed");
			return 1;
		}
		//If Child Process 2 is created successfully
		else if (cpid2 == 0)
		{
			//Child Process 2 successfully created
			printf("Child 2 with pid %d created\n", getpid());
			
			//Exit the Child Process 2
			exit(0);
		}
		else
		{
			//Creation of the new child process 3
			int cpid3 = fork();
			
			
			//Checking if the child process is created successfully or not
			if(cpid3 < 0)
			{
				printf("Error: fork() failed");
				return 1;
			}
			//If child process is created successfully
			else if (cpid3 == 0)
			{
				//Child Process 3 successfully created
				printf("Child 3 with pid %d created\n", getpid());
				
				//Exit the Child Process 3
				exit(0);
			}
			else
			{
				//Parent Process
				int status;
				int wpid;
				int i = 0;
				sleep(1);
				
				
				//For Loop 
				for(i = 0; i < 3; i++)
				{

				        //pid_t wait(int *wstatus)
//The wait() system call suspends execution of the calling thread  until one of its children terminates.
					
//Wait for  the Child Process
					wpid = wait(&status);
					
					//Check the exit status of the child process
				
					if(WIFEXITED(status))
					{
						//Printing the Child Process after terminating and printing the way the child process has exited itself
						printf("Child %d exited with status %d \n", wpid, WEXITSTATUS(status));
					}
				}
				
						
			}			
		}	
	}
	return 0;
}
