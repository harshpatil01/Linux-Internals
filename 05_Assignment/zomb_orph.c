#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main()
{
	int cpid;
	
	//Creating a child process and returning the process id 
	cpid =fork();
	
	//If the child process is not created then print a error
	if(cpid < 0)
	{
		printf("Error: fork() operation failed");
		return 1;
	}
	//If cpid is 0 because child process gets zero in return whereas the parent process gets the child process
	else if (cpid == 0)
	{
	  printf("Child Process with pid %d has been created\n",getpid());
		//Creating a buff array
		char buff[100];
		
		//Fetching the parent process id using getpid command
		int pid = getpid();
		//Writing the format into the buf array
		sprintf(buff, "cat /proc/%d/status | head -5", pid);
		//Calling the call in the parent process using the system call function
		system(buff);
		
		//exit child process
		exit(1);
	}
	else
	{
		//Parent process made to sleep
		sleep(2);
		
		//Printing the status
		char buff[100];
		//Writing the format into the buf array
		sprintf(buff, "cat /proc/%d/status | head -5", cpid);
	
		//Calling the call in the parent process using the system call function
		system(buff);

		//To create a second child
		int cpid2;
		//Creating a child process and returning the process id 
		cpid2 = fork();		

		
		
		//If the child process is not created then print a error
		if(cpid2 < 0)
		{
			printf("Error: fork() failed");
			return 1;
		}
		//If cpid is 0 because child process gets zero in return whereas the parent process gets the child process
		else if (cpid2 == 0)
		{
			//second child process
			sleep(2);
			
			//Confirming if the system call is not returning -1 
			if(system(buff) != -1)
			{
				printf("Process %d cleared by init", cpid);
		
			}
			else
			{
				printf("Error\n");
			}			
					
			
			//Exiting child process 2
			exit(1);
		}
		else
		{
		
			//Exiting Parent Process 
			exit(1);
		}
	}
}
