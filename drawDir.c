#include "FSreport.h"

int main(int argc, char *argv[])
{
    char *fileDir = argv[1];
    if (argc != 2)
    {
        printf("Correct Fromat Choose Either:\n");
        printf("1- ./drawDir directory > webpage.html\n");
        return 1;
    }
    char cwd[PATH_MAX];
    char *string = malloc(1000);
    if (getcwd(cwd, sizeof(cwd)) != NULL)
    {
        int index = getIndex(cwd, '/');
        char *stripped = strip(cwd, "/", index);
        if (strcmp(stripped, fileDir) != 0)
        {
            strcat(cwd, "/");
            strcat(cwd, fileDir);
        }
        char label = 'A';
        char *firstHtml = calloc(1000, sizeof(char));
        strcpy(firstHtml, "<!DOCTYPE html>\n<html lang=\"en\">\n<head>\n<meta charset=\"utf-8\">\n</head>\n<body>\n<center>\n<div class=\"mermaid\">\n");
        printf("%s", firstHtml);
        printf("graph TD\n");
        printf("A[%s]\n", fileDir);
        tree2(cwd, &label, &string);
        printf("%s", string);
        char *secondHtml = calloc(1000, sizeof(char));
        strcpy(secondHtml, "</div>\n</center>\n<script src=\"mermaid8.9.1.min.js\"></script>\n<script>mermaid.initialize({startOnLoad:true});</script>\n</body>\n</html>\n");
        printf("%s", secondHtml);
    }
    else
    {
        perror("error getting the directory");
        return 1;
    }
}
int getIndex(char *string, char c)
{
    for (int i = strlen(string); i >= 0; i--)
    {
        if (string[i] == c)
        {
            return i;
        }
    }
    return -1;
}
char *strip(char *string, char *toRemove, int index)
{
    char *stripped = calloc(strlen(string) + 1, sizeof(char));
    for (int i = index; i <= strlen(string) - 1; i++)
    {
        stripped[i - index - 1] = string[i];
    }
    return stripped;
}
char treeDirectories2(char *fileDir, char *label, char **string)
{
    Chart chart;
    struct stat fileinfo;
    int n;
    int i = 0;
    char *absolutePath = malloc(sizeof(char) * 1000);

    strcpy(absolutePath, fileDir);
    n = scandir(fileDir, &chart.namelist, NULL, alphasort);
    char currentLabel = *label;
    int c = 0;
    int number = 0;
    char nextLetter;
    char *temp = malloc(sizeof(char) * 10000);
    if (n < 0)
        perror("scandir");

    else
    {
        while (i < n)
        {
            absolutePath = getAbsolutePath(absolutePath, fileDir, chart.namelist, fileinfo, i);

            if (!stat(absolutePath, &fileinfo))
            {
                if (strcmp(chart.namelist[i]->d_name, ".") != 0 && strcmp(chart.namelist[i]->d_name, "..") != 0)
                {
                    if (S_ISDIR(fileinfo.st_mode))
                    {
                        nextLetter = currentLabel + 1;

                        if (*label != 'A')
                        {
                            currentLabel = *label;
                            printf("%c%d", currentLabel, number);
                            sprintf(temp, "%c --- %c%d\n", *label, currentLabel, number);
                        }
                        else
                        {
                            currentLabel = nextLetter;
                            printf("%c", currentLabel);
                            sprintf(temp, "%c --- %c\n", *label, currentLabel);
                        }
                        printf("[%s]\n", chart.namelist[i]->d_name);
                    }
                    number = number + 1;
                    c = c + 1;
                    if (S_ISDIR((fileinfo).st_mode))
                    {

                        // sprintf(temp,"%c --- %c\n",*label,currentLabel);
                        strcat(*string, temp);
                        tree2(absolutePath, &currentLabel, string);
                    }
                }
            }
            ++i;
        }
        free(chart.namelist);
    }
    return currentLabel;
}

void tree2(char *fileDir, char *label, char **string)
{
    Chart chart;

    struct stat fileinfo;
    int n;
    int i = 0;
    char *absolutePath = malloc(sizeof(char) * 1000);
    strcpy(absolutePath, fileDir);
    n = scandir(fileDir, &chart.namelist, NULL, alphasort);
    if (n < 0)
        perror("scandir");

    else
    {
        char current = treeDirectories2(fileDir, label, string);
        treeInfo2(&absolutePath, &fileDir, &fileinfo, &i, &n, label, &chart, &current, string);
        free(chart.namelist);
    }
}

void treeInfo2(char **absolutePath, char **fileDir, struct stat *fileinfo, int *i, int *n, char *level, Chart *chart, char *current, char **string)
{
    int number = 0;
    int check = 0;
    char levelCopy = *level;
    if (*level == 'A')
    {
        level = current;
        check = 1;
    }
    char currentLabel = *level;
    char *temp = malloc(sizeof(char) * 10000);
    while (*i < *n)
    {
        *absolutePath = getAbsolutePath(*absolutePath, *fileDir, (*chart).namelist, *fileinfo, *i);

        if (!stat(*absolutePath, fileinfo))
        {
            if (strcmp((*chart).namelist[*i]->d_name, ".") != 0 && strcmp((*chart).namelist[*i]->d_name, "..") != 0)
            {
                if (S_ISREG((*fileinfo).st_mode))
                {
                    char nextLetter = currentLabel + 1;
                    if (check == 0)
                    {
                        if (*level != 'A')
                        {
                            printf("%c%d", currentLabel, number);
                            sprintf(temp, "%c --- %c%d\n", *level, currentLabel, number);
                        }
                        else
                        {
                            currentLabel = nextLetter;
                            printf("%c", currentLabel);
                            sprintf(temp, "%c --- %c\n", *level, currentLabel);
                        }
                    }
                    if (check == 1)
                    {
                        if (*level != *current)
                        {
                            printf("%c%d", currentLabel, number);
                            sprintf(temp, "%c --- %c%d\n", levelCopy, currentLabel, number);
                        }
                        else
                        {
                            currentLabel = nextLetter;
                            printf("%c", currentLabel);
                            sprintf(temp, "%c --- %c\n", levelCopy, currentLabel);
                        }
                    }

                    printf("(%s)\n", (*chart).namelist[*i]->d_name);

                    strcat(*string, temp);
                }
                number = number + 1;
            }
        }
        *i = *i + 1;
    }
}
char *getAbsolutePath(char *absolutePath, char *fileDir, struct dirent **namelist, struct stat fileinfo, int i)
{
    strcpy(absolutePath, fileDir);
    if (fileDir[strlen(fileDir)] != '/')
    {
        absolutePath = strcat(absolutePath, "/");
    }
    absolutePath = strcat(absolutePath, namelist[i]->d_name);

    if ((stat(absolutePath, &fileinfo)) == -1)
    {
        printf("can't find %s\n", namelist[i]->d_name);
    }
    return absolutePath;
}
