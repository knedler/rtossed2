// pdblink.c

#include "pdblink.h"
#include "gpio.h"

int pdblink(void)
{
	while (1) {
		HAL_GPIO_TogglePin(D3_GPIO_Port, D3_Pin);

		HAL_Delay(50);
	}
}
