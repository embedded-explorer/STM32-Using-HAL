#include <string.h>
#include <stdio.h>
#include "main.h"

void Error_handler(void);
void UART2_Init(void);
void ButtonInit(void);
void RTC_Init(void);
void RTC_CalendarConfig(void);
void RTC_AlarmConfig(void);
char* getWeekDay (uint8_t n);

UART_HandleTypeDef huart2;
RTC_HandleTypeDef hrtc;
RTC_TimeTypeDef rtc_time;
RTC_DateTypeDef rtc_date;

char message[100];

int main(void)
{
	HAL_Init();
	ButtonInit();
	RTC_Init();
	UART2_Init();

	//Enable clock for PWR Controller block
	__HAL_RCC_PWR_CLK_ENABLE();

	if(__HAL_PWR_GET_FLAG(PWR_FLAG_SB) != RESET)
	{
		PWR->CR |= PWR_CR_CSBF;
		__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc,RTC_FLAG_ALRAF);
		__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

		sprintf(message, "Exiting Standby Mode\r\n");
		if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
		{
			Error_handler();
		}
	}

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

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	RTC_CalendarConfig();
	RTC_AlarmConfig();

	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	sprintf(message, "Time: %02d:%02d:%02d\r\n", rtc_time.Hours, rtc_time.Minutes, rtc_time.Seconds);
	if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}

	sprintf(message, "%s: %02d:%02d:20%02d\r\n", (char*)getWeekDay(rtc_date.WeekDay), rtc_date.Date, rtc_date.Month, rtc_date.Year);
	if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}

#ifdef WAKEUP_STANDBY
	sprintf(message, "Entering Standby Mode\r\n");
	if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}

	//make sure that WUF and RTC alarm A flag are cleared
	__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc,RTC_FLAG_ALRAF);
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);

	// Enter standby mode
	HAL_PWR_EnterSTANDBYMode();
#endif

#ifdef WAKEUP_STOP
	sprintf(message, "Entering STOP Mode\r\n");
	if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}

	//make sure alarm A flag are cleared
	__HAL_RTC_ALARM_CLEAR_FLAG(&hrtc,RTC_FLAG_ALRAF);

	// Enter standby mode
	HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
#endif

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
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;

	if(HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_handler();
	}
}

void RTC_CalendarConfig(void)
{
	rtc_time.Hours = 11;
	rtc_time.Minutes = 59;
	rtc_time.Seconds = 50;
	rtc_time.TimeFormat = RTC_HOURFORMAT12_AM;
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

void RTC_AlarmConfig(void)
{
	RTC_AlarmTypeDef rtc_alarm;

	memset(&rtc_alarm, 0, sizeof(rtc_alarm));
	HAL_RTC_DeactivateAlarm(&hrtc, RTC_ALARM_A);

	rtc_alarm.Alarm = RTC_ALARM_A;
	rtc_alarm.AlarmTime.Hours = 12;
	rtc_alarm.AlarmTime.Minutes = 0;
	rtc_alarm.AlarmTime.Seconds = 0;
	rtc_alarm.AlarmTime.TimeFormat = RTC_HOURFORMAT12_PM;
	rtc_alarm.AlarmSubSecondMask = RTC_ALARMSUBSECONDMASK_NONE;
	rtc_alarm.AlarmMask = RTC_ALARMMASK_DATEWEEKDAY;

	if(HAL_RTC_SetAlarm_IT(&hrtc, &rtc_alarm, RTC_FORMAT_BIN) != HAL_OK)
	{
		Error_handler();
	}
}

void HAL_RTC_AlarmAEventCallback(RTC_HandleTypeDef *hrtc)
{
	sprintf(message, "Exiting STOP Mode\r\n");
	if(HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY) != HAL_OK)
	{
		Error_handler();
	}
}

char* getWeekDay (uint8_t n)
{
	char *Weekday[] = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};

	return (Weekday[n - 1]);
}
