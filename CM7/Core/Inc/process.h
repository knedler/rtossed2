// process.h
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include "stm32h7xx_hal.h"
#include "sys/types.h"

#define PROC_MAX (4) // Max processes

#define STATE_UNUSED		(0b000000)
#define STATE_TIME_SLEEP	(0b000010)
#define STATE_IO_SLEEP		(0b000100)
#define STATE_RUN		(0b001000)
#define STATE_STOP		(0b010000)
#define STATE_ZOMBIE		(0b100000)

extern volatile uint8_t kready;

struct __attribute__((packed)) saved_registers {
	uint32_t R0;
	uint32_t R1;
	uint32_t R2;
	uint32_t R3;
	uint32_t R4;
	uint32_t R5;
	uint32_t R6;
	uint32_t R7;
	uint32_t R8;
	uint32_t R9;
	uint32_t R10;
	uint32_t R11;
	uint32_t R12;
	uint32_t SP;
	uint32_t LR;
	uint32_t PC;
	uint32_t xPSR;
};

struct __attribute__((packed)) task_struct {
	uint8_t state;			// Process state
	pid_t pid;			// Unique id
	uint32_t exc_return;		// Special prog counter
	uint32_t sp_start;		// Starting stack pointer addr
	int (*cmd)(void);		// Ptr to function
	struct saved_registers r;	// Saved registers struct
};

extern struct task_struct process_table[PROC_MAX];

extern struct task_struct *current;

extern struct task_struct *const idle_task;

static inline void yield(void)
{
	// Set PENDSV bit
	SCB->ICSR |= SCB_ICSR_PENDSVSET_Msk;

	// Data sync barrier
	__DSB();

	// Instruction sync barrier
	__ISB();
}

#endif // __PROCESS_H__
