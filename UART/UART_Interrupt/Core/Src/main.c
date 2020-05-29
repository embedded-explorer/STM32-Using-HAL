#include <string.h>
#include "stm32f4xx_hal.h"
#include "main.h"

#define TRUE 	1
#define FALSE	0

void SystemClockConfig();
void UART2_Init(void);
void Error_handler(void);
uint8_t convertToCaps(uint8_t data);

UART_HandleTypeDef huart2;


uint32_t count;
uint8_t received_data;
uint8_t data_buffer[100];
uint8_t reception_complete;
uint8_t transmission_complete;

char *user_data = "Enter Some Data\r\n";
int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART2_Init();

	if(HAL_UART_Transmit_IT(&huart2, (uint8_t*)user_data, strlen(user_data)) != HAL_OK)
	{
		Error_handler();
	}

	while(reception_complete != TRUE)
		HAL_UART_Receive_IT(&huart2, &received_data, 1);

	while(1);

	return 0;
}

void SystemClockConfig()
{

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

uint8_t convertToCaps(uint8_t data)
{
	if(data >= 'a' && data <= 'z')
	{
		data -= ('a' - 'A');
	}
	return data;
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if(received_data == '\r')
	{
		reception_complete = TRUE;
		data_buffer[count++] = '\r';
		if(HAL_UART_Transmit_IT(&huart2, data_buffer, count) != HAL_OK)
		{
			Error_handler();
		}
	}
	else
	{
		data_buffer[count++] = convertToCaps(received_data);
	}
}
