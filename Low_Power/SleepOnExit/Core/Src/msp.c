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
void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	// 1. Enable Clock for TIM2
	__HAL_RCC_TIM2_CLK_ENABLE();

	// 2. Enable IRQ for TIM10
	HAL_NVIC_EnableIRQ(TIM2_IRQn);

	// 3. Setup the Priority
	HAL_NVIC_SetPriority(TIM2_IRQn, 15, 0);
}

// Low Level Peripheral Initialization USART2
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;

	// 1. Enable clock for USART2
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// Disable clock during sleep mode
	__HAL_RCC_USART2_CLK_SLEEP_DISABLE();
	__HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();

	// 2. Configure UART1 Pins
	// USART1 PA2 -> TX, PA3 -> RX
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Alternate = GPIO_AF7_USART2;
	gpio_uart.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	// 3. Enable IRQ and set priority (NVIC Settings)
	HAL_NVIC_EnableIRQ(USART2_IRQn);	// Enable IRQ for USART2
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}
