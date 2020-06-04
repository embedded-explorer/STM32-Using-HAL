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

// Low Level Peripheral Initialization USART1
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;
	// 1. Enable clock for USART1 peripheral
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();


	// 2. Configure UART1 Pins
	// USART1 PA9 -> TX, PA10 -> RX
	gpio_uart.Pin = GPIO_PIN_9;
	gpio_uart.Mode = GPIO_MODE_AF_PP;
	gpio_uart.Pull = GPIO_PULLUP;
	gpio_uart.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	gpio_uart.Pin = GPIO_PIN_10;
	HAL_GPIO_Init(GPIOA, &gpio_uart);

	// 3. Enable IRQ and set priority (NVIC Settings)
	HAL_NVIC_EnableIRQ(USART1_IRQn);	// Enable IRQ for USART2
	HAL_NVIC_SetPriority(USART1_IRQn, 15, 0);
}


// Low Level Peripheral Initialization of CAN1
void HAL_CAN_MspInit(CAN_HandleTypeDef *hcan)
{
	GPIO_InitTypeDef gpio_can;

	// 1. Enable clock for CAN peripheral
	__HAL_RCC_CAN1_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();


	// 2. Configure CAN1 Pins
	// CAN1 PA11 -> CAN_RX, PA12 -> CAN_TX
	gpio_can.Pin = GPIO_PIN_11;
	gpio_can.Mode = GPIO_MODE_AF_PP;
	gpio_can.Pull = GPIO_NOPULL;
	gpio_can.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &gpio_can);

	gpio_can.Pin = GPIO_PIN_12;
	HAL_GPIO_Init(GPIOA, &gpio_can);

	HAL_NVIC_SetPriority(USB_HP_CAN1_TX_IRQn, 15, 0);
	HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 15, 0);
	HAL_NVIC_SetPriority(CAN1_SCE_IRQn, 15, 0);

	HAL_NVIC_EnableIRQ(USB_HP_CAN1_TX_IRQn);
	HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
	HAL_NVIC_EnableIRQ(CAN1_SCE_IRQn);
}

