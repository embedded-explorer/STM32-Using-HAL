#include "main.h"

void Error_handler(void);
void SystemClockConfig(void);
void Timer10_Init(void);
void GPIO_Init(void);

TIM_HandleTypeDef hTimer10;

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	GPIO_Init();
	Timer10_Init();

	// Start the timer
	HAL_TIM_Base_Start(&hTimer10);

	while(1)
	{
		// Wait for update event
		while(!(TIM10->SR & TIM_SR_UIF));
		// Delay has been elapsed
		TIM10->SR &= ~(0x0001);
		HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
	}

	return 0;
}

void SystemClockConfig()
{

}

void Error_handler(void)
{
	while(1);
}

void Timer10_Init()
{
	hTimer10.Instance = TIM10;
	hTimer10.Init.Prescaler = 23;
	hTimer10.Init.Period = 64000-1;
	hTimer10.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&hTimer10) != HAL_OK)
	{
		Error_handler();
	}
}

void GPIO_Init(void)
{
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitTypeDef gpioLED;
	gpioLED.Pin = GPIO_PIN_5;
	gpioLED.Mode = GPIO_MODE_OUTPUT_PP;
	gpioLED.Speed = GPIO_SPEED_FAST;
	gpioLED.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOA, &gpioLED);
}
