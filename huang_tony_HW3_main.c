/**************************************************************
* Class:  CSC-415-03 Fall 2021
* Name:	Tony Huang
* Student ID: 915090688
* GitHub Name: tzphuang
* Project: Assignment 3 – Simple Shell
*
* File: huang_tony_HW3_main.c
*
* Description: Program imitates the bash shell by prompting the user for input.
* Program then takes the input and calls the system call "execvp" to execute
* input if applicable if not, the user is prompted again for more input
* program exits on "exit"
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

char * getInputFromUser(char * buffer, char * promp);
void tokenizeStoreString(char * string, char * parsedStrings[]);
void printArray(char * stringArray[]);


int main(int argc, char *argv[]) 
{
	//buffer will hold the user's command inputs
	char * buffer = malloc(BUFFER_SIZE+1);

	//parsedBuffer size should be able to hold a line of 180 bytes
	//if those bytes are separated by spaces, then you get 90 valid bytes
	//and we need 1 extra byte for null terminator
	//char * parsedBuffer[(BUFFER_SIZE/2) +1];
	char ** parsedBuffer = malloc( ((BUFFER_SIZE/2)+1)  * sizeof(char*));
	char * strExit = "exit"; //used for string comparisons
	int keepRunning = 1; //keeps loop running for user input
	char * returnFgets; //used for checking if fgets returned a null
	int forkResult; //used to tell which process is the child/parent
	char * defaultPrompt = "> ";
	int execvpReturn = 0;
	pid_t childReturnPID;

	//keep looping so shell continues taking inputs from user
	while(keepRunning){
		
		//setting custom prompt for shell
		if(NULL != argv[1]){
			returnFgets = getInputFromUser(buffer, argv[1]);
		}
		else{
			returnFgets = getInputFromUser(buffer, defaultPrompt);
		}
		
		//if fgets() returns null we either have end of file or an error
		//we then check which one it is and stop looping
		if (NULL == returnFgets){

			//this can be forcefully activated when you input ctrl + d into the command line
			if(feof(stdin)){
				//printf("End of file! Exiting Gracefullly.\n");
			}

			if(ferror(stdin)){
				perror("Error! Input error for fgets():\n");
			}

			keepRunning = 0;
			break;
		}

		//if user inputs "exit", stop looping for input
		if(	strncasecmp(strExit, buffer, 4)	== 0){
			//printf("User has input \"Exit\", program will exit gracefully.\n");
			keepRunning = 0;
			break;
		}
		//if user input did not return null and was not "exit", parse it
		else
		{
			tokenizeStoreString(buffer, parsedBuffer);
			//printArray(parsedBuffer); //function used to check if items are stored properly

			//fork() to create a new process so our current process does not 
			forkResult = fork();
		}
		
		switch (forkResult)
		{
			case -1:
				printf("ERROR! Switch statement error -1, something went wrong with result from fork().\n");
				break;

			//case 0 is always returned within the child
			case 0:
				//printf("Child process activated, PID: %d.\n",getpid());

				//exec takes over the child process! so it does not continue to the next line
				execvp(parsedBuffer[0], parsedBuffer);
			break;
			
			//default case is for any PID greater than 0 
			//because for the parent fork() returns the pid of the child
			default: //printf("I am the parent, my PID: %d, and my PPID is: %d.\n", getpid(), getppid());
				//printf("Parent process activated with PID: %d. Parent will begin waiting for child to die.\n", getpid());
				childReturnPID = wait(&execvpReturn); //the parent needs to wait until the child finishes

				
				//this handles if the user input something that was not valid for executing
				if(childReturnPID == -1){
					perror("Error on execvp:");
				}
				//default print message of child process completion
				else{
					printf("Child %d, exited with %d\n", childReturnPID, execvpReturn);
				}
				/*
				//should trigger when execvpReturn returns 0
				if(	WIFEXITED(*execvpReturn) != 0	){
					printf("execvpReturn returned 0.\n");
					
				}*/

				
				
			break;
		}

	}
	//printf("Parent with PID: %d has stopped waiting for child to die.\n", getpid());

	//for every malloc there is a free
	free(buffer);
	free(parsedBuffer);
	return 0;
}

//fills the parameter "buffer" with user input string
char * getInputFromUser(char * buffer, char * promp){
	char * returnFgets;

	printf("%s", promp);

	returnFgets = fgets(buffer, BUFFER_SIZE, stdin);

	//gets rid of the "\n" at the end of the string with a null terminator
	buffer[strlen(buffer) - 1] = '\0';

	return returnFgets;
}

void tokenizeStoreString(char * string, char * parsedStrings[]){
	//printf("tokenizer activated!\n");

	//used to place a null pointer after the last tokenized string
	int count = 0;

	//tokenized part of string that is separated by spaces
	char * delimitedInput;

	//used to store the address of the user input not tokenized yet
	char * restOfInput;

	restOfInput = string;

	//printf("input broken down by space:\n");

	while(delimitedInput = strtok_r(restOfInput, " ", &restOfInput)){
		//printf("Tokenized input is: %s \n", delimitedInput);
		parsedStrings[count] = delimitedInput;
		count++;
	}
	
	//after tokenizing input, after the last substring set it to a null ptr
	parsedStrings[count] = NULL;

	//printf("tokenizer finished!\n");
	return;
}

//prints all strings within an array of strings
void printArray(char * stringArray[]){
	printf("printArray activated\n");

	int count = 0;

	while(NULL != stringArray[count]){
		printf("%s\n",stringArray[count]);
		count++;
		if(NULL == stringArray[count]){
			printf("inside printArray() while loop. count: %d\n", count);
		}
	}

	printf("printArray finished\n");
	return;
}


	
