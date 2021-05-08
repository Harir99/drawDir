#ifndef FSREPORT_H
#define FSREPORT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

typedef void (*ISFILE)(bool* check, char* string);
typedef void (*FUNCPTR)(char *fileDir, int level);
typedef char* (*FUNCPTR2)(char *fileDir, int level);

typedef struct Chart {
    char *alpha;
    struct dirent **namelist;
}Chart;


int getIndex(char *string, char c);
char *strip(char *string, char *toRemove,int index);
char treeDirectories2(char *fileDir,char* label,char** string);
void tree2(char *fileDir, char* label,char**string);
void treeInfo2(char **absolutePath, char **fileDir, struct stat *fileinfo, int *i, int *n, char* level, Chart* chart,char* current,char** string);
char *getAbsolutePath(char *absolutePath, char *fileDir, struct dirent **namelist, struct stat fileinfo, int i);


#endif
