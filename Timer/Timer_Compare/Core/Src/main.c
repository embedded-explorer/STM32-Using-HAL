#include "main.h"

void Error_handler(void);
void Timer2_Init(void);

UART_HandleTypeDef huart2;
TIM_HandleTypeDef hTimer2;

uint32_t pulse = 1600000;
volatile uint32_t ccr_content;

int main(void)
{
	HAL_Init();
	Timer2_Init();

	// Start the timer
	if(HAL_TIM_OC_Start_IT(&hTimer2, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}

	while(1);

	return 0;
}

void Error_handler(void)
{
	while(1);
}

void Timer2_Init()
{
	TIM_OC_InitTypeDef timer2OC_Config;

	// Initialize Time Base Unit
	hTimer2.Instance = TIM2;
	hTimer2.Init.Prescaler = 0;
	hTimer2.Init.Period = 0xFFFFFFFF;
	hTimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_OC_Init(&hTimer2) != HAL_OK)
	{
		Error_handler();
	}

	// Configure Input Channel of Timer
	timer2OC_Config.OCMode = TIM_OCMODE_TOGGLE;
	timer2OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	timer2OC_Config.Pulse = pulse;
	if(HAL_TIM_OC_ConfigChannel(&hTimer2, &timer2OC_Config, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}
}

void HAL_TIM_OC_DelayElapsedCallback(TIM_HandleTypeDef *htim)
{
	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	{
		// Read content of CCR
		ccr_content = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
		// Reprogram CCR
		__HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL_1, (ccr_content + pulse));
	}
}
