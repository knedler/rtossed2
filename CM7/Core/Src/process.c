// process.c

#include <string.h>
#include "process.h"
#include "sh.h"

#define IDLE_TASK (0)
#define SHELL_TASK (1)

// Define process table
struct task_struct process_table[PROC_MAX] = {0};

// Define idle_task
struct task_struct *const idle_task = &process_table[0];

// Define current var
struct task_struct *current = idle_task;

// Define _eustack
extern const uint32_t _eustack[];

void init_process_stack(struct task_struct *task);
void init_process_table(void);
void process_start(void);

void init_process_stack(struct task_struct *task)
{
	uint32_t *sp = (uint32_t *) task->r.SP;

	// Set floating point regs to 0
	for (int i = 0; i < 17; i++) {
		*(sp -= 4) = 0;
	}

	// Add saved regs to stack
	*(sp -= 4) = task->r.xPSR;
	*(sp -= 4) = task->r.PC;
	*(sp -= 4) = task->r.LR;
	*(sp -= 4) = task->r.R12;
	*(sp -= 4) = task->r.R3;
	*(sp -= 4) = task->r.R2;
	*(sp -= 4) = task->r.R1;
	*(sp -= 4) = task->r.R0;

	// Update sp
	task->r.SP = (uint32_t)sp;
}

void init_process_table(void)
{
	// Set to zero
	memset(process_table, 0, sizeof(process_table));

	// Init Idle Task
	process_table[IDLE_TASK].state = STATE_STOP;
	process_table[IDLE_TASK].r.xPSR = 0x01000000;
	process_table[IDLE_TASK].exc_return = EXC_RETURN_THREAD_MSP_FPU;
	process_table[IDLE_TASK].pid = 0;

	// Init Shell Task
	process_table[SHELL_TASK].r.SP = (uint32_t)_eustack;
	process_table[SHELL_TASK].sp_start = (uint32_t)_eustack;
	process_table[SHELL_TASK].r.LR = 0;
	process_table[SHELL_TASK].r.PC = (uint32_t)&process_start;
	process_table[SHELL_TASK].r.xPSR = 0x01000000;
	process_table[SHELL_TASK].state = STATE_RUN;
	process_table[SHELL_TASK].cmd = (int(*)(void))&shell;
	process_table[SHELL_TASK].pid = 0;
}

void process_start(void)
{
	// Call function
	current->cmd();

	// Change state
	current->state = STATE_STOP;

	// Loop forever
	while(1);
}
