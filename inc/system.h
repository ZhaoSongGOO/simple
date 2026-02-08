#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdint.h>

// System Call Start
typedef enum { SYS_PRINT = 1 } SysCall_t;

typedef enum { INFO = 0, DEBUG = 1, WARN = 2, ERROR = 3 } LOG_LEVEL;

void printk(const char *str);
void printk_new_line(const char *str);
void log(const char *str, LOG_LEVEL level);

typedef struct {
  void (*i)(const char *msg);
  void (*d)(const char *msg);
  void (*e)(const char *msg);
  void (*w)(const char *msg);
} Log_Typedef;

extern Log_Typedef Log;

void SystemInit(void);
uint32_t SysTick_Config(uint32_t ticks);
void Delay_ms(uint32_t ms);
void Trigger_SVC(SysCall_t sc_id);
void Trigger_SVC_Print();
#endif