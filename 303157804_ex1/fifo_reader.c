/*
 Created By: Matan Gizunterman
 TAU Username: gizunterman
 ID number: 303157804
 Most of the code was with the help of Recitation code, StackOverflow Issues, and Tutorials Point C Programming Guide
 */

#include <time.h> // for time measurement
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/mman.h>
#include <signal.h>
#include <fcntl.h>

//Macros Declarations - as asked in assignment
#define PATH "/tmp/osfifo"
#define PERMISSION 0600
#define BUFFER 2048

//Functions Declarations
void gracefullyExit(int fd, struct sigaction signal);
void safeClose(int fd);
void safeUnlink();
void restoreHandler(struct sigaction signal);

int main(int argc, char *argv[]) {
	int result=0;
	int cnt = 0;
	char arr[BUFFER];

	//File descriptor initialization
	int fd = -1;

	//ignored signals structure
	struct sigaction ignore;

	//original signals structure
	struct sigaction original;

	//Time measurement structures and variables (From assignment example)
	struct timeval t1, t2;
	double elapsed_microsec;

	// Register the ignored signal handler
	ignore.sa_handler = SIG_IGN;
	ignore.sa_flags = 0;
	if (sigaction(SIGINT, &ignore, &original) < 0) {
		gracefullyExit(fd,original);
		printf("Signal registration failed. %s\n", strerror(errno));
		exit(-1);
	}

	//Open '/tmp/osfifo' for reading with sleeping stabilization time
	//attempting to open 5 times before give up...
	int retry=0;
	while(fd<0){
		fd = open(PATH, 0600);
		if (fd < 0) {
			retry++;
		}
		if(retry==5){
			printf( "Error opening fifo : %s\n",strerror(errno));
			gracefullyExit(fd,original);
		exit(-1);
		}
		sleep(1);
	}

	//Time measurement START
	if (gettimeofday(&t1, NULL) < 0) {
		gracefullyExit(fd,original);
		printf("Error getting the current time of day : %s\n", strerror(errno));
		exit(-1);
	}

	//Read data and count the number of 'a' bytes read
	do{
		result = read(fd, arr,BUFFER);
		//At any point - exit in case of reading error
		if(result < 0){
			gracefullyExit(fd,original);
			printf( "Error reading from file : %s\n",strerror(errno));
			exit(-1);
		}
		//Counting number of 'a'
		int i;
		for(i=0;i<result;i++){
			if(arr[i]=='a')
				cnt++;
		}
	}
	while(result!=0); //While there are still bytes to read


	//Time measurement END
	if (gettimeofday(&t2, NULL) < 0 ) {
		gracefullyExit(fd,original);
		printf("Error getting the current time of day : %s\n", strerror(errno));
		exit(-1);
	}

	//Calculating and printing the result if the time measurement
	elapsed_microsec = (t2.tv_sec - t1.tv_sec) * 1000.0;
	elapsed_microsec += (t2.tv_usec - t1.tv_usec) / 1000.0;

	//convert the time result to milliseconds as asked
	elapsed_microsec = elapsed_microsec/1000.0;
	printf("%d were read in %f milliseconds through FIFO\n", cnt, elapsed_microsec);

	//Cleanup
	close(fd);
	exit(0);
}

void gracefullyExit(int fd,struct sigaction signal){
	restoreHandler(signal);
	safeUnlink();
	safeClose(fd);

}
void safeUnlink(){
	if(unlink(PATH)<0){
		printf( "Error unlink the file : %s\n",strerror(errno));
        	exit(-1);
	}
}

void safeClose(int fd){
	if(fd>=0){
		close(fd);
	}
}
void restoreHandler(struct sigaction signal){
	if (sigaction(SIGINT, &signal, NULL) < 0) {
		printf("Signal registration failed. %s\n", strerror(errno));
		exit(-1);
	}
}
