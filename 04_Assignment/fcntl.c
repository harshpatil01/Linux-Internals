#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main(int argc, char **argv)
{
	//Declaring variables
	int fd; 
	//Process Associated Locks 
	struct flock lock;
	pid_t pid;
	int status;

		//Argument Check 
    	if (argc < 2)
    	{
        	printf("Insufficient arguments\n");
		printf("Usage:- ./a.out <file> \n");
        	return 1;
    	}	

	
	//Opening the file on which synchronization is performed
	if ( (fd = open(argv[1], O_APPEND|O_WRONLY)) == -1 )
	{
		perror("open");
		return -1;	
	}
	
//memset() is used to fill a block of memory with a particular value.
//The syntax of memset() function is as follows :
// ptr ==> Starting address of memory to be filled
// x   ==> Value to be filled
// n   ==> Number of bytes to be filled starting 
//		   from ptr to be filled
//void *memset(void *ptr, int x, size_t n);

	//Initialize the flock structure
	memset (&lock, 0, sizeof(lock));

	//To set the write lock 
	lock.l_type = F_WRLCK;
	//Place a write lock on the file 
	//File that has been opened on which the synchronization process has to be performed
	//F_GETLK, F_SETLK and F_SETLKW are used to acquire, release, and test for the existence of record locks (also known as file-segment or file-region locks).
	fcntl (fd, F_SETLKW, &lock);

	//Create a new process
	pid = fork();
    if(pid < 0)
	{
	        perror("Operation not successful");
	        return 1;
	}

	if(pid == 0)
	{
			//Child Process
			sleep(2);
			//If the lock is present wait for the release of the lock
		 if ( fcntl(fd, F_SETLK, &lock) == 0)
		 {
			printf ("The child is locked You will have to enter the unlock \n");
			//Waiting for the user
 			getchar ();

			printf ("Child Process is unlocking \n");
			//Releasing the lock
 			lock.l_type = F_UNLCK;
			fcntl (fd, F_SETLKW, &lock);
		 }	
		 else
		{
			printf ("Child Process is unlocking \n");
			//Initialize the flock structure
 			memset (&lock, 0, sizeof(lock));
 			//Setting the write lock
			lock.l_type = F_WRLCK;
			//Placing a write lock on the file 
 			fcntl (fd, F_SETLKW, &lock);
			
		}
         	exit(1);

	}
	else
	{
		//Parent Process
		sleep(5);
		int wpid = waitpid(&status);
			
		//If lock is present wait for release
		 if (fcntl(fd, F_SETLK, &lock) == 0)
		 {
			printf ("Parent Process is locked Please hit enter to unlock ");
 			//Waiting for the user
 			getchar ();
			printf ("Parent Process unlocking\n");
			//Release the lock
 			lock.l_type = F_UNLCK;
			fcntl (fd, F_SETLKW, &lock);
		 }	
		 else
		{
			printf ("Parent Process locked\n");
			//Initialize the flock structure
 			memset (&lock, 0, sizeof(lock));
 			//Setting the write lock
			lock.l_type = F_WRLCK;
			//Placing a write lock on the file
 			fcntl (fd, F_SETLKW, &lock);
		
		}
        //Close fd
		close (fd);

	}		

	
	
 	return 0;
}
