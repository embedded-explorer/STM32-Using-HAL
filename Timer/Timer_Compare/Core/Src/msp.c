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

// Low Level Initialization of Timer2
void HAL_TIM_OC_MspInit(TIM_HandleTypeDef *htim)
{
	GPIO_InitTypeDef tim2_gpio;

	// 1. Enable Clock for TIM4
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Configure Alternate Function
	tim2_gpio.Pin = GPIO_PIN_5;
	tim2_gpio.Mode = GPIO_MODE_AF_PP;
	tim2_gpio.Alternate = GPIO_AF1_TIM2;
	HAL_GPIO_Init(GPIOA, &tim2_gpio);

	// 2. Enable IRQ for TIM10
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	// 3. Setup the Priority
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
}
