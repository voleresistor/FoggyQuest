/* dq_log.h */

struct tm;

#ifndef DQ_LOG__
#define DQ_LOG__

enum LOGLEVEL {
    DQINFO = 1,
    DQEROR,
    DQDBUG
};

char log_root[200];
FILE* log_ptr;

extern void log_open_file(char name[55]);
extern void log_write_log(char message[255], char component[20], int log_level);
extern void log_new_name(char name[55]);
extern struct tm* log_get_localtime(void);

#endif