// process.h
#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdint.h>

#define TASK_IDLE process_table[0]

extern volatile uint8_t kready;

#endif // __PROCESS_H__
