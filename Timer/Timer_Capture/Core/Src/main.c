#include <stdio.h>
#include <string.h>
#include "main.h"

void Error_handler(void);
void Timer10_Init(void);
void LSE_Configuration(void);
void UART2_Init(void);

UART_HandleTypeDef huart2;
TIM_HandleTypeDef hTimer10;

uint32_t input_capture[2] = {0, 0};
uint8_t count = 1;
volatile uint8_t capture_done = FALSE;

int main(void)
{
	uint32_t capture_difference = 0;
	double timer10_cnt_freq = 0;
	double timer10_cnt_res = 0;
	double signal_period = 0;
	double signal_freq = 0;
	char message[100];

	HAL_Init();
	LSE_Configuration();
	UART2_Init();
	Timer10_Init();

	// Start the timer
	HAL_TIM_IC_Start_IT(&hTimer10, TIM_CHANNEL_1);

	while(1)
	{
		if(capture_done)
		{
			if(input_capture[1] > input_capture[0])
				capture_difference = (input_capture[1] - input_capture[0]);
			else
				capture_difference = ((0xFFFF - input_capture[0]) + input_capture[1]);

			timer10_cnt_freq = ((HAL_RCC_GetPCLK2Freq() / (hTimer10.Init.Prescaler + 1)));
			timer10_cnt_res = (1 / timer10_cnt_freq);
			signal_period = (capture_difference * timer10_cnt_res);
			signal_freq = (1 / signal_period);

			sprintf(message, "Frequency of the Signal is: %f\r\n", signal_freq);

			if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
			{
				Error_handler();
			}
			capture_done = FALSE;
		}
	}

	return 0;
}

void Error_handler(void)
{
	while(1);
}

void Timer10_Init()
{
	TIM_IC_InitTypeDef timer10IC_Config;

	// Initialize Time Base Unit
	hTimer10.Instance = TIM10;
	hTimer10.Init.Prescaler = 0;
	hTimer10.Init.Period = 0xFFFF;
	hTimer10.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_IC_Init(&hTimer10) != HAL_OK)
	{
		Error_handler();
	}

	// Configure Input Channel of Timer
	timer10IC_Config.ICFilter = 0;
	timer10IC_Config.ICPolarity = TIM_ICPOLARITY_RISING;
	timer10IC_Config.ICPrescaler = TIM_ICPSC_DIV1;
	timer10IC_Config.ICSelection = TIM_ICSELECTION_DIRECTTI;
	if(HAL_TIM_IC_ConfigChannel(&hTimer10, &timer10IC_Config, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}
}

void LSE_Configuration(void)
{
	// Activate LSE
	RCC_OscInitTypeDef lse_init;
	lse_init.OscillatorType = RCC_OSCILLATORTYPE_LSE;
	lse_init.LSEState = RCC_LSE_ON;
	if(HAL_RCC_OscConfig(&lse_init) != HAL_OK)
	{
		Error_handler();
	}

	HAL_RCC_MCOConfig(RCC_MCO1, RCC_MCOSOURCE_LSE, RCC_MCODIV_1);
}

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
	if(! capture_done)
	{
		if(count == 1)
		{
			input_capture[0] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count++;
		}
		else if(count == 2)
		{
			input_capture[1] = __HAL_TIM_GET_COMPARE(htim, TIM_CHANNEL_1);
			count = 1;
			capture_done = TRUE;
		}
	}
}

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

