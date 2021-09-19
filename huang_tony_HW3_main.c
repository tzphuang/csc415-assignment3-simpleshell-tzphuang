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

char * getInputFromUser(char * buffer, char * promp);
void tokenizeStoreString(char * string, char * parsedStrings[]);
void printArray(char * stringArray[]);


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
	

	//buffer will hold the user's command inputs
	char buffer[BUFFER_SIZE+1];
	//printf("Buffer size of buffer is %d \n", BUFFER_SIZE+1);

	//parsedBuffer size should be able to hold a line of 180 bytes
	//if those bytes are separated by spaces, then you get 90 valid bytes
	//and we need 1 extra byte for null terminator
	char * parsedBuffer[(BUFFER_SIZE/2) +1];
	//printf("Buffer size of buffer is %d \n", (BUFFER_SIZE/2)+1);

	//used for string comparisons
	char * strExit = "exit";
	//used for checking if fgets returned a null
	char * returnFgets;
	//used to store parts of the tokenized string
	char * delimitedInput;
	//used to store the address of the user input not tokenized yet
	char * restOfInput;
	//keeps track of strtok_r and used to place a NULL pointer at 
	//the end of the pseduoArgv called "parsedBuffer"
	int count;

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
		if(	strncasecmp(strExit, buffer, strlen(strExit))	== 0){
			printf("User has input \"Exit\", program will exit gracefully.\n");
			break;
		}
		else{
			printf("User input was: \"%s\"\n", buffer);
			restOfInput = buffer;

			printf("input broken down by space:\n");
			while(delimitedInput = strtok_r(restOfInput, " ", &restOfInput)){
				printf("%s \n", delimitedInput);
			}
		}
	}
	*/


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

	//keep looping so shell continues taking inputs from user
	while(keepRunning){
		forkResult = fork();
		switch (forkResult)
		{
		case -1:
			printf("ERROR! Switch statement error -1, something went wrong with result from fork().\n");
			break;

		//case 0 is always returned within the child
		case 0:
			printf("Child process activated, PID: %d.\n",getpid());

			if(NULL != argv){
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
					printf("End of file! Exiting Gracefullly.\n");
				}

				if(ferror(stdin)){
					perror("Error! Input error for fgets():\n");
				}

				keepRunning = 0;
				break;
			}

			//if user inputs "exit", stop looping for input
			if(	strncasecmp(strExit, buffer, strlen(strExit))	== 0){
			printf("User has input \"Exit\", program will exit gracefully.\n");
			keepRunning = 0;
			break;
			}
			//if user input did not return null and was not "exit", parse it
			else
			{
				tokenizeStoreString(buffer, parsedBuffer);
				printArray(parsedBuffer); //function used to check if items are stored properly
				
				if(-1 == execvp(parsedBuffer[0], parsedBuffer))
				{
					perror("Error on execvp:");
					exit(1);
				}
			}
			printf("Child process with PID: %d and parent PID %d will now die. \n", getpid(), getppid());

		break;
		
		//default case is for any PID greater than 0 
		//because for the parent fork() returns the pid of the child
		default: //printf("I am the parent, my PID: %d, and my PPID is: %d.\n", getpid(), getppid());
			printf("Parent process activated with PID: %d. Parent will begin waiting for child to die.\n", getpid());
			wait(NULL); //the parent needs to wait until the child finishes
			printf("Parent with PID: %d has stopped waiting for child to die.\n", getpid());
			keepRunning = 0;
			break;
		}

	}

	//for every malloc there is a free
	free(buffer);
	free(parsedBuffer);
	return 0;
}

//fills the parameter "buffer" with user input string
char * getInputFromUser(char * buffer, char * promp){
	char * returnFgets;

	printf("Please input your command line! %s ", promp);

	returnFgets = fgets(buffer, BUFFER_SIZE, stdin);

	//gets rid of the "\n" at the end of the string with a null terminator
	buffer[strlen(buffer) - 1] = '\0';

	return returnFgets;
}

void tokenizeStoreString(char * string, char * parsedStrings[]){
	printf("tokenizer activated!\n");

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

	printf("tokenizer finished!\n");
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


	
