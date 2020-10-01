/* dq_log.h */

#include <stdio.h>

#ifndef DQ_LOG__
#define DQ_LOG__

#define VERSION_STRING "v0.1a"

struct tm;
struct Locator;

struct LogService {
    void (*write_log)(char message[255], const char component[20], int log_level);
    int (*init_log)(char s_log_root[200], int i_lvl);
    void (*close_log)(void);
};

struct LogInfo {
    int g_log_lvl;
    char log_path[256];
    FILE* log_ptr;
};

enum LOGLEVELS {
    DQINFO = 1,
    DQWARNING,
    DQERROR,
    DQDEBUG
};

// char log_msg_[255];

static struct LogInfo* log_s;
static struct LogService* system_log;

/* Public function */
void new_log(void);

/* Service functions */
static void write_log(char message[255], const char component[20], int log_level);
static int init_log(char s_log_root[200], int i_lvl);
static void close_log(void);

/* Private functions */
static void log_open_file(char name[256]);
static void log_new_name(char name[55]);
static void log_get_timestamp(char timestamp[8]);
static void log_get_datestamp(char datestamp[10]);
static void log_get_levelstring(char s_lvl[8], int i_lvl);

#endif