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
	spLoggerDestroy();
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
	spLoggerDestroy();
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
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//Only Errors and Warning will be print
static bool basicLoggerWarningTest() {
	const char* expectedFile = "basicLoggerWarningTestExp.log";
	const char* testFile = "basicLoggerWarningTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//All messeges should be printed except Debug.
static bool basicLoggerInfoTest() {
	const char* expectedFile = "basicLoggerInfoTestExp.log";
	const char* testFile = "basicLoggerInfoTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintError("MSGA","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintWarning("MSGB","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintInfo("MSGC") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintDebug("MSGD","sp_logger_unit_test.c",__func__,__LINE__) == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(identicalFiles(testFile,expectedFile));
	return true;
}

//Will be printed at any level
static bool identicalLines(const char* testFile,char* line){
	int n = 20;
	char* tempLine = (char*)malloc(sizeof(char)*n);
	FILE *fp;
	fp = fopen(testFile, "r");
	tempLine = fgets(tempLine, n, fp);
	bool a = strcmp(tempLine,line);
	fclose(fp);
	free(tempLine);
	return a;
}
static bool basicLoggerMsgTest() {

//	const char* expectedFile = "basicLoggerMsgTestExp.log";
	const char* testFile = "basicLoggerMsgTest.log";
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg("Message ERROR Level") == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(identicalLines(testFile,"Message ERROR Level"));
	spLoggerDestroy();
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg("Message WAENING Level") == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg("Message INFO Level") == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
	ASSERT_TRUE(spLoggerCreate(testFile,SP_LOGGER_DEBUG_INFO_WARNING_ERROR_LEVEL) == SP_LOGGER_SUCCESS);
	ASSERT_TRUE(spLoggerPrintMsg("Message DEBUG Level") == SP_LOGGER_SUCCESS);
	spLoggerDestroy();
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

