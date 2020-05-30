#include "main.h"

// This function is called by HAL_Init
// Used to do low level processor specific Initializations
// Refer stm32f4xx_hal_cortex.c
void HAL_MspInit(void)
{
	// 1. Set up the priority grouping of the arm cortex mx processor
	HAL_NVIC_SetPriorityGrouping(NVIC_PRIORITYGROUP_4);

	// 2. Enable the required system exceptions of the arm cortex mx processor
	SCB->SHCSR |= (7 << 16);	// Enable Usage Fault, Bus Fault and Memory Fault

	// 3. Configure priority for system exceptions
	HAL_NVIC_SetPriority(MemoryManagement_IRQn, 0, 0);	// Priority for Memory management fault
	HAL_NVIC_SetPriority(BusFault_IRQn, 0, 0);			// Priority for Bus fault
	HAL_NVIC_SetPriority(UsageFault_IRQn, 0, 0);		// Priority for Usage fault
}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	// 1. Enable Clock for TIM10
	__HAL_RCC_TIM10_CLK_ENABLE();

	// 2. Enable IRQ for TIM10
	HAL_NVIC_EnableIRQ(TIM1_UP_TIM10_IRQn);

	// 3. Setup the Priority
	HAL_NVIC_SetPriority(TIM1_UP_TIM10_IRQn, 15, 0);
}

