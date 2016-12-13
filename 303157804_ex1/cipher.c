#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <string.h>


/*
Created By: Matan Gizunterman
TAU Username: gizunterman
ID number: 303157804
Most of the code was with the help of StackOverflow Issues, and Tutorials Point C Programming Guide
*/


//Constants Number and Error Messages Declarations

#define MAX_FILENAME 200
#define BUFFER_SIZE 2048

#define ERR_MSG_OPEN_FILE "Error While Opening File: "
#define ERR_MSG_READ_FILE "Error While Reading File: "
#define ERR_MSG_CLOSE_FILE "Error While Closing File: "
#define ERR_MSG_CREATE_FILE "Error While Creating File "

#define ERR_MSG_OPEN_DIR "Error While Opening Directory: "
#define ERR_MSG_READ_DIR "Error While Reading Directory: "
#define ERR_MSG_CLOSE_DIR "Error While Closing Directory: "
#define ERR_MSG_CREATE_DIR "Error While Creating Directory "

#define ERR_MSG_CREATE_DEST_DIR "Error While Creating Destination Dir: "
#define ERR_MSG_READING_KEY_BYTES "Error While Reading Key File Bytes"

#define ERR_MSG_ARGS "Invalid Number Of Arguments - Expected: 3 , Actual: "
#define CIPHER_MSG "Ciphering File: "



//Functions Declaration
int readKeyBytesToBuffer(char *keyBuffer, int inputBytesRead, int *pKeyFd,
		char *keyPath);
int xorFile(char *sourcePath, char *destinationPath, char *keyPath);

//Main Function
int main(int argc, char** argv) {

	//Checking if number of arguments is valid
	if (argc < 4) {
		printf(ERR_MSG_ARGS);
		printf("%d\n",argc-1);
		return -1;
	}
	else if (argc > 4) {
		printf(ERR_MSG_ARGS);
		printf("%d\n",argc-1);
		return -1;
}

  char *srcDirPath = argv[1];  //Source Files Directory Path Argument String
  char *keyFilePath = argv[2]; //Key File Path Argument String
  char *desDirPath = argv[3];  //Destination Files Directory Path Argument String
	DIR *dir;
	char inputFile[MAX_FILENAME];
	char outputFile[MAX_FILENAME];
	struct dirent *ent;

	struct stat st = {0};  //Used For Creating mkdir func

//   Checks if destination folder exist, otherwise create one
	if (stat(desDirPath, &st) == -1) {
		printf("Destination Directory:  %s Is Not Exist,\nCreating New Directory\n", desDirPath);
		mkdir(desDirPath, 0700);
	}
	if ((dir = opendir(srcDirPath)) != NULL) {
		if(dir<0){

			printf("Error While Opening Source Path: %s \n", srcDirPath);
			return -1;
		}

		while ((ent = readdir(dir)) != NULL) {
			sprintf(inputFile, "%s/%s", srcDirPath, ent->d_name);
			sprintf(outputFile, "%s/%s", desDirPath, ent->d_name);
//			printf(CIPHER_MSG);  //Used for debug
			printf("%s\n", ent->d_name);
			xorFile(inputFile, outputFile, keyFilePath);

		}
		if(closedir(dir)<0){
			printf(ERR_MSG_CLOSE_DIR);
			printf("%s \n", srcDirPath);
			return -1;
		}
	} else {
		// Error Opening Directory
		printf(ERR_MSG_OPEN_DIR);
		printf("%s\n", srcDirPath);
		return -1;
	}
}

int readKeyBytesToBuffer(char *keyBuffer, int inputBytesRead, int *pKeyFd,
		char *keyPath) {
	int i;
	for (i = 0; i < inputBytesRead; i++) {

		if ((read(*pKeyFd, &(keyBuffer[i]), 1) == 0)) {
			close(*pKeyFd);
			*pKeyFd = open(keyPath, O_RDWR);
			read(*pKeyFd, &(keyBuffer[i]), 1);

		}
	}
}
int xorFile(char *sourcePath, char *destinationPath, char *keyPath) {
	char inputBuffer[BUFFER_SIZE];
	char keyBuffer[BUFFER_SIZE];
	int bufferIndex = 0;
	int inputBytesRead, keyBytesRead;
	int srcFd, destFd, keyFd;
	int i;
	srcFd = open(sourcePath, O_RDWR);
	destFd = creat(destinationPath, S_IRUSR | S_IWUSR | S_IROTH);
	keyFd = open(keyPath, O_RDWR);
	if (srcFd == -1) {
		printf(ERR_MSG_OPEN_FILE);
		printf("%s\n", sourcePath);
		return -1;
	}

	if (destFd == -1) {
		printf(ERR_MSG_CREATE_FILE);
		printf("%s\n", destinationPath);
		return -1;
	}
	if (keyFd == -1) {
		printf(ERR_MSG_OPEN_FILE);
		printf("%s\n", keyPath);
		return -1;
	}
	while (1) {
		inputBytesRead = read(srcFd, inputBuffer, BUFFER_SIZE);
		if (inputBytesRead < 0) {
			printf(ERR_MSG_READING_KEY_BYTES);
				return -1;
			}
		keyBytesRead = readKeyBytesToBuffer(keyBuffer, inputBytesRead, &keyFd,
				keyPath);

		if (keyBytesRead != inputBytesRead) {
			printf(ERR_MSG_READING_KEY_BYTES);
			return -1;
		}

		for (i = 0; i < inputBytesRead; i++) {
			inputBuffer[i] = inputBuffer[i] ^ keyBuffer[i];
			//printf("%c", inputBuffer[i]);

		}
		write(destFd, inputBuffer, inputBytesRead);
		if (inputBytesRead < BUFFER_SIZE)
			break;
	}

	// Closing file And other directories
	if (close(keyFd) < 0) {
		printf(ERR_MSG_CLOSE_FILE);
		printf("Key");
		return -1;
	}
	if (close(srcFd) < 0) {
		printf(ERR_MSG_CLOSE_DIR);
		printf("Source");
		return -1;
	}
	if (close(destFd) < 0) {
		printf(ERR_MSG_CLOSE_DIR);
		printf("Destination");
		return -1;
	}

	return 1;
}


