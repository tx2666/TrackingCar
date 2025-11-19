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

uint8_t UIpos = 0;

int main(void)
{
	UI_Init();
	UI_Show(&UI_root);
	UIpos = UI_root.Num;
	Key_Init();
	
	while (1)
	{
		
		/* 按钮检测 */
		if (Key_GetState(KEY_UP, KEY_PRESS))
		{
			if 		(UIpos == UI_root.Num)
			{

			}
			else if (UIpos == UI_start.Num)
			{

			}
			else if (UIpos == UI_PID.Num)
			{

			}
		}
	}
}



void TIM1_UP_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Key_Tick();

		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
