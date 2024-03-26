// kernel_syscalls.h

#include "kernel_syscalls.h"
#include "process.h"

int sys_millisleep(uint32_t req)
{
	// Set wake time
	current->w_time = uwTick + req;

	// Change state
	current->state &= ~STATE_RUN;
	current->state |= STATE_TIME_SLEEP;

	// Yield
	yield();

	return 0;
}

int sys_kill(pid_t pid, int sig)
{
	return 0;
}
