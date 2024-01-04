/*Output :
Terminal 1 :
Process 4489 waiting for signal
^C
 SIGINT recieved
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Inside Handler...
Aborted (core dumped)

Terminal 2 :
kill -SIGABRT 4489
kill -SIGUSR1 4489
kill -SIGTERM 4489
*/



#include <stdio.h>
#include <unistd.h>
#include <sys/signal.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void handler(int signum, siginfo_t *info, void *dummy)
{
    int i = 20;
    printf("\n SIGINT recieved \n");
    while(i)
    {
        printf("Inside Handler...\n");
        i--;
        sleep(2);
    }
}

int main()
{
    //Declaring a variable sa of type struct sigaction
    struct sigaction sa;
    //Declaring a signal set of block_mask
    sigset_t block_mask;
    
    //Initializing the memory
    memset(&sa,0,sizeof sa);
    //Initializes a signal set set to the empty set
    //int sigemptyset(sigset_t *set)
    sigemptyset(&block_mask);
 
    //Block other terminal-generated signals while handler runs
    //User defined signal
    //Adds a signal to the set of signals already recorded in set
    //int sigaddset(sigset_t *set, int signal);
    //The SIGUSR1 and SIGUSR2 signals are set aside for you to use any way you want. The signals are used for simple interprocess communication, if you write a signal handler for them in the program that receives the signal.
    sigaddset (&block_mask, SIGUSR1);
    //Adding the Aborted signal to the signal set
    //SIGABRT errors are caused by your program aborting due to a fatal error
      sigaddset (&block_mask, SIGABRT);
    //The SIGTERM signal is a generic signal used to cause program termination
    //Adding the Termination signal to the signal set
      sigaddset (&block_mask, SIGTERM);
     
    //Setting the sa_mask to the signal set that is the block_mask that we have created
    //In this way it will block the sigusr1 ,sigabrt and sigterm signals sent to it by the kill process command from another command line terminal
    sa.sa_mask = block_mask;
    
    //Setting the sigaction to the handler that we have defined
    sa.sa_sigaction = handler;   
    //Setting the flags for the options
     sa.sa_flags = SA_SIGINFO ;
 
    //Printing the Process along with its pid
    printf("Process %d waiting for signal\n", getpid());
    //Registering the signal
    //After pressing ctrl + c it will go to handler
    sigaction (SIGINT, &sa, 0);
    //
    sleep(30);
}
