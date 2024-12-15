#include "log.h"
#include <stdarg.h>
#include <time.h>

static FILE *log_file = NULL;

int init_log() {
    // Open the operations.log file in append mode
    log_file = fopen("operations.log", "a");
    if (!log_file) {
        return -1;
    }

    // session start header
    time_t now = time(NULL);
    fprintf(log_file, "\n--- Log session started at %s", ctime(&now));
    fflush(log_file);
    return 0;
}

void log_message(const char *fmt, ...) {
    if (!log_file) return; // If log not initialized, do nothing

    va_list args;
    va_start(args, fmt);

    // add a timestamp to each log entry
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    char time_str[20]; // "YYYY-MM-DD HH:MM:SS"
    strftime(time_str, sizeof(time_str), "%Y-%m-%d %H:%M:%S", tm_info);

    fprintf(log_file, "[%s] ", time_str);
    vfprintf(log_file, fmt, args);
    fprintf(log_file, "\n");
    fflush(log_file);

    va_end(args);
}

