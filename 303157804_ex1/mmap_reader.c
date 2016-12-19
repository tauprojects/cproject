/*
 Created By: Matan Gizunterman
 TAU Username: gizunterman
 ID number: 303157804
 Most of the code was with the help of Recitation code, StackOverflow Issues, and Tutorials Point C Programming Guide
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

//Macros Declarations - as asked in assignment
#define PATH "/tmp/mmapped.bin"

//Functions Declarations
void gracefullyExit(char* arr, int result,int fd,int linkFlag);
void safeClose(int fd);
void mmapSignalHandler(int signum);


//Global Struct Declaration to hold the signal handler defined before this program
struct sigaction original;

//Global Struct Declaration to hold the SIGINT signal handler in order to ignore it as asked.
struct sigaction ignore;

//Main function
int main (void){
  // Structure to pass to the registration syscall
  struct sigaction new_action;

  // Assign pointer to our handler function
  new_action.sa_handler = mmapSignalHandler;

  // Remove any special flag
  new_action.sa_flags = 0;

  // Register the new signal handler
  if (0 != sigaction (SIGUSR1, &new_action, &original)){
    printf("Signal handle registration failed. %s\n",strerror(errno));
    exit(-1);
  }
  // Assign pointer to ignored signal handler and removoe flags
  ignore.sa_handler = SIG_IGN;
  ignore.sa_flags = 0;

  // Register the ignored signal handler
  if(sigaction(SIGTERM, &ignore, &original)<0){
		printf("Signal handle registration failed. %s\n",strerror(errno));
		exit(-1);
	}
  if(sigaction(SIGTERM, &original, NULL)<0){
		printf("Signal handle registration failed. %s\n",strerror(errno));
		exit(-1);
  }
  // Meditate untill killed
  while(1)
  {
    sleep(2);
//    printf("Process runs.\n");
  }
  exit(0);
}


// Signal handler.
void mmapSignalHandler(int signum) {
	//Variables decelerations
	int fd, result=0;

	//Time measurement structures and variables (From assignment example)
	double elapsed_microsec;
	struct timeval t1, t2;

	fd = open(PATH, O_RDWR | O_CREAT);
	if (-1 == fd) {
		if(sigaction(SIGUSR1,&original,NULL)<0){
			printf("Signal handle registration failed. %s\n",strerror(errno));
			gracefullyExit(NULL,0,fd,0);
		}
		printf("Error opening file for writing: %s\n", strerror(errno));
		gracefullyExit(NULL,0,fd,0);

	}
	//Getting the file size
	if((result = lseek(fd, 0, SEEK_END))<0){
		printf( "Error getting the file size : %s\n",strerror(errno));
		gracefullyExit(NULL,0,fd,0);
	}

	//Time measurement START
	if(gettimeofday(&t1, NULL) <0){
		printf( "Error getting the current time of day : %s\n",strerror(errno));
		gracefullyExit(NULL,0,fd,0);
	}

	//Now the file is ready to be mmapped - using the size of the file
	char *arr = (char*) mmap(NULL, result, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	if (MAP_FAILED == arr) {
		printf("Error mmapping the file: %s\n", strerror(errno));
		gracefullyExit(NULL,0,fd,0);
	}

	//Counting the number of 'a'
	int cnt; //'a' counter
	int i;
	for(i=0,cnt=0;i<result-1;i++){
		if(arr[i] == 'a')
			cnt++;
	}
	//Checking if the last character is '\0'
	if(arr[i]!='\0'){
		printf("Error While Reading - No EOS Character found");
		gracefullyExit(arr,result,fd,0);
	}


	//Time measurement END
	if(gettimeofday(&t2, NULL)<0){
		printf( "Error getting the current time of day : %s\n",strerror(errno));
		gracefullyExit(arr,result,fd,0);
	}

	//Calculating and printing the result if the time measurement
	elapsed_microsec = (t2.tv_sec - t1.tv_sec) * 1000.0;
	elapsed_microsec += (t2.tv_usec - t1.tv_usec) / 1000.0;

	//convert the time result to milliseconds as asked
	elapsed_microsec = elapsed_microsec/1000.0;

	printf("%d were read in %f milliseconds through MMAP\n", i+1, elapsed_microsec);
	gracefullyExit(arr,result,fd,1);
}
void gracefullyExit(char* arr, int result, int fd, int linkFlag) {
	//Exit flag
	int flag = 0;

	//Unmapping when needed
	if ((arr != NULL) && (result != 0) && (munmap(arr, result) < 0)) {
		flag = -1;
		printf("Error un-mmaping the file : %s\n", strerror(errno));
	}
	//Link flag - need to free link
	if (linkFlag == 1) {
		if (unlink(PATH) < 0) {
			printf("Error unlink the file : %s\n", strerror(errno));
			flag = -1;
		}
	}
	//Restore original signal handler before exit
	if (sigaction(SIGUSR1, &original, NULL) < 0) {
		flag = -1;
		printf("Signal handle registration failed. %s\n", strerror(errno));
	}
	safeClose(fd);
	exit(flag);
}

void safeClose(int fd){
	if(fd<0){
		close(fd);
	}
}
