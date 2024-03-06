// pdblink.c

#include "pdblink.h"
#include "gpio.h"
#include "user_syscalls.h"

int pdblink(void)
{
	while (1) {
		HAL_GPIO_TogglePin(D3_GPIO_Port, D3_Pin);
		
		// Sleep 50 ms
		microsleep(50000);
	}
}
