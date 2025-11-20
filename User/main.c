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
PID_Typedef *pPID_Motor;

uint8_t UIpos = 0;
uint8_t Edit_Mode = 0;

int main(void)
{
	Key_Init();
	Timer_Init();
	UI_Init();
	UI_Show(&UI_root);
	UIpos = UI_root.Num;
	pPID_Motor = &PID_Motor1;
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	while (1)
	{
		if (UIpos == UI_root.Num)
		{
			UI_Show(&UI_root);
		}
		else if (UIpos == UI_start.Num)
		{
			UI_Show(&UI_start);
		}
		else if (UIpos == UI_PID.Num)
		{
			UI_Show(&UI_PID);
			UI_Show_PID(pPID_Motor->Kp, pPID_Motor->Ki, pPID_Motor->Kd);
		}
		/* 按钮检测 */
		/* 上 */
		if (Key_GetState(KEY_UP, KEY_PRESS))
		{
			if 		(UIpos == UI_root.Num)
			{
				UI_MoveUp_Cursor(&UI_root);
			}
			else if (UIpos == UI_start.Num)
			{
				UI_MoveUp_Cursor(&UI_start);
			}
			else if (UIpos == UI_PID.Num)
			{
				if (Edit_Mode == 1)
				{
					if (UI_PID.cursor == 2)
					{
						pPID_Motor->Kp += 0.01;
					}
					else if (UI_PID.cursor == 3)
					{
						pPID_Motor->Ki += 0.01;
					}
					else if (UI_PID.cursor == 4)
					{
						pPID_Motor->Kd += 0.01;
					}
				}
				else if (Edit_Mode == 0)
				{
					UI_MoveUp_Cursor(&UI_PID);
				}
			}
		}
		/* 下 */
		if (Key_GetState(KEY_DOWN, KEY_PRESS))
		{
			if 		(UIpos == UI_root.Num)
			{
				UI_MoveDown_Cursor(&UI_root);
			}
			else if (UIpos == UI_start.Num)
			{
				UI_MoveDown_Cursor(&UI_start);
			}
			else if (UIpos == UI_PID.Num)
			{
				if (Edit_Mode == 1)
				{
					if (UI_PID.cursor == 2)
					{
						pPID_Motor->Kp -= 0.01;
					}
					else if (UI_PID.cursor == 3)
					{
						pPID_Motor->Ki -= 0.01;
					}
					else if (UI_PID.cursor == 4)
					{
						pPID_Motor->Kd -= 0.01;
					}
				}
				else if (Edit_Mode == 0)
				{
					UI_MoveDown_Cursor(&UI_PID);
				}
			}
		}
		/* 确定 */
		if (Key_GetState(KEY_CONFIRM, KEY_PRESS))
		{
			if 		(UIpos == UI_root.Num)
			{
				if (UI_root.cursor == 1)
				{
					UIpos = UI_start.Num;
					UI_Reset_Cursor(&UI_root);
					OLED_Clear();
				}
				else if (UI_root.cursor == 2)
				{
					UIpos = UI_PID.Num;
					UI_Reset_Cursor(&UI_root);
					OLED_Clear();
				}
				else if (UI_root.cursor == 3)
				{
					// 位置预留
				}
				else if (UI_root.cursor == 4)
				{
					// 位置预留
				}
			}
			else if (UIpos == UI_start.Num)
			{
				
			}
			else if (UIpos == UI_PID.Num)
			{
				if (Edit_Mode == 1)
				{
					Edit_Mode = 0;
					UI_Show_Edit_Mode(Edit_Mode);
				}
				else if (Edit_Mode == 0)
				{
					if (UI_PID.cursor == 1)
					{
						if (pPID_Motor == &PID_Motor1)
						{
							pPID_Motor = &PID_Motor2;
						}
						else
						{
							pPID_Motor = &PID_Motor1;
						}
					}
					else
					{
						Edit_Mode = 1;
						UI_Show_Edit_Mode(Edit_Mode); 
					}
				}
			}
		}
		/* 取消 */
		if (Key_GetState(KEY_CANCEL, KEY_PRESS))
		{
			if 		(UIpos == UI_root.Num)
			{
				
			}
			else if (UIpos == UI_start.Num)
			{
				// 下面这个条件先空着，这个是要判断是不是在发车状态
				// 如果是确认发车，这个取消是退出发车状态，而不是返回主菜单
				if (0)
				{

				}
				else
				{
					UIpos = UI_root.Num;
					UI_Reset_Cursor(&UI_start);
					OLED_Clear();
				}
			}
			else if (UIpos == UI_PID.Num)
			{
				if (Edit_Mode == 1)
				{
					Edit_Mode = 0;
					UI_Show_Edit_Mode(Edit_Mode);
				}
				else if (Edit_Mode == 0) 
				{
					// 返回上一级
					UIpos = UI_root.Num;
					UI_Reset_Cursor(&UI_PID);
					OLED_Clear();
				}
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
