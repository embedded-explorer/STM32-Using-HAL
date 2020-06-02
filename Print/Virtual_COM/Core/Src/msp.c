#include "stm32f4xx_hal.h"

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

// Low Level Peripheral Initialization
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	// 1. Enable clock for USART2 peripheral
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

	// 2. Configure UART2 Pins
	// USART2 PA2 -> TX, PA3 -> RX, AF -> 07 (Virtual COM)
	gpio_uart.Pin = GPIO_PIN_2;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Alternate = GPIO_AF7_USART2;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_3;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	// 3. Enable IRQ and set priority (NVIC Settings)
	HAL_NVIC_EnableIRQ(USART2_IRQn);	// Enable IRQ for USART2
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}
