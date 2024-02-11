// process.h
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>
#include "stm32h7xx_hal.h"

#define TASK_IDLE process_table[0]

#define PROC_MAX (4) // Max processes

#define STATE_UNUSED		(0)
#define STATE_TIME_SLEEP	(1)
#define STATE_IO_SLEEP		(2)
#define STATE_RUN		(3)
#define STATE_STOP		(4)
#define STATE_ZOMBIE		(5)

extern volatile uint8_t kready;

struct saved_registers {
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
	uint32_t LR;
	uint32_t PC;
	uint32_t xPSR;
} __attribute__((packed));

struct task_struct {
	uint8_t state;			// Process state
	uint16_t pid;			// Unique int
	uint32_t exc_return;		// Special prog counter
	uint32_t sp_start;		// Starting stack pointer addr
	int (*cmd)(void);		// Ptr to function
	struct saved_registers r;	// Saved registers struct
} __attribute__((packed));

extern struct task_struct process_table[PROC_MAX];

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
