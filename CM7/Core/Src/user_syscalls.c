// user_syscalls.c

#include "user_syscalls.h"
#include "process.h"
#include <errno.h>

#define __NR_millisleep 2
#define __NR_kill 5

// Kill user space system call - does nothing in kernel space
int _kill(int pid, int sig)
{
	register uint32_t r0 asm("r0");	// IN: Syscall # - OUT: return value
	register uint32_t r1 asm("r1");	// IN: pid - OUT: errno
	register uint32_t r2 asm("r2");	// IN: sig
	int ret;		// Stage return value
	// Load registers and transition to kernel space
	__asm__ __volatile__("movs %0,%3\n\t"
			     "ldr %1,%4\n\t"
			     "ldr %2,%5\n\t"
			     "svc 42\n\t":"=r"(r0), "=r"(r1), "=r"(r2)
			     :"i"(__NR_kill), "m"(pid), "m"(sig)
			     :);
	__DSB();
	__ISB();
	// We do not need to wait for the PendSV bit to clear in this function
	ret = r0;		// Collect return value
	errno = r1;		// & errno from kernel
	return ret;
}

// millisleep user space kernel call
int _millisleep(int req)
{
	register uint32_t r0 asm("r0");
	register uint32_t r1 asm("r1");
	int ret;

	// Load registers and transition to kernel space
	__asm__ __volatile__("movs %0, %2\n\t"
			     "ldr %1, %3\n\t" "svc 42\n\t":"=r"(r0), "=r"(r1)
			     :"i"(__NR_millisleep), "m"(req)
			     :);

	__DSB();
	__ISB();

	while (((SCB->ICSR) & SCB_ICSR_PENDSVSET_Msk) ==
	       SCB_ICSR_PENDSVSET_Msk) ;

	ret = r0;		// Return value
	errno = r1;		// errno from kernel
	return ret;
}

int microsleep(uint32_t usec)
{
	// Get wakeup time after converting to micro sec 
	current->w_time = uwTick + (usec / 1000);

	// Change the state
	current->state &= ~STATE_RUN;
	current->state |= STATE_TIME_SLEEP;

	// Yield
	yield();

	return 0;
}
