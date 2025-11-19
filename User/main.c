#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "Key.h"
#include "Timer.h"
#include "Encoder.h"
#include <string.h>
#include "Motor.h"
#include "PID.h"
#include "UI.h"

PID_Typedef PID_Motor1;
PID_Typedef PID_Motor2;

uint8_t KeyNum;
uint8_t Mode = 0;

int main(void)
{
	UI_Init();
	Show_UI(&UI_root);
	
	while (1)
	{
		
	}
}



void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
