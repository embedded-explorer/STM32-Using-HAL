#include "main.h"

extern TIM_HandleTypeDef hTimer10;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void TIM1_UP_TIM10_IRQHandler()
{
	HAL_TIM_IRQHandler(&hTimer10);
}
