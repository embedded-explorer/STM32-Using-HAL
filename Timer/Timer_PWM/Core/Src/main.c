#include <string.h>
#include "main.h"

void Error_handler(void);
void Timer2_Init(void);

TIM_HandleTypeDef hTimer2;
uint16_t pulse;

int main(void)
{
	HAL_Init();
	Timer2_Init();
	if(HAL_TIM_PWM_Start(&hTimer2, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}
	while(1)
	{
		while(pulse < hTimer2.Init.Period)
		{
			__HAL_TIM_SET_COMPARE(&hTimer2, TIM_CHANNEL_1, pulse);
			pulse += 10;
			HAL_Delay(10);
		}
		while(pulse > 0)
		{
			__HAL_TIM_SET_COMPARE(&hTimer2, TIM_CHANNEL_1, pulse);
			pulse -= 10;
			HAL_Delay(10);
		}
}


	return 0;
}

void Error_handler(void)
{
	while(1);
}

void Timer2_Init(void)
{
	TIM_OC_InitTypeDef timer2OC_Config;

	// Initialize Time Base Unit
	hTimer2.Instance = TIM2;
	hTimer2.Init.Prescaler = (25 - 1);
	hTimer2.Init.Period = (640 - 1);
	hTimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_PWM_Init(&hTimer2) != HAL_OK)
	{
		Error_handler();
	}

	// Configure Input Channel of Timer
	memset(&timer2OC_Config, 0, sizeof(TIM_OC_InitTypeDef));
	timer2OC_Config.OCMode = TIM_OCMODE_PWM1;
	timer2OC_Config.OCPolarity = TIM_OCPOLARITY_HIGH;
	timer2OC_Config.Pulse = (hTimer2.Init.Period * 50) / 100;
	if(HAL_TIM_PWM_ConfigChannel(&hTimer2, &timer2OC_Config, TIM_CHANNEL_1) != HAL_OK)
	{
		Error_handler();
	}
}
