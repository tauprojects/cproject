/*
 Created By: Matan Gizunterman
 TAU Username: gizunterman
 ID number: 303157804
 Most of the code was with the help of Recitation code, StackOverflow Issues, and Tutorials Point C Programming Guide
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h> // for time measurement
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>


//Error Messages Declarations
#define ERR_MSG_ARGS "Invalid Number Of Arguments - Expected: 2 , Actual: "
#define ERR_MSG_CREATE_FILE "Error While Creating File: "

//Macros Declarations - as asked in assignment
#define PATH "/tmp/mmapped.bin"
#define PERMISSION 0600

//Functions Declarations
void gracefullyExit(char* arr,int NUM,int fd); //Un-Mapping and closing File for gracefully shutdown
void safeUnMapping(char* arr,int NUM);
void safeClose(int fd);

//Main Function - Given 2 arguments
int main(int argc, char** argv) {
	//Variables decelerations
	char* arr;
	int result;

	//number of bytes to write argument
	int NUM=0;

	//PID number argument
	int RPID=0;

	//Time measurement structures and variables (From assignment example)
	double elapsed_microsec;
	struct timeval t1, t2;

	//Signals structures
	struct sigaction ignore;
	struct sigaction original;

	//Checking if number of arguments is valid
	if (argc < 3) {
		printf(ERR_MSG_ARGS);
		printf("%d\n", argc - 1);
		exit(-1);
	} else if (argc > 3) {
		printf(ERR_MSG_ARGS);
		printf("%d\n", argc - 1);
		exit(-1);
	}

	//Validate valid arguments value
	if (atoi(argv[1])) {
		 NUM = atoi(argv[1]);
	} else {
		printf("Invalid Argument Error: %s\n", strerror(errno));
		exit(-1);
	}
	if (atoi(argv[2])) {
		 RPID = atoi(argv[2]);
	} else {
		printf("Invalid Argument Error: %s\n", strerror(errno));
		exit(-1);
	}

	//Register signal handler
	ignore.sa_handler = SIG_IGN;
	ignore.sa_flags = 0;
	if(sigaction(SIGTERM,&ignore,&original) != -1)
		sigaction(SIGTERM,&original,NULL);
	else{
		printf( "Signal registration failed: %s\n",strerror(errno));
		exit(-1);
	}


	//Creating new File with given permission
	int fileDesc = creat(PATH,PERMISSION);
	if (fileDesc < 0) {
		puts(ERR_MSG_CREATE_FILE);
		printf("\n%s",PATH);
		exit(-1);
	}
	printf("File: %s Created Successfully\n", PATH); //Debug mode

	// Open a file for writing
	int fd = open(PATH, O_RDWR | O_CREAT);
	if (-1 == fd) {
		printf("Error opening file for writing: %s\n", strerror(errno));
		exit(-1);
	}

	// Force the file to be of the same size as the (mmapped) array
	result = lseek(fileDesc, NUM - 1, SEEK_SET);
	if (-1 == result) {
		close(fileDesc);
		printf("Error calling lseek() to 'stretch' the file: %s\n",
				strerror(errno));
		exit(-1);
	}

	// Something has to be written at the end of the file,
	// so the file actually has the new size.
	result = write(fileDesc, "", 1);
	if (1 != result) {
		close(fileDesc);
		printf("Error writing last byte of the file: %s\n", strerror(errno));
		exit(-1);
	}

	//Now the file is ready to be mmapped.
	arr = (char*) mmap(NULL,
	NUM, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (MAP_FAILED == arr) {
		gracefullyExit(NULL,0,fd);
		printf("Error mmapping the file: %s\n", strerror(errno));
		exit(-1);
	}

	//Time measurement START
	if(gettimeofday(&t1, NULL) <0){
		gracefullyExit(arr,NUM,fd);
		printf( "Error getting the current time of day : %s\n",strerror(errno));
		exit(-1);
	}

	// now write to the file as if it were memory
	int i;

	//Iterating over all of the array except the last char
	for (i = 0; i < NUM-1; ++i) {
		arr[i] = 'a';
	}

	//Last char of the array initialization
	arr[NUM-1]='\0';

	//Signal sending
	if(kill(RPID,SIGUSR1)<0){
		gracefullyExit(arr,NUM,fd);
		printf( "Error sending signal to the other process : %s\n",strerror(errno));
		exit(-1);
	}

	//Time measurement END
	if(gettimeofday(&t2, NULL) < 0){
		gracefullyExit(arr,NUM,fd);
		printf( "Error getting the current time of day : %s\n",strerror(errno));
		exit(-1);
	}

	//Calculating and printing the result if the time measurement
	elapsed_microsec = (t2.tv_sec - t1.tv_sec) * 1000.0;
	elapsed_microsec += (t2.tv_usec - t1.tv_usec) / 1000.0;

	//convert the time result to milliseconds as asked
	elapsed_microsec = elapsed_microsec/1000.0;

	printf("%d were written in %f milliseconds through MMAP\n", NUM, elapsed_microsec);

	//Exit
	gracefullyExit(arr,NUM,fd);
	exit(0);
}

void gracefullyExit(char* arr,  int NUM, int fd){
	safeClose(fd);
	safeUnMapping(arr,NUM);
}

void safeUnMapping(char* arr,int NUM){
	if((arr!=NULL) && (NUM !=0) && (munmap(arr,NUM) < 0)){
		printf( "Error un-mmaping the file : %s\n",strerror(errno));
		exit(-1);
	}
}
void safeClose(int fd){
	if(fd<0){
		close(fd);
	}
}









