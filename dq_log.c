/* dq_log.c */

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "dq_log.h"

/*
    Public API functions
*/
void log_write_log(char s_message[255], const char s_component[30], int i_lvl)
{
    /* Discard message if it's higher level than global log level */
    if(i_lvl > log_s->g_log_lvl)
    {
        printf("%s() - Discarding message with level %d greater than global level %d.\n", this_func, i_lvl, log_s->g_log_lvl);
        return;
    }

    char log_msg[512];
    char s_timestamp[8];
    char s_level[8];

    log_get_timestamp(s_timestamp);
    log_get_levelstring(s_level, i_lvl);

    sprintf(log_msg, "%s[%s()] %s => %s\n", s_timestamp, s_component, s_level, s_message);
    fputs(log_msg, log_s->log_ptr);

    // printf("%s", log_msg);
}

int log_init(char s_log_root[256], int i_lvl)
{
    if(log_s != NULL)
    {
        return 1;
    }

    log_s = malloc(sizeof(struct LogInfo));

    char s_log_name[55];
    log_new_name(s_log_name);

    *strncpy(log_s->log_path, s_log_root, strlen(s_log_root));
    *strncat(log_s->log_path, s_log_name, strlen(s_log_name));
    // printf("%s() - path: %s\n", this_func, log_path);
    // printf("%d\n", strlen(log_path));

    log_s->g_log_lvl = i_lvl;

    log_open_file(log_s->log_path);

    /* Log initialization of log component */
    log_write_log("========== Foggy Quest v0.1a ==========", this_func, DQINFO);
    log_write_log("Logging initialized.", this_func, DQINFO);
    char log_msg[255];
    sprintf(log_msg, "Log file: %s", log_s->log_path);
    log_write_log(log_msg, this_func, DQINFO);

    return 0;
}

void log_close(void)
{
    log_write_log("========== Log Closed ==========", this_func, DQINFO);
    fclose(log_s->log_ptr);
    free(log_s);
    log_s = NULL;
}

/*
    Private functions
*/
static void log_open_file(char path[256])
{
    log_s->log_ptr = fopen(path, "a");
}

static void log_new_name(char name[55])
{
    char datestamp[11];
    log_get_datestamp(datestamp);
    strcpy(name, "DQ_Log_");
    *strncat(name, datestamp, strlen(datestamp));
    strcat(name, ".log");
    // printf("%s(): %s\n", this_func, name);
}

static void log_get_datestamp(char datestamp[11])
{
    time_t rawtime;
    struct tm* info;

    time(&rawtime);
    info = localtime(&rawtime);

    sprintf(datestamp, "%d-%02d-%02d", info->tm_year + 1900, info->tm_mon + 1, info->tm_mday);
    // printf("%s(): %s\n", this_func, datestamp);
}

static void log_get_timestamp(char timestamp[9])
{
    time_t rawtime;
    struct tm* info;

    time(&rawtime);
    info = localtime(&rawtime);

    sprintf(timestamp, "%02d:%02d:%02d", info->tm_hour, info->tm_min, info->tm_sec);
    // printf("%s(): %s\n", this_func, timestamp);
}

static void log_get_levelstring(char s_lvl[9], int i_lvl)
{
    switch(i_lvl)
    {
        case 1:
        strcpy(s_lvl, "INFO");
        break;

        case 2:
        strcpy(s_lvl, "WARNING");
        break;

        case 3:
        strcpy(s_lvl, "ERROR");
        break;

        case 4:
        strcpy(s_lvl, "DEBUG");
        break;
    }

    // printf("%s(): %s\n", this_func, s_lvl);
}