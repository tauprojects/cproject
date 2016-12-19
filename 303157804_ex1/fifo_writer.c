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

//Error Messages Declarations
#define ERR_MSG_ARGS "Invalid Number Of Arguments - Expected: 2 , Actual: "
#define ERR_MSG_CREATE_FILE "Error While Creating File: "

//Macros Declarations - as asked in assignment
#define PERMISSION 0600
#define PATH "/tmp/osfifo"
#define BUFFER 2048

//Global Variables declarations:
//File descriptor initialization
int fd = -1;
//original signals structure
struct sigaction original;

//Byte Written counter initialization
int cnt = 0;

//Functions Declarations
void gracefullyExit(int fd,struct sigaction signal);
void safeClose(int fd);
void safeUnlink(int fd);
void pipeErrorHandler(int signum);
void restoreHandler(struct sigaction signal);

//Main function
int main(int argc, char** argv) {
	//Variables decelerations
	char arr[BUFFER];

	//Time measurement structures and variables (From assignment example)
	struct timeval t1, t2;
	double elapsed_microsec;

	//number of bytes to write argument
	int NUM = 0;

	int fd = -1;

	//Checking if number of arguments is valid
	if (argc < 2) {
		printf(ERR_MSG_ARGS);
		printf("%d\n", argc - 1);
		exit(-1);
	} else if (argc > 2) {
		printf(ERR_MSG_ARGS);
		printf("%d\n", argc - 1);
		exit(-1);
	}

	//Validate valid arguments value
	if (atoi(argv[1])) {
		NUM = atoi(argv[1]);
	} else {
		puts("Invalid Argument");
		return -1;
	}

	//ignored signals structure
	struct sigaction ignore;



	//pipe handler
	struct sigaction pipe_action;

	pipe_action.sa_handler = pipeErrorHandler;
	pipe_action.sa_flags = 0;

	// Register the ignored signal handler
	ignore.sa_handler = SIG_IGN;
	ignore.sa_flags = 0;
	if (sigaction(SIGINT, &ignore, &original) < 0) {
		gracefullyExit(fd,original);
		printf("Signal registration failed. %s\n", strerror(errno));
		exit(-1);
	}

	// Register the SIGPIPE signal handler
	if (sigaction(SIGPIPE, &pipe_action, NULL) == -1) {
		gracefullyExit(fd,original);
		printf("Signal registration failed. %s\n", strerror(errno));
		exit(-1);
	}


	//Create named pipe file
	struct stat pipeStat;
	if (lstat(PATH, &pipeStat) == -1) {
		//If fifo file no exist - create it
		if (errno == ENOENT) {
			if (mkfifo(PATH, PERMISSION) == -1) {
				printf("fifo doesn't exist - Error while creating: %s\n", strerror(errno));
				exit(-1);
			}
		}
	} else {
		if (!S_ISFIFO(pipeStat.st_mode)) {
			//If file exists - unlink and trying to re-recreate
			safeUnlink(fd);
			if (mkfifo(PATH, PERMISSION) == -1) {
				printf("file exist - Error while creating: %s\n", strerror(errno));
				exit(-1);
			}
		}
	}

	fd = open(PATH, O_WRONLY, PERMISSION);
	if (fd < 0) {
		gracefullyExit(fd,original);
		printf("Error opening the file: %s\n", strerror(errno));
		exit(-1);
	}

	//Time measurement START
	if (gettimeofday(&t1, NULL) < 0) {
		gracefullyExit(fd,original);
		printf("Error getting the current time of day writer 1: %s\n",
				strerror(errno));
		exit(-1);
	}

	//Write NUM 'a' bytes to this named pipe file
	int i;
	for (i = 0; i < BUFFER; i++) {
		arr[i] = 'a';
	}

	//in case our buffer is smaller the NUM - we need to write iteratively and reducing the total size

	if (NUM > BUFFER) {
		int sum = NUM; //Sum of all bytes needed to be read
		int tempRead;
		for (sum = NUM; sum > BUFFER; sum = sum - BUFFER) {

			if ((tempRead = write(fd, arr, BUFFER)) < 0) {
				gracefullyExit(fd,original);
				printf("Error writing to file : %s\n", strerror(errno));
				exit(-1);
			}
			cnt += tempRead;     //Counting how many bytes written
		}

		if ((tempRead = write(fd, arr, sum)) < 0) {
			gracefullyExit(fd,original);
			printf("Error writing to file : %s\n", strerror(errno));
			exit(-1);
		}
		cnt += tempRead;     //Counting how many bytes written
	}
	//in case our buffer is larger or equal - we can just use write() function
	else if (BUFFER >= NUM) {
		if ((cnt = write(fd, arr, NUM)) < 0) {
			gracefullyExit(fd,original);
			printf("Error writing to file : %s\n", strerror(errno));
			exit(-1);
		}
	}
	//Time measurement END
	if (gettimeofday(&t2, NULL) < 0) {
		gracefullyExit(fd,original);
		printf("Error getting the current time of day writer 2 : %s\n",
				strerror(errno));
		exit(-1);
	}
	//Calculating and printing the result if the time measurement
	elapsed_microsec = (t2.tv_sec - t1.tv_sec) * 1000.0;
	elapsed_microsec += (t2.tv_usec - t1.tv_usec) / 1000.0;

	//convert the time result to milliseconds as asked
	elapsed_microsec = elapsed_microsec / 1000.0;

	printf("%d were written in %f milliseconds through FIFO\n", cnt,
			elapsed_microsec);
	gracefullyExit(fd,original);
	exit(0);
}

void pipeErrorHandler(int signum) {
	gracefullyExit(fd,original);
	printf("Error trying to write to closed pipe : %s\n", strerror(errno));
	printf("%d were written through FIFO\n", cnt);
	exit(-1);
}

void gracefullyExit(int fd,struct sigaction signal) {
	restoreHandler(signal);
	safeUnlink(fd);
	safeClose(fd);

}
void safeUnlink(int fd) {
	if (unlink(PATH) < 0) {
		safeClose(fd);
		printf("Error unlink the file : %s\n", strerror(errno));
		exit(-1);
	}
}

void safeClose(int fd) {
	if (fd >= 0) {
		close(fd);
	}
}
void restoreHandler(struct sigaction signal){
	if (sigaction(SIGINT, &signal, NULL) < 0) {
		printf("Signal registration failed. %s\n", strerror(errno));
		exit(-1);
	}
}

