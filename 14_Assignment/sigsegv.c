//Output:
//harsh@harsh-VirtualBox:~$ ./a.out
//Segmentation fault ! 
//Address 0xabcd caused error

#include <stdio.h>
#include <unistd.h>
#include <sys/signal.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>


	   
 /*	   
 void handler(int sig, siginfo_t *info, void *ucontext)
       sig    The number of the signal that caused invocation of the
              handler.
       info   A pointer to a siginfo_t, which is a structure containing
              further information about the signal, as described below.
       ucontext
              This is a pointer to a ucontext_t structure.  The structure pointed to by this field contains signal context information that was saved on the user-
              space stack .
*/			  
void handler(int signum, siginfo_t *info, void *dummy)
{
	printf("Segmentation fault ! \n");
	printf("Address %p caused error\n", info -> si_addr);
	exit(1);
}
	

int main()
{
	//Declaring a pointer of int type and storing a address which is likely to cause the segmentation fault
	int *ptr = (int *)0xABCD;
	
	//Declaring a struct variable of type sigaction
	struct sigaction act, oldact;
	
	//Prerequists for using sigaction
	//Memory allocation for the struct sigaction variable
	//void *memset(void *ptr, int x, size_t n);
	memset(&act, 0, sizeof(act));
	act.sa_sigaction = handler;
	//SA_RESETHAND: Restore the signal action to the default upon entry to the signal handler.  This flag is meaningful only when establishing a signal handler.
	//If SA_SIGINFO is specified in sa_flags, then sa_sigaction(instead of sa_handler) specifies the signal-handling function for signum.
	act.sa_flags = SA_SIGINFO | SA_RESETHAND;
	
//Sigaction to catch SIGSEGV signal
//Definition of handler
//The sigaction() system call is used to change the action taken by a process on receipt of a specific signal.
//int sigaction(int signum, const struct sigaction *restrict act, struct sigaction *restrict oldact);
 //signum specifies the signal and can be any valid signal except SIGKILL and SIGSTOP.
 //The sigaction structure is defined as something like:

			  // struct sigaction {
               //void     (*sa_handler)(int);
               //void     (*sa_sigaction)(int, siginfo_t *, void *);
               //sigset_t   sa_mask;
               //int        sa_flags;
               //void     (*sa_restorer)(void);
  /*1.sa_handler specifies the action to be associated with signum and
       is be one of the following that is  SIG_DFL for the default action,
	   SIG_IGN to ignore this signal or the signal handler function
       2. A pointer to a signal handling function.  
	   3. sa_mask specifies a mask of signals which should be blocked.
	   4. sa_flags specifies a set of flags which modify the behavior of
       the signal.It is formed by the bitwise OR of zero or more.
	*/
	//Registering the signal
	sigaction(SIGSEGV, &act, &oldact);
	//To cause the error we store a integer value to the pointer which already contains some value for the segmentation fault error to occur
	*ptr = 10;

	return 0;

}

