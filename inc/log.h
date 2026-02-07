#ifndef LOG_H
#define LOG_H

typedef enum { INFO = 0, DEBUG = 1, WARN = 2, ERROR = 3 } LOG_LEVEL;

void printk(const char *str);
void log(const char *str, LOG_LEVEL level);

typedef struct {
  void (*i)(const char *msg);
  void (*d)(const char *msg);
  void (*e)(const char *msg);
  void (*w)(const char *msg);
} Log_Typedef;

extern Log_Typedef Log;

void Log_Init();

#endif
