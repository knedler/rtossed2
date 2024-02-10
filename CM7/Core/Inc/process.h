// process.h
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>

#define TASK_IDLE process_table[0]

extern volatile uint8_t kready;

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
