// process.c

#include "process.h"

// Define process table
struct task_struct process_table[PROC_MAX] = {0};

// Define idle_task
struct task_struct *const idle_task = &process_table[0];

// Define current var
struct task_struct *current = idle_task;

// Define _eustack
extern const uint32_t _eustack[];

void init_process_stack(struct task_struct *task)
{
	uint32_t *sp = (uint32_t *) task->sp_start;

	for (int i = 0; i < 17; i++) {
		*(--sp) = 0;
	}

	*(--sp) = task->r.xPSR;
	*(--sp) = task->r.PC;
	*(--sp) = task->r.LR;
	*(--sp) = task->r.R12;
	*(--sp) = task->r.R3;
	*(--sp) = task->r.R2;
	*(--sp) = task->r.R1;
	*(--sp) = task->r.R0;

	task->sp_start = (uint32_t)sp;
}
