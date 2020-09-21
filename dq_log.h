/* dq_log.h */

struct tm;

#ifndef DQ_LOG__
#define DQ_LOG__

#define this_func __func__

enum LOGLEVELS {
    DQINFO = 1,
    DQWARNING,
    DQERROR,
    DQDEBUG
};

struct LogInfo {
    int g_log_lvl;
    char log_path[256];
    FILE* log_ptr;
};

static struct LogInfo* log_s;

/* Public functions */
extern void log_write_log(char message[255], const char component[20], int log_level);
extern int log_init(char s_log_root[200], int i_lvl);
extern void log_close(void);

/* Private functions */
static void log_open_file(char name[256]);
static void log_new_name(char name[55]);
static void log_get_timestamp(char timestamp[8]);
static void log_get_datestamp(char datestamp[10]);
static void log_get_levelstring(char s_lvl[8], int i_lvl);

#endif