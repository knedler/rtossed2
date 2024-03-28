// locking.h
#ifndef LOCKING_H
#define LOCKING_H

#include <stdint.h>

extern volatile uint64_t counter __attribute__((section(".axi")));

#endif				// LOCKING_H
