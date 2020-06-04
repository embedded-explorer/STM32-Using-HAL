#include "main.h"

extern CAN_HandleTypeDef hCAN1;
extern TIM_HandleTypeDef hTIM3;

void SysTick_Handler(void)
{
	HAL_IncTick();
	HAL_SYSTICK_IRQHandler();
}

void USB_HP_CAN1_TX_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hCAN1);
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hCAN1);
}

void CAN1_RX1_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hCAN1);
}

void CAN1_SCE_IRQHandler(void)
{
	HAL_CAN_IRQHandler(&hCAN1);
}

void TIM3_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&hTIM3);
}
