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


int main(int argc, char *argv[]) 
{
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

	
	return 0;
}
	
