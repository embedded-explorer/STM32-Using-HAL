#include <string.h>
#include <stdio.h>
#include "main.h"

void UART1_Init(void);
void Error_handler(void);
void SystemClockConfig(void);
void CAN1_Init(void);
void CAN1_Tx(void);
void CAN1_FilterConfig(void);
void CAN1_Rx(void);

UART_HandleTypeDef huart1;
RCC_OscInitTypeDef osc_init;
RCC_ClkInitTypeDef clk_init;
CAN_HandleTypeDef hCAN1;

char notify[100];

int main(void)
{
	HAL_Init();
	SystemClockConfig();
	UART1_Init();
	CAN1_Init();
	CAN1_FilterConfig();
	if(HAL_CAN_ActivateNotification(&hCAN1, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING | CAN_IT_BUSOFF) != HAL_OK)
	{
		Error_handler();
	}

	if(HAL_CAN_Start(&hCAN1) != HAL_OK)
	{
		Error_handler();
	}

	CAN1_Tx();
	CAN1_Rx();

	while(1);

	return 0;
}

// High Level Peripheral Initialization
void UART1_Init(void)
{
	huart1.Instance = USART1;
	huart1.Init.BaudRate = 115200;
	huart1.Init.WordLength = UART_WORDLENGTH_8B;
	huart1.Init.StopBits = UART_STOPBITS_1;
	huart1.Init.Parity = UART_PARITY_NONE;
	huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart1.Init.Mode = UART_MODE_TX_RX;

	if(HAL_UART_Init(&huart1) != HAL_OK)
	{
		//  There is a problem
		Error_handler();
	}
}

void Error_handler(void)
{
	while(1);
}

// Drive the clock from HSE
void SystemClockConfig(void)
{
	osc_init.OscillatorType = RCC_OSCILLATORTYPE_HSE;
	osc_init.HSEState = RCC_CR_HSEON;
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

	if(HAL_RCC_ClockConfig(&clk_init, FLASH_ACR_LATENCY_0) != HAL_OK)
	{
		Error_handler();
	}

	// Disable HSI
	__HAL_RCC_HSI_DISABLE();

	// Configure SysTick timer
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
	HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

void CAN1_Init(void)
{
	hCAN1.Instance = CAN1;
	hCAN1.Init.Mode = CAN_MODE_LOOPBACK;
	hCAN1.Init.AutoBusOff = ENABLE;
	hCAN1.Init.AutoRetransmission = ENABLE;
	hCAN1.Init.AutoWakeUp = DISABLE;
	hCAN1.Init.ReceiveFifoLocked = DISABLE;
	hCAN1.Init.TimeTriggeredMode = DISABLE;
	hCAN1.Init.TransmitFifoPriority = DISABLE;
	hCAN1.Init.Prescaler = 1;
	hCAN1.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hCAN1.Init.TimeSeg1 = CAN_BS1_13TQ;
	hCAN1.Init.TimeSeg2 = CAN_BS2_2TQ;

	if(HAL_CAN_Init(&hCAN1) != HAL_OK)
	{
		Error_handler();
	}
}

void CAN1_Tx(void)
{
	CAN_TxHeaderTypeDef TxHeader;
	uint32_t TxMailbox;

	uint8_t message[5] = {'H', 'E', 'L', 'L', 'O'};

	TxHeader.DLC = 5;
	TxHeader.StdId = 0x65D;
	TxHeader.IDE = CAN_ID_STD;
	TxHeader.RTR = CAN_RTR_DATA;

	if(HAL_CAN_AddTxMessage(&hCAN1, &TxHeader, message, &TxMailbox) != HAL_OK)
	{
		Error_handler();
	}
}

void CAN1_Rx(void)
{

}

void CAN1_FilterConfig(void)
{
	CAN_FilterTypeDef FilterConfig;

	FilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
	FilterConfig.FilterIdHigh = 0x0000;
	FilterConfig.FilterIdLow = 0x0000;
	FilterConfig.FilterMaskIdHigh = 0x0000;
	FilterConfig.FilterMaskIdLow = 0x0000;
	FilterConfig.FilterBank = 0;
	FilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
	FilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
	FilterConfig.FilterActivation = CAN_FILTER_ENABLE;

	if(HAL_CAN_ConfigFilter(&hCAN1, &FilterConfig) != HAL_OK)
	{
		Error_handler();
	}
}

void HAL_CAN_TxMailbox0CompleteCallback(CAN_HandleTypeDef *hcan)
{
	sprintf(notify, "Transmission Successful using M0\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)notify, strlen(notify), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox1CompleteCallback(CAN_HandleTypeDef *hcan)
{
	sprintf(notify, "Transmission Successful using M1\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)notify, strlen(notify), HAL_MAX_DELAY);
}

void HAL_CAN_TxMailbox2CompleteCallback(CAN_HandleTypeDef *hcan)
{
	sprintf(notify, "Transmission Successful using M2\r\n");
	HAL_UART_Transmit(&huart1, (uint8_t*)notify, strlen(notify), HAL_MAX_DELAY);
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t buffer[5];

	CAN_RxHeaderTypeDef RxHeader;

	if(HAL_CAN_GetRxMessage(&hCAN1, CAN_RX_FIFO0, &RxHeader, buffer) != HAL_OK)
	{
		Error_handler();
	}

	sprintf(notify, "Received Message: %s\r\n", buffer);
	HAL_UART_Transmit(&huart1, (uint8_t*)notify, strlen(notify), HAL_MAX_DELAY);
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
	Error_handler();
}
