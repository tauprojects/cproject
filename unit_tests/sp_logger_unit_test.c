#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include "unit_test_util.h" //SUPPORTING MACROS ASSERT_TRUE/ASSERT_FALSE etc..
#include "../SPLogger.h"
#include <string.h>

//This is a helper function which checks if two files are identical
static bool identicalFiles(const char* fname1, const char* fname2) {
	FILE *fp1, *fp2;
	fp1 = fopen(fname1, "r");
	fp2 = fopen(fname2, "r");
	char ch1 = EOF, ch2 = EOF;

	if (fp1 == NULL) {
		return false;
	} else if (fp2 == NULL) {
		fclose(fp1);
		return false;
	} else {
		ch1 = getc(fp1);
		ch2 = getc(fp2);

		while ((ch1 != EOF) && (ch2 != EOF) && (ch1 == ch2)) {
			ch1 = getc(fp1);
			ch2 = getc(fp2);
		}
		fclose(fp1);
		fclose(fp2);
	}
	if (ch1 == ch2) {
		return true;
	} else {
		return false;
	}
}

//Logger is not defined
static bool basicLoggerTest() {
	ASSERT_TRUE(spLoggerPrintError("A","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_UNDIFINED);
	spLoggerDestroy();               //Logger destroy  and Memory Re-Allocation
	return true;
}

//Only Errors should be printed
static bool basicLoggerErrorTest() {
	const char* expectedFile = "basicLoggerErrorTestExp.log";
	const char* testFile = "basicLoggerErrorTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();               //Logger destroy  and Memory Re-Allocation
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//All messages should be printed in debug level
static bool basicLoggerDebugTest() {
	const char* expectedFile = "basicLoggerDebugTestExp.log";
	const char* testFile = "basicLoggerDebugTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();          //Logger destroy  and Memory Re-Allocation
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

/**
 * Warning Messages Unit-Test
 * Only Error and Warning Messages Should be printed
 * Compares and Asserts an expected message text file to the logger message text file
 * Use identicalFiles() helper function to compare files
 */
static bool basicLoggerWarningTest() {
//Defining messages text files references for comparing printed messages
	const char* expectedFile = "basicLoggerWarningTestExp.log";
	const char* testFile = "basicLoggerWarningTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();          //Logger destroy  and Memory Re-Allocation
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}
/**
 * Warning Messages Unit-Test
 * All Messages should be printed except Debug
 * Compares and Asserts an expected message text file to the logger message text file
 * Use identicalFiles() helper function to compare files
 */
static bool basicLoggerInfoTest() {
	//Defining messages text files references for comparing printed messages
	const char* expectedFile = "basicLoggerInfoTestExp.log";
	const char* testFile = "basicLoggerInfoTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();          //Logger destroy  and Memory Re-Allocation
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

/**
 * Warning Messages Unit-Test
 * Messages printed in all different levels
 * Compares and Asserts an expected message text file to the logger message text file
 * Assert that message s printed in every different level by using and modifying the same expected file
 * Use identicalFiles() helper function to compare files
 * Asserts Logger Messages in all different cases
 */

static bool basicLoggerMsgTest() {

	const char* expectedFile = "basicLoggerMsgTestExp.log";
	const char* testFile = "basicLoggerMsgTest.log";
	const char* msg = "Message ERROR Level";  //Expected msg string - will be modified.
	FILE *f;				         //Creates new file for the expectedFile modifying
	ASSERT_TRUE(spLoggerCreate("",SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_CANNOT_OPEN_FILE);
	f=fopen(expectedFile,"w");
	fprintf(f,"%s\n", msg);		//modifying expectedFile sith relevant message
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_DEFINED); //checks logger already define
	ASSERT_TRUE(spLoggerPrintMsg(msg) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	spLoggerDestroy();               //Logger destroy  and Memory Re-Allocation
	fclose(f);							 //Closing the file after asserting
	f=fopen(expectedFile,"w");
	msg = "Message WAENING Level";
	fprintf(f,"%s\n", msg);			  //modifying expectedFile with relevant message
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg(msg) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	spLoggerDestroy();               //Logger destroy  and Memory Re-Allocation
	fclose(f);
	f=fopen(expectedFile,"w");
	msg = "Message INFO Level";
	fprintf(f,"%s\n", msg);			  //modifying expectedFile with relevant message
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg(msg) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	spLoggerDestroy();               //Logger destroy  and Memory Re-Allocation
	fclose(f);
	f=fopen(expectedFile,"w");
	msg="Message DEBUG Level";
	fprintf(f,"%s\n", msg);			  //modifying expectedFile with relevant message
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg(msg) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();               //Logger destroy  and Memory Re-Allocation
	fclose(f);							 //closing file
	return true;
}

int main() {
	RUN_TEST(basicLoggerTest);
	RUN_TEST(basicLoggerErrorTest);
	RUN_TEST(basicLoggerDebugTest);
	RUN_TEST(basicLoggerWarningTest);
	RUN_TEST(basicLoggerInfoTest);
	RUN_TEST(basicLoggerMsgTest);
	return 0;
}

