// user_syscalls.c

#include "user_syscalls.h"
#include "process.h"

int microsleep(uint32_t msec)
{
	// Convert from ms to us
	uint32_t usec = msec * 1000;
	
	// Get current and wakeup time 
	current->w_time = uwTick + usec;

	// Change the state
	current->state &= ~STATE_TIME_SLEEP;
	current->state |= STATE_TIME_SLEEP;

	return 0;
}
