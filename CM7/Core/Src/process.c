// process.c

#include "process.h"

#define IDLE_TASK (0)

// Define process table
struct task_struct process_table[PROC_MAX] = {0};

// Define current var
struct task_struct *current = &process_table[IDLE_TASK];

// Define _eustack
extern const uint32_t _eustack[];
