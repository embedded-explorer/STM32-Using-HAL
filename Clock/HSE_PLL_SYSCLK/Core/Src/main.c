#include <string.h>
#include <stdio.h>
#include "main.h"

void UART2_Init(void);
void Error_handler(void);
void SystemClockConfig(uint8_t clkFreq);
void PrintClkFreq(void);

UART_HandleTypeDef huart2;

char message[100];
uint8_t FLatency = 0;

int main(void)
{
	HAL_Init();

	SystemClockConfig(SYSCLK_FREQ_84M);
	UART2_Init();
	PrintClkFreq();

	while(1);

	return 0;
}

void SystemClockConfig(uint8_t clkFreq)
{
	RCC_OscInitTypeDef osc_init;
	RCC_ClkInitTypeDef clk_init;

	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;

	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLCFGR_PLLSRC_HSE;

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

	switch(clkFreq)
	{
		case SYSCLK_FREQ_24M:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 192;
			osc_init.PLL.PLLP = RCC_PLLP_DIV8;

			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV1;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
			FLatency = FLASH_ACR_LATENCY_0WS;
			break;
		}
		case SYSCLK_FREQ_50M:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 100;
			osc_init.PLL.PLLP = RCC_PLLP_DIV2;

			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
			FLatency = FLASH_ACR_LATENCY_1WS;
			break;
		}
		case SYSCLK_FREQ_84M:
		{
			osc_init.PLL.PLLM = 8;
			osc_init.PLL.PLLN = 168;
			osc_init.PLL.PLLP = RCC_PLLP_DIV2;

			clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
			clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
			clk_init.APB2CLKDivider = RCC_HCLK_DIV1;
			FLatency = FLASH_ACR_LATENCY_2WS;
			break;
		}
		default:
			return;
	}
	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init, FLatency)!= HAL_OK)
	{
		Error_handler();
	}

	__HAL_RCC_HSI_DISABLE();

// Systick Configuration
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

// High Level Peripheral Initialization
void UART2_Init(void)
{
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 115200;
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

void Error_handler(void)
{
	while(1);
}

void PrintClkFreq(void)
{
	memset(message, 0, sizeof(message));
	sprintf(message, "SYSCLK = %ldHz\n\r", HAL_RCC_GetSysClockFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

	memset(message, 0, sizeof(message));
	sprintf(message, "HCLK = %ldHz\n\r", HAL_RCC_GetHCLKFreq());
	HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

	memset(message, 0, sizeof(message));
	sprintf(message, "PCLK1 = %ldHz\n\r", HAL_RCC_GetPCLK1Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

	memset(message, 0, sizeof(message));
	sprintf(message, "PCLK2 = %ldHz\n\r", HAL_RCC_GetPCLK2Freq());
	HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);
}

