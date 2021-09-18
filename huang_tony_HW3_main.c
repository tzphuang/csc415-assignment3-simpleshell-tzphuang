/**************************************************************
* Class:  CSC-415-03 Fall 2021
* Name:	Tony Huang
* Student ID: 915090688
* GitHub Name: tzphuang
* Project: Assignment 3 – Simple Shell
*
* File: huang_tony_HW3_main.c
*
* Description: 
*
**************************************************************/
#include <stdio.h> 
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFFER_SIZE										180



int main(int argc, char *argv[]) 
{
	// to run this put into the command line
	// ./huang_tony_HW3_main & 
	// the & puts the process in the background
	/*
	int result = fork();
	switch (result)
	{
		//case -1 only activates when fork returns an error
		case -1:
		printf("ERROR! Switch statement error -1, something went wrong with result from fork().\n");
		break;

		//case 0 is always returned within the child
		case 0:
		while(1){
			//execlp("full path to executable file if using long format", "name of process", "extra options")
			//execlp("/bin/ls", "ls", NULL);
			printf("I am the child, my PID: %d, and my PPID is: %d.\n", getpid(), getppid());
			sleep(3);
		}
		break;
	
		//default case is for any PID greater than 0 
		//because for the parent fork() returns the pid of the child
		default:
		wait(NULL); //the parent needs to wait until the child finishes
		while(1){
			printf("I am the parent, my PID: %d, and my PPID is: %d.\n", getpid(), getppid());
			sleep(3);
		}
		break;
	}
	*/

	//buffer will hold the user's command inputs
	char buffer[BUFFER_SIZE+1];
	//printf("Buffer size of buffer is %d \n", BUFFER_SIZE+1);

	//parsedBuffer size should be able to hold a line of 180 bytes
	//if those bytes are separated by spaces, then you get 90 valid bytes
	//and we need 1 extra byte for null terminator
	char * parsedBuffer[(BUFFER_SIZE/2) +1];
	//printf("Buffer size of buffer is %d \n", (BUFFER_SIZE/2)+1);

	char * strExit = "exit";

	char * returnFgets;

	//while loop continues to take in input from user until user exits
	while(1){
		printf("Please input your command line!\n");
		returnFgets = fgets(buffer, BUFFER_SIZE, stdin);

		//if fgets() returns null we either have end of file or an error
		//we then check which one it is and proceed accordingly
		if (NULL == returnFgets){
			//this can be forcefully activated when you input ctrl + d into the command line
			if(feof(stdin)){
				printf("End of file! Exiting Gracefullly.\n");
				break;
			}

			if(ferror(stdin)){
				printf("Error! Input error for fgets().");
				break;
			}
		}

		printf("Buffer now contains: %s, and stringComparison is: %s\n", buffer, strExit);

		int stringComparison = strcasecmp(buffer, strExit);
		printf ("string Comparison is: %d\n", stringComparison);

		stringComparison = strncasecmp(buffer, strExit, 4);
		printf ("string n Comparison is: %d\n", stringComparison);

		//string comparison for the first 4 characters to see if its "EXIT"
		if(	strncasecmp(strExit, buffer, 4)	== 0){
			printf("User has input \"Exit\", program will exit gracefully.\n");
			break;
		}
		else{
			printf("User input was: \"%s\"\n", buffer);
		}

		
	}

	
	return 0;
}
	
