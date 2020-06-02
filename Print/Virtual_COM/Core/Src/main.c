#include <string.h>
#include <stdio.h>
#include "stm32f4xx_hal.h"

void UART2_Init(void);

UART_HandleTypeDef huart2;

char message[100];

int main(void)
{
	HAL_Init();
	UART2_Init();

	sprintf(message, "Application is running\r\n");

	HAL_UART_Transmit(&huart2, (uint8_t*)message, strlen(message), HAL_MAX_DELAY);

	return 0;
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

	HAL_UART_Init(&huart2);
}
