// process.c

#include "process.h"

// Define process table
struct task_struct process_table[PROC_MAX] = {0};

// Define current var
struct task_struct *current = &process_table[0];
