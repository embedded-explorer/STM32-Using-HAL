#include <string.h>
#include <stdio.h>
#include "main.h"

void Error_handler(void);
void SystemClockConfig(void);
void UART2_Init(void);
void ButtonInit(void);
void RTC_Init(void);
void RTC_CalendarConfig(void);
char* getWeekDay (uint8_t n);

UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;

char message[100];

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	ButtonInit();
	UART2_Init();
	RTC_Init();
	//RTC_CalendarConfig();

#ifdef checkStandBy
	// Check StandBy Flag
	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)

	{
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_SB);

		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

		HAL_GPIO_EXTI_Callback(0);
	}

	//Enable the wakeup pin 1 in pwr_csr register
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1);

	HAL_PWR_EnterSTANDBYMode();
#endif

	while(1);

	return 0;
}

void Error_handler(void)
{
	while(1);
}

void ButtonInit(void)
{
	// User Button is connected to PC13
	GPIO_InitTypeDef gpioc;

	gpioc.Pin = GPIO_PIN_13;
	gpioc.Mode = GPIO_MODE_IT_RISING;
	HAL_GPIO_Init(GPIOC, &gpioc);

	__HAL_RCC_GPIOC_CLK_ENABLE();

	// Enable Button Interrupt
	HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
	HAL_NVIC_SetPriority(EXTI15_10_IRQn, 15, 0);
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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	sprintf(message, "Time: %02d:%02d:%02d %s\r\n", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds, \
			rtc_time.TimeFormat ? "PM" : "AM");
	if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}

	sprintf(message, "%s: %02d:%02d:20%02d\r\n", (char*)getWeekDay(rtc_date.WeekDay), rtc_date.Date, rtc_date.Month, rtc_date.Year);
	if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}
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

void RTC_Init(void)
{
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_12;
	hrtc.Init.AsynchPrediv = 0x7F;
	hrtc.Init.SynchPrediv = 0xFF;

	if(HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_handler();
	}
}

void RTC_CalendarConfig(void)
{
	rtc_time.Hours = 3;
	rtc_time.Minutes = 00;
	rtc_time.Seconds = 00;
	rtc_time.TimeFormat = RTC_HOURFORMAT12_PM;
	if(HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}

	rtc_date.Date = 6;
	rtc_date.Month = RTC_MONTH_JUNE;
	rtc_date.WeekDay = RTC_WEEKDAY_SATURDAY;
	rtc_date.Year = 20;
	if(HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}
}

char* getWeekDay (uint8_t n)
{
	char *Weekday[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

	return (Weekday[n - 1]);
}
