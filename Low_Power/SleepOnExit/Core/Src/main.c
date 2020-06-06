#include <string.h>
#include <stdio.h>
#include "main.h"

void Error_handler(void);
void Timer2_Init(void);
void UART2_Init(void);
void SystemClockConfig(void);
void GPIO_AnalogConfig(void);

UART_HandleTypeDef huart2;
TIM_HandleTypeDef hTimer2;
RCC_OscInitTypeDef osc_init;
RCC_ClkInitTypeDef clk_init;

char message [100];

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	GPIO_AnalogConfig();
	Timer2_Init();
	UART2_Init();

	// clear TIM2 status register to avoid unnecessary interrupts
	TIM2->SR = 0;

	//Enable Sleep on Exit
	SCB->SCR |= (1 << 1);

	// Start the timer
	if(HAL_TIM_Base_Start_IT(&hTimer2) != HAL_OK)
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

void GPIO_AnalogConfig(void)
{
	GPIO_InitTypeDef gpioA;

	gpioA.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_4 | GPIO_PIN_5 | \
				GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | \
				GPIO_PIN_10 | GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | \
				GPIO_PIN_14 | GPIO_PIN_15;
	gpioA.Mode = GPIO_MODE_ANALOG;

	HAL_GPIO_Init(GPIOA, &gpioA);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	sprintf(message, "I am Running at = %ldHz\n\r", HAL_RCC_GetSysClockFreq());
	if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}
}

#ifdef HSI_16MHz
void SystemClockConfig(void)
{

}

void Timer2_Init()
{
	hTimer2.Instance = TIM2;
	hTimer2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTimer2.Init.Prescaler = 25 - 1;
	hTimer2.Init.Period = 6400 - 1;
	hTimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&hTimer2) != HAL_OK)
	{
		Error_handler();
	}
}
#endif

#ifdef HSI_50MHz
void SystemClockConfig(void)
{
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	osc_init.HSIState = RCC_HSI_ON;
	osc_init.HSICalibrationValue = 16;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLCFGR_PLLSRC_HSI;

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	osc_init.PLL.PLLM = 16;
	osc_init.PLL.PLLN = 100;
	osc_init.PLL.PLLP = RCC_PLLP_DIV2;

	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_1WS)!= HAL_OK)
	{
		Error_handler();
	}

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void Timer2_Init()
{
	hTimer2.Instance = TIM2;
	hTimer2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTimer2.Init.Prescaler = 25 - 1;
	hTimer2.Init.Period = 20000 - 1;
	hTimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&hTimer2) != HAL_OK)
	{
		Error_handler();
	}
}
#endif

#ifdef HSE_8MHz
void SystemClockConfig(void)
{
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

void Timer2_Init()
{
	hTimer2.Instance = TIM2;
	hTimer2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTimer2.Init.Prescaler = 25 - 1;
	hTimer2.Init.Period = 3200 - 1;
	hTimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&hTimer2) != HAL_OK)
	{
		Error_handler();
	}
}
#endif

#ifdef HSE_50MHz
void SystemClockConfig(void)
{
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_HSE_BYPASS;
	osc_init.PLL.PLLState = RCC_PLL_ON;
	osc_init.PLL.PLLSource = RCC_PLLCFGR_PLLSRC_HSE;

	clk_init.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | \
						 RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
	clk_init.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;

	osc_init.PLL.PLLM = 8;
	osc_init.PLL.PLLN = 100;
	osc_init.PLL.PLLP = RCC_PLLP_DIV2;

	clk_init.AHBCLKDivider = RCC_SYSCLK_DIV1;
	clk_init.APB1CLKDivider = RCC_HCLK_DIV2;
	clk_init.APB2CLKDivider = RCC_HCLK_DIV1;

	if(HAL_RCC_OscConfig(&osc_init) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_1WS)!= HAL_OK)
	{
		Error_handler();
	}

	__HAL_RCC_HSI_DISABLE();

	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void Timer2_Init()
{
	hTimer2.Instance = TIM2;
	hTimer2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	hTimer2.Init.Prescaler = 25 - 1;
	hTimer2.Init.Period = 20000 - 1;
	hTimer2.Init.CounterMode = TIM_COUNTERMODE_UP;
	if(HAL_TIM_Base_Init(&hTimer2) != HAL_OK)
	{
		Error_handler();
	}
}
#endif

#ifdef UART_9600
void UART2_Init(void){
	huart2.Instance = USART2;
	huart2.Init.BaudRate = 9600;
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
#endif

#ifdef UART_115200
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
#endif

#ifdef UART_460800
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
#endif

