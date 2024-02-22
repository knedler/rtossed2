// user_syscalls.c

#include "user_syscalls.h"
#include "process.h"

int microsleep(uint32_t usec)
{	
	// Get wakeup time after converting to micro sec 
	current->w_time = (uwTick * 1000) + usec;

	// Change the state
	current->state &= ~STATE_TIME_SLEEP;
	current->state |= STATE_TIME_SLEEP;

	return 0;
}
