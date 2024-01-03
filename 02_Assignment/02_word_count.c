//Output : harsh@harsh-VirtualBox:~$
//vi f.txt
// Hello  how are you
// ./a.out f.txt
//1   4  20 f.txt

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#define BUFF_SIZE 255
#define SUCCESS 0
#define FAILURE -1

int line_flag = 0, word_flag = 0, char_flag = 0;
	
//Typedef	
typedef int count_t;
count_t ccount;
count_t wcount;
count_t lcount; 

count_t total_ccount =0; 
count_t total_wcount =0; 
count_t total_lcount =0; 

//Declaring functions
int word_count(int fd, int *lines, int *words, int *bytes);
int process_word_count(char *pathname);
void print_report(char *pathname, int lines, int words, int bytes, int l_flag, int w_flag, int c_flag);

int main(int argc, char **argv)
{
	//Declaring variables
	int i;
	int c, index = 0 ;
	//Declaring global variables
	extern int optind, optopt;
	extern char *optarg;

	//Fetching the flags using getopt and set the flags accordingly
	//To fetch the options such as -l , -w and -c
	while ((c = getopt(argc, argv, "clwn:")) != -1)
	{
		switch (c)
      		{
			case 'l': 
				line_flag = 1;
				break;
			case 'w': 
				word_flag = 1;
				break;
			case 'c': 
				char_flag = 1;
				break;
			case '?':
				fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				return -1;	
		}
	}

	//If no files are given read from stdin
	if(argv[optind] == NULL)
	{
		//Duplicating the stdin file descriptor to fd
		int fd = dup(0);
        //Reading from stdin and get the word count
		ccount = 0;
		wcount = 0;
		lcount = 0;	

		//Fetching the word count 
		//Calling the word_count function 
		word_count(fd, &lcount, &wcount, &ccount);

		//Close the file pointer
		close(fd);
		
		//Print the file and their respective word count
		//Calling the print_report function
		print_report("stdin", lcount, wcount, ccount, line_flag, word_flag, char_flag);
			
	}
	else
	{
		//For multiple file cases using the for loop to process each file
		for (i = optind; i < argc; i++)
		{
			//Increment index variable
			index++;
			//Calling the process_word_count function
			if (process_word_count(argv[i]) == FAILURE)
			{
				printf("Processing the multiple files have failed \n");
				return -1;
			}	
		}

		//Print the file and respective word counts after it has processed the each file passed through the command line arguments
		if (index > 1)
		{
			print_report("total ", total_lcount, total_wcount, total_ccount,0,0,0);
		}
	}	

}

//To process words i.e to open the file, get the word count, and then print accordingly
int process_word_count(char *pathname)
{
	//Declaring file variable fd
	int fd;
	//To open file to read the file name passed through the command line arguments
	if ( (fd = open(pathname, O_RDONLY)) == -1 )
	{
		perror("open");
		return FAILURE;	
	}
	ccount = 0;
	wcount = 0;
	lcount = 0;	

	//To get the word count
	//Calling the function
	word_count(fd, &lcount, &wcount, &ccount);

	//Close the file pointer fd
	close(fd);
	
	//Print the file and respective word counts
	//Calling the function
	print_report(pathname, lcount, wcount, ccount, line_flag, word_flag, char_flag);
		//Total character count , word count and line count of all the files passed through the command line arguments
		total_ccount += ccount;
      	total_wcount += wcount;
       	total_lcount += lcount;
}


//To get the word count, line count and bytes count
int word_count(int fd, int *lines, int *words, int *bytes)
{
	int read_len = 0, temp_line = 0, temp_words = 0, i, sp ;
	char buff[BUFF_SIZE]; 
	
	//Traversing the loop until it reaches the end of the file or the statements
	while( ((read_len = read(fd, buff, BUFF_SIZE)) != -1 ) && (read_len != 0))
	{
		//Number of characters read store it in variable pointed by bytes
		*bytes = read_len;

		//Number of lines and words
		for(i = 0; i < read_len; i++)
		{
			//To fetch the lines
			//Traversing till the end of the lines
			if(buff[i] == '\n')
			{
				//Increment temp_line
				temp_line++;
			}
			
			//To fetch the  words
			//Traversing till the end of total words
			if(buff[i] == ' ' || buff[i] == '\n') 
			sp = 1;
        		else if(sp) 
				{
					//Increment temp_words
       	    		++temp_words;
					//Set sp to zero
			        sp = 0;
		        }
		}
	
	}
	//Number of lines read store it in variable pointed by bytes
	//Number of words read store it in variable pointed by bytes
	*lines = temp_line;
	*words = temp_words;	
}

//Print the report depending on the flags set
void print_report(char *pathname, int lines, int words, int bytes, int l_flag, int w_flag, int c_flag)
{
	//Check for flag outputs
			
	if (line_flag == 1)
	{
		if (word_flag == 1)
		{
			if (char_flag == 1)
			{
				//If all flags are set
				printf ("%d %3d %3d %s\n", lines, words, bytes, pathname);
			}
			else
			{
				//If -l and -w is set
				printf ("%d %3d %s\n", lines, words, pathname);
			}
		}
		else if (char_flag == 1)
		{
			//If -l and -c is set
			printf ("%d %3d %s\n", lines, bytes, pathname);
		}
		else
		{
			//If -l is set
			printf ("%d %s\n", lines, pathname);
		}
	}
	else if (word_flag == 1)
	{
		if (char_flag == 1)
		{
			//If -w and -c is set	
			printf ("%d %3d %s\n", lines, bytes, pathname);
			
		}
		else
		{
			//If -w is set
			printf ("%d %s\n", words, pathname);
		}	
	}
	else if (char_flag == 1)
	{
		//If -c is set
		printf ("%d %s\n", bytes, pathname);
	}
	else
	{
		//If none are set 
		printf ("%d %3d %3d %s\n", lines, words, bytes, pathname);
	}		
	
}
