/* dq_log.c */

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "dq_log.h"
#include "dq_locator.h"

/*
    Public functions
*/
void new_log(void)
{
    /*
        Only init if not already done.
        
        This prevents overwriting an existing service.
    */
    if(system_log != NULL)
    {
        return;
    }

    system_log = malloc(sizeof(struct LogService));
    system_log->write_log = &write_log;
    system_log->init_log = &init_log;
    system_log->close_log = &close_log;

    /* Register with locator */
    system_locator->provide_log(system_log);
}

/*
    Service provider functions
*/
static void write_log(char s_message[255], const char s_component[30], int i_lvl)
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
}

static int init_log(char s_log_root[256], int i_lvl)
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

    log_s->g_log_lvl = i_lvl;

    log_open_file(log_s->log_path);

    /* Log initialization of log component */
    char log_msg[255];
    sprintf(log_msg, "========== Foggy Quest %s ==========", VERSION_STRING);
    write_log(log_msg, this_func, DQINFO);
    write_log("Logging initialized.", this_func, DQINFO);
    sprintf(log_msg, "Log file: %s", log_s->log_path);
    write_log(log_msg, this_func, DQINFO);

    return 0;
}

static void close_log(void)
{
    write_log("========== Log Closed ==========", this_func, DQINFO);
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
}

static void log_get_datestamp(char datestamp[11])
{
    time_t rawtime;
    struct tm* info;

    time(&rawtime);
    info = localtime(&rawtime);

    sprintf(datestamp, "%d-%02d-%02d", info->tm_year + 1900, info->tm_mon + 1, info->tm_mday);
}

static void log_get_timestamp(char timestamp[9])
{
    time_t rawtime;
    struct tm* info;

    time(&rawtime);
    info = localtime(&rawtime);

    sprintf(timestamp, "%02d:%02d:%02d", info->tm_hour, info->tm_min, info->tm_sec);
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
}