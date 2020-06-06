#include <string.h>
#include <stdio.h>
#include "main.h"

void Error_handler(void);
void UART2_Init(void);
void SystemClockConfig(void);
void ButtonInit(void);
void LowPowerConfig(void);

UART_HandleTypeDef huart2;
GPIO_InitTypeDef gpioa;
GPIO_InitTypeDef gpioc;

char *message = "Hey you woke me up\r\n";

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	ButtonInit();
	LowPowerConfig();
	UART2_Init();

	while(1)
	{
		// Enter normal sleep using WFI
		__WFI();
	}

	return 0;
}

void Error_handler(void)
{
	while(1);
}

void ButtonInit(void)
{
	// User Button is connected to PC13

	gpioc.Pin = GPIO_PIN_13;
	gpioc.Mode = GPIO_MODE_IT_RISING;
	HAL_GPIO_Init(GPIOC, &gpioc);

	__HAL_RCC_GPIOC_CLK_ENABLE();

	// Enable Button Interrupt
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
}

void LowPowerConfig(void)
{
	// 1. Keep unused GPIOs in Analog mode
	gpioa.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | \
				GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | \
				GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
				GPIO_PIN_14 | GPIO_PIN_15;
	gpioa.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOA, &gpioa);


	gpioc.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | \
				GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_6 | GPIO_PIN_7 | \
				GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 | GPIO_PIN_11 | \
				GPIO_PIN_12 | GPIO_PIN_14 | GPIO_PIN_15;
	gpioc.Mode = GPIO_MODE_ANALOG;
	HAL_GPIO_Init(GPIOC, &gpioc);

	// 2. Disable clock for GPIOA in Sleep mode
	__HAL_RCC_GPIOA_CLK_SLEEP_DISABLE();
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}
}

void SystemClockConfig(void)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;

	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_HSE;
	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0WS) != HAL_OK)
	{
		Error_handler();
	}

	__HAL_RCC_HSI_DISABLE();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}



void UART2_Init(void){
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 460800;
	huart2.Init.WordLength = UART_WORDLENGTH_8B;
	huart2.Init.StopBits = UART_STOPBITS_1;
	huart2.Init.Parity = UART_PARITY_NONE;
	huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart2.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart2) != HAL_OK)
	{
		//  There is a problem
		Error_handler();
	}
}
