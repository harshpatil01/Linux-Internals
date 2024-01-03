/*Output:
harsh@harsh-VirtualBox:~$ ./a.out
Error: No arguments passed
Usage: ./a.out <command 1> | <command 2> | <command 3>
harsh@harsh-VirtualBox:~$ ./a.out ls -l '|' grep ":" '|' wc -l
14
*/



#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <stdlib.h>

int pipecount = 0;
int twopipe(char *commandarray1[], char *commandarray2[],char *commandarray3[]);
void parse(int argc, char *argv[],char *commandarray1[],char *commandarray2[], char *commandarray3[]);

int main(int argc, char **argv)
{
	//Declaring variables and array of pointers
	int i;
	char *commandarray1[255];
	char *commandarray2[255];
	char *commandarray3[255];

	//Checking the argument count passed through the command line
	if(argc == 1)
	{
		printf("Error: No arguments passed\nUsage: ./a.out <command 1> | <command 2> | <command 3>\n");	
		return -1;
	}
	if(argc == 2)
	{
		printf("Error: Insufficient arguments passed\nUsage: ./a.out <command 1> | <command 2> | <command 3>\n");
    		return -1;
	}
	else
	{	
		//Parse for the commands passed through the command line
		parse(argc, argv, commandarray1, commandarray2, commandarray3);	
		//printf("1: %s %s \n", command1Array[0], command1Array[1]);	
		//printf("2: %s %s \n", command2Array[0], command2Array[1]);
		//printf("3: %s %s \n", command3Array[0], command3Array[1]);
	}	

	//Pipe Execution
	if(pipecount == 2)
	{
		//Calling the function for the two pipe implementation
		twopipe(commandarray1,commandarray2,commandarray3);
	}
	else
	{
		printf("Error: No arguments passed\nUsage: ./a.out <command     1> | <command 2> | <command 3>\n");
		return -1;
	}
}


//Definition of the twopipe function
int twopipe(char *commandarray1[], char *commandarray2[], char *commandarray3[])
{
	//Declaring array variables
	int pipefd1[2];
	int pipefd2[2];
	//Declaring variables of pid type
	pid_t firstchild;
	pid_t secondchild;
	pid_t thirdchild;

	//Creation of the first pipe
	pipe(pipefd1);

	//Check if the creation of the first child process is successful or not
	if ( (firstchild = fork()) == -1 )
	{ 
		//If the first child process is not successful
		perror("Operation not successful\n");
		exit(1);
	}
	//If the creation of the first child process is successful 
	else if (firstchild == 0)
	{
		//First Child Process
		//Redirecting to the parent process [stdout]
		//pipefd[0] refers to read end of the pipe
		//pipefd[1] refers to write end of the pipe
		//dup2 duplicates the process 
		//If the duplication of the process is not successful then print an error
		// int dup2(int oldfd, int newfd)
		//Assigning the 1 (stdout) to the pipefd[1]
		if (dup2(pipefd1[1],1) < 0)
		{
			perror("ERROR: dup2 failed!");
			exit(2);
		}

		//Close the file descriptor
		//Close the read end of the Pipe
		close(pipefd1[0]);
		close(pipefd1[1]);

		//Execute the command
		//int execvp(const char *file, char *const argv[])
		if(execvp(commandarray1[0], commandarray1)<0)
		{ 
			perror(commandarray1[0]);
			exit(3);
		}
	}

	//Creation of the second pipe
	pipe(pipefd2);

	//Check if the creation of the second child process is successful or not
	if ( (secondchild = fork()) == -1 )
	{ 
		//If the second child process is not successful
		perror("Operation not successful\n");
		exit(1);
	}
	//If the creation of the second child process is successful 
	else if (secondchild == 0)
	{
		//Close the write end of the pipefd 1
		close(pipefd1[1]);
		
		//Second Child Process
		//Redirecting to the parent process [stdin]
		//pipefd[0] refers to read end of the pipe
		//pipefd[1] refers to write end of the pipe
		//dup2 duplicates the process 
		//If the duplication of the process is not successful then print an error
		//int dup2(int oldfd, int newfd)
		//Assigning the 0 (stdin) to the pipefd[0]
		if (dup2(pipefd1[0],0)<0)
		{
			perror("ERROR: dup2 failed!");
			exit(5);
		}

	

		//Close the file descriptor of reading end of the pipe 2
		close(pipefd2[0]);

		//Redirecting from stdin input to the writing end of the pipe 2
		if (dup2(pipefd2[1],1)<0)
		{
			perror("ERROR: dup2 failed!");
			exit(5);
		}
		
		// close the file descriptor 
		close(pipefd2[1]);
		

		//Execute the command
		//int execvp(const char *file, char *const argv[])
		if(execvp(commandarray2[0], commandarray2)<0)
		{ 
			perror(commandarray2[0]);
			exit(3);
		}
	
	}
	
	// close the file descriptor for pipe1
	close(pipefd1[0]);
	close(pipefd1[1]);
	
		
	//Check if the creation of the third child process is successful or not
	if ( (thirdchild = fork()) == -1 )
	{ 
		//If the third child process is not successful
		perror("Operation not successful\n");
		exit(1);
	}
	//If the creation of the second child process is successful 
	else if (thirdchild == 0)
	{
		//Close the write end of the pipefd 1
		close(pipefd2[1]);
		
		//Third Child Process
		//Redirecting to the parent process [stdin]
		//pipefd[0] refers to read end of the pipe
		//pipefd[1] refers to write end of the pipe
		//dup2 duplicates the process 
		//If the duplication of the process is not successful then print an error
		//int dup2(int oldfd, int newfd)
		//Assigning the 0 (stdin) to the pipefd[0]
		if (dup2(pipefd2[0],0)<0)
		{
			perror("ERROR: dup2 failed!");
			exit(5);
		}


		
		//Close the file descriptor of reading end of the pipe 2
		close(pipefd2[0]);

	

		//Execute the command
		//int execvp(const char *file, char *const argv[])
		if(execvp(commandarray3[0], commandarray3)<0)
		{ 
			perror(commandarray3[0]);
			exit(3);
		}
		
	
	}
	
	//close the file descriptor 
	close(pipefd2[0]);
	close(pipefd2[1]);
	
	//pid_t wait(int *wstatus)
	//Wait command takes one argument as the status 
	//It returns a process ID of the terminated child process
	//wait(NULL) will block parent process until any of the child process are terminated.
	//If child process terminates before parent process reaches wait(NULL) then the child process turns to a zombie process until its parent waits on it and its released from memory.
	while(1)
	{
		pid_t pid;
		pid = wait(NULL);
		if(pid == secondchild)
		{
			break;
		}
		if(pid == firstchild)
		{
			break;
		}
		if(pid == thirdchild)
		{
			break;
		}
	}

}

//Definition of the parse command
void parse(int argc, char *argv[], char *commandarray1[], char *commandarray2[], char *commandarray3[])
{
	//Declaring variables 
	int i, count1 = 0, count2 = 0, count3 = 0;
		
	for (i = 1; i < argc; i++)
	{
		//check for '|' i.e a pipe
		if( strcmp(argv[i], "|") != 0)
		{
			//If the pipecount is equal to zero
			if(pipecount == 0)
			{
				//Saving the commands in the commandarray1 if the pipecount is 0
				commandarray1[count1++] = argv[i];
			}
			else if(pipecount == 1)
			{
				//Saving the commands in the commandarray2 if the pipecount is 1
				commandarray2[count2++] = argv[i];
			} 
			else if(pipecount == 2)
			{
				//Saving the commands in the commandarray3 if the pipecount is 2
				commandarray3[count3++] = argv[i];
			} 
			
		}
		else
		{
			//get the pipe count
			pipecount++;			
			continue;
		}
	}
	commandarray1[count1] = NULL;
	commandarray2[count2] = NULL;	
	commandarray3[count3] = NULL;	
}

