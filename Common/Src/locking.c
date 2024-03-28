#include "locking.h"

volatile uint64_t counter __attribute__((section(".axi")));
