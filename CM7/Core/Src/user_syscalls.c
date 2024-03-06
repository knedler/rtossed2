// user_syscalls.c

#include "user_syscalls.h"
#include "process.h"

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
