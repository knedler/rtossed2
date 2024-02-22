// process.c

#include <string.h>
#include "process.h"
#include "sh.h"
#include "pdblink.h"

#define IDLE_TASK (0)
#define SHELL_TASK (1)
#define PDBLINK_TASK (2)

// Define process table
struct task_struct process_table[PROC_MAX] = { 0 };

// Define idle_task
struct task_struct *const idle_task = &process_table[IDLE_TASK];

// Define shell_task
struct task_struct *const shell_task = &process_table[SHELL_TASK];

// Define pdblink_task
struct task_struct *const pdblink_task = &process_table[PDBLINK_TASK];

// Define current var
struct task_struct *current = idle_task;

// Define _eustack
extern const uint32_t _eustack[];

// Define next_index
uint8_t next_index = 0;

void init_process_stack(struct task_struct *task);
void process_start(void);

void init_process_stack(struct task_struct *task)
{
	uint32_t *sp = (uint32_t *) task->r.SP;

	// Set floating point regs to 0
	for (int i = 0; i < 17; i++) {
		*(--sp) = 0;
	}

	// Add saved regs to stack
	*(--sp) = task->r.xPSR;
	*(--sp) = task->r.PC;
	*(--sp) = task->r.LR;
	*(--sp) = task->r.R12;
	*(--sp) = task->r.R3;
	*(--sp) = task->r.R2;
	*(--sp) = task->r.R1;
	*(--sp) = task->r.R0;

	// Update sp
	task->r.SP = (uint32_t) sp;
}

void init_process_table(void)
{
	// Set to zero
	memset(process_table, 0, sizeof(process_table));

	// Init Idle Task
	process_table[IDLE_TASK].state = STATE_RUN;
	process_table[IDLE_TASK].r.xPSR = 0x01000000;
	process_table[IDLE_TASK].exc_return = EXC_RETURN_THREAD_MSP_FPU;
	process_table[IDLE_TASK].pid = 0;

	// Init Shell Task
	process_table[SHELL_TASK].r.SP = (uint32_t) _eustack;
	process_table[SHELL_TASK].sp_start = (uint32_t) _eustack;
	process_table[SHELL_TASK].r.LR = 0;
	process_table[SHELL_TASK].r.PC = (uint32_t) & process_start;
	process_table[SHELL_TASK].r.xPSR = 0x01000000;
	process_table[SHELL_TASK].state = STATE_RUN;
	process_table[SHELL_TASK].cmd = (int (*)(void))&shell;
	process_table[SHELL_TASK].exc_return = EXC_RETURN_THREAD_PSP;
	process_table[SHELL_TASK].pid = 1;

	init_process_stack(&process_table[SHELL_TASK]);
	
	// Init Pdblink Task
	process_table[PDBLINK_TASK].r.SP = (uint32_t) (_eustack - 0x800);
	process_table[PDBLINK_TASK].sp_start = (uint32_t) (_eustack - 0x800);
	process_table[PDBLINK_TASK].r.LR = 0;
	process_table[PDBLINK_TASK].r.PC = (uint32_t) & process_start;
	process_table[PDBLINK_TASK].r.xPSR = 0x01000000;
	process_table[PDBLINK_TASK].state = STATE_RUN;
	process_table[PDBLINK_TASK].cmd = &pdblink;
	process_table[PDBLINK_TASK].exc_return = EXC_RETURN_THREAD_PSP;
	process_table[PDBLINK_TASK].pid = 2;

	init_process_stack(&process_table[PDBLINK_TASK]);
}

void process_start(void)
{
	// Call function
	current->cmd();

	// Change state
	current->state = STATE_STOP;

	// Loop forever
	while (1) ;
}

struct task_struct *schedule(void)
{
	if (current == idle_task) {
		return shell_task;
	} else if (current == shell_task) {
		return pdblink_task;
	} else if (current == pdblink_task) {
		return idle_task;
	}
	/*
	// Iterate through to find next runnable task
	for (uint8_t i = 0; i < PROC_MAX; i++) {
		next_index = (next_index + 1) % PROC_MAX;

		// Get next task struct
		struct task_struct *next = &process_table[next_index];

		// Check if sleeping and is time to wake
		if ((next->state & STATE_TIME_SLEEP) && (uwTick > next->w_time)) {
			next->state &= STATE_TIME_SLEEP;
			next->state |= STATE_RUN;
		}

		// Check if runnable
		if (next->state & STATE_RUN) {
			return next;
		}
	}
	*/
	// Return idle if no runnable task
	return idle_task;
}
