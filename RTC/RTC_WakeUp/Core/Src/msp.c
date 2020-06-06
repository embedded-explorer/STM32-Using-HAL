#include "main.h"

extern void Error_handler(void);

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

// Low Level Peripheral Initialization USART2
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
	GPIO_InitTypeDef gpio_uart;

	// 1. Enable clock for USART2
	__HAL_RCC_USART2_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();

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
	HAL_NVIC_EnableIRQ(USART2_IRQn);
	HAL_NVIC_SetPriority(USART2_IRQn, 15, 0);
}

void HAL_RTC_MspInit(RTC_HandleTypeDef* hrtc)
{
	// Enable Clock directly from LSE

	// 1. Turn on LSE
	RCC_OscInitTypeDef rtc_osc;
	RCC_PeriphCLKInitTypeDef rtc_clk;

	rtc_osc.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	rtc_osc.LSEState = RCC_LSE_ON;
	if(HAL_RCC_OscConfig(&rtc_osc) != HAL_OK)
	{
		Error_handler();
	}

	// 2. Select LSE as RTC clock
	rtc_clk.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	rtc_clk.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if(HAL_RCCEx_PeriphCLKConfig(&rtc_clk) != HAL_OK)
	{
		Error_handler();
	}

	// 3. Enable RTC clock
	__HAL_RCC_RTC_ENABLE();

	// 4. Enable Interrupts
	HAL_NVIC_SetPriority(RTC_Alarm_IRQn, 15, 0);
	HAL_NVIC_EnableIRQ(RTC_Alarm_IRQn);
}
