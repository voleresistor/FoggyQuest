/* dq_log.c */

#include <stdio.h>
#include <time.h>
#include <string.h>

#include "log.h"

void log_open_file(char name[55])
{
    char log_path[255];

    strcpy(log_path, log_root);
    strcat(log_path, name);
    log_ptr = fopen(log_path, "a");
}

void log_write_log(char message[255], char component[20], int log_level)
{
    char log_name[55];
    char timestamp[8];
    char log_lvl[7];
    char log_str[512];
    struct tm* info;

    log_new_name(log_name);
    log_open_file(log_name);
    log_get_localtime();

    /* Generate message timestamp */
    sprintf(timestamp, "%02d:%02d:%02d", info->tm_hour + 1, info->tm_min + 1, info->tm_sec + 1);

    switch(log_level)
    {
        case 1:
        strcpy(log_lvl, "DQINFO");
        break;

        case 2:
        strcpy(log_lvl, "DQEROR");
        break;

        case 3:
        strcpy(log_lvl, "DQDBUG");
        break;
    }

    /* Write message to log file */
    fprintf(log_ptr, "[%s][%s]-> %s: %s\n", timestamp, component, log_lvl, message);
    fclose(log_ptr);
}

void log_new_name(char name[55])
{
    struct tm* info =  log_get_localtime();
    int yr = info->tm_year + 1900;

    sprintf(name, "DQ_Log_%d-%02d-%d.log", yr, info->tm_mon + 1, info->tm_mday);
    // printf("%s", name);
}

struct tm* log_get_localtime(void)
{
    time_t rawtime;
    time(&rawtime);
    return localtime(&rawtime);
}