#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <time.h>
#include "debug_printing.h"

/**
 * @brief Struct containng Debug datatype info
 * 
 */
struct _Debug
{
    char logFilePath[MAX_FILE_PATH_LENGTH]; /*!<*/
    FILE *logFile;
};

Debug* globalDebug;/*!< Debug global variable so debug_log doesnt require any Debug* type parameter*/


Debug* debug_create(char* debugFilePath, short setAsGlobal)
{
    Debug *debug = NULL;
    if(debugFilePath == NULL)
        return NULL;

    debug = (Debug *)malloc(sizeof(Debug));
    if(debug == NULL)
        return NULL;

    strcpy(debug->logFilePath, debugFilePath);

    debug->logFile = fopen(debug->logFilePath, "w");
    if(!debug->logFile){
        free(debug);
        return NULL;
    }

    if(setAsGlobal)
        globalDebug = debug;

    return debug;
}

void debug_destroy(Debug * info){
    if(info)
    {
        if(info->logFile){
            fclose(info->logFile);
        }
        free(info);
    }
}

/* GNU printf code: 
https://sourceware.org/git/?p=glibc.git;a=blob;f=stdio-common/printf.c;h=4c8f3a2a0c38ab27a2eed4d2ff3b804980aa8f9f;hb=3321010338384ecdc6633a8b032bb0ed6aa9b19a */
int debug_log(LOG_LEVEL level, char *formatedString, ...)
{
    va_list arg;
    int done;

    time_t rawtime;
    struct tm * timeinfo;

    char *finalString;

    time(&rawtime);
    timeinfo = localtime(&rawtime);


    finalString = (char*)malloc((15 + strlen(asctime(timeinfo)))*sizeof(char));
    if(!finalString)
        return -1;

    finalString[0] = '[';
    finalString[1] = 0;

    strcat(finalString, asctime(timeinfo));
    finalString[strlen(finalString)-1] = 0;/*Quita el \n final de asctime()*/
    strcat(finalString, "]");

    switch (level)
    {
    case PRINT:
        strcat(finalString, "LOG: ");
        break;
    case LOG_WARNING:
        strcat(finalString, "WARNING: ");
        break;
    case LOG_ERROR:
        strcat(finalString, "ERROR: ");
        break;
    case DEBUG:
        strcat(finalString, "DEBUG_LOG: ");
        break;
    default:
        break;
    }

    done = fprintf((globalDebug->logFile), "%s", finalString);
    if(!done){
        free(finalString);
        return -1;
    }

    va_start(arg, formatedString);
    done = vfprintf(globalDebug->logFile, formatedString, arg);
    va_end(arg);
    if(!done){
        free(finalString);
        return -1;
    }
    done = fprintf((globalDebug->logFile), "\n");
    if(!done){
        free(finalString);
        return -1;
    }
    free(finalString);
    return 1;
}

char *debug_getPath(Debug *info){
    return info->logFilePath;
}