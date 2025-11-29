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
#include "Sensor.h"

#define SERIAL_OUT_MODE_NONE			((uint8_t)0)
#define SERIAL_OUT_MODE_MOTOR_DATA		((uint8_t)1)
#define SERIAL_OUT_MODE_SENSOR_DATA		((uint8_t)2)

PID_Data_Typedef PID_Motor1;
PID_Data_Typedef PID_Motor2;
PID_Data_Typedef PID_Sensor_Data;
PID_Data_Typedef *pPID_Motor;

PID_Tick_Typedef PID_Tick_Motor1;
PID_Tick_Typedef PID_Tick_Motor2;

uint8_t UIpos = 0;
uint8_t Edit_Mode = 0;
uint8_t Serial_Out_Mode = SERIAL_OUT_MODE_MOTOR_DATA;
uint8_t start_flag = 0;

int16_t Tar_Step = 60;

void UI_Exhibit(void);
void Key_Monitor(void);

int main(void)
{
	Sensor_Init();
	Key_Init();
	Timer_Init();
	Encoder_Init();
	Motor_Init();
	Serial_Init();
	UI_Init();
	PID_TypedefStructInit(&PID_Motor1);
	PID_Tick_Motor1.Mode = ADDITION;
	PID_Tick_Motor1.Motor_Num = 1;
	PID_Tick_Motor1.pPID_Data_Structure = &PID_Motor1;
	PID_TypedefStructInit(&PID_Motor2);
	PID_Tick_Motor2.Mode = ADDITION;
	PID_Tick_Motor2.Motor_Num = 2;
	PID_Tick_Motor2.pPID_Data_Structure = &PID_Motor2;
	PID_TypedefStructInit(&PID_Sensor_Data);
	PID_Sensor_Data.Kp = 1;
	PID_Sensor_Data.Ki = 20;
	PID_Sensor_Data.Kd = -0.5;


	UIpos = UI_root.Num;
	pPID_Motor = &PID_Motor1;

	UI_Show(&UI_root);

	Motor1_SetSpeed(0);
	Motor2_SetSpeed(0);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	while (1)
	{
		UI_Exhibit();
		Key_Monitor();
	}
}

void UI_Exhibit(void)
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
		if (pPID_Motor == &PID_Motor1)
		{
			UI_PID_Show_Motor_Num(1);
		}
		else if (pPID_Motor == &PID_Motor2)
		{
			UI_PID_Show_Motor_Num(2);
		}
	}
	else if (UIpos == UI_test.Num)
	{
		UI_Show(&UI_test);

	}
	else if (UIpos == UI_target.Num)
	{
		UI_Show(&UI_target);
		UI_Show_test_PID_Struct(&PID_Motor1, &PID_Motor2, Tar_Step);
	}
	else if (UIpos == UI_sensor.Num)
	{
		UI_Show(&UI_sensor);
		UI_Show_Sensor(Sensor_Data_Bit);
	}
	else if (UIpos == UI_serial.Num)
	{
		UI_Show(&UI_serial);
		UI_Show_Serial(Serial_Out_Mode);
	}	
	else if (UIpos == UI_go.Num)
	{
		UI_Show(&UI_go);
		UI_Show_Start(PID_Motor1.Target, PID_Motor2.Target);
		OLED_ShowChar(1, 7, 'D');
		if (PID_Sensor_Data.Out > 0)
		{
			OLED_ShowChar(1, 6, 'L');
		}
		else if (PID_Sensor_Data.Out < 0)
		{
			OLED_ShowChar(1, 6, 'R');
		}
	}
}

void Key_Monitor(void)
{
	/* 按钮检测 */
	/* 上 */
	/* 单击 */
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
		else if (UIpos == UI_test.Num)
		{
			if (Edit_Mode == 1)
			{
				pPID_Motor->Target += 1;
			}
			else if (Edit_Mode == 0)
			{
				UI_MoveUp_Cursor(&UI_test);
			}
		}
		else if (UIpos == UI_target.Num)
		{
			if (Edit_Mode == 1)
			{
				if (UI_target.cursor == 2)
				{
					PID_Motor1.Target += Tar_Step;
				}
				else if (UI_target.cursor == 3)
				{
					PID_Motor2.Target += Tar_Step;
				}
				else if (UI_target.cursor == 4)
				{
					Tar_Step += 1;
				}
			}
			else if (Edit_Mode == 0)
			{
				UI_MoveUp_Cursor(&UI_target);
			}
		}
		else if (UIpos == UI_sensor.Num)
		{

		}
		else if (UIpos == UI_serial.Num)
		{
			UI_MoveUp_Cursor(&UI_serial);
		}
	}
	/* 长按 */
	if (Key_GetState(KEY_UP, KEY_LONG))
	{
		if (UIpos == UI_root.Num)
		{

		}
		else if (UIpos == UI_start.Num)
		{

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

			}
		}
		else if (UIpos == UI_test.Num)
		{

		}
		else if (UIpos == UI_target.Num)
		{
			if (Edit_Mode == 1)
			{
				if (UI_target.cursor == 2)
				{
					PID_Motor1.Target += Tar_Step;
				}
				else if (UI_target.cursor == 3)
				{
					PID_Motor2.Target += Tar_Step;
				}
				else if (UI_target.cursor == 4)
				{
					Tar_Step += 1;
				}
			}
			else if (Edit_Mode == 0)
			{
				
			}
		}
	}
	/* 二连 */
	if (Key_GetState(KEY_UP, KEY_DOUBLE))
	{

	}
	/* 三连 */
	if (Key_GetState(KEY_UP, KEY_TRINPLE))
	{

	}
	/* 下 */
	/* 单击 */
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
		else if (UIpos == UI_test.Num)
		{
			if (Edit_Mode == 1)
			{
				pPID_Motor->Target -= 1;
			}
			else if (Edit_Mode == 0)
			{
				UI_MoveDown_Cursor(&UI_test);
			}
		}
		else if (UIpos == UI_target.Num)
		{
			if (Edit_Mode == 1)
			{
				if (UI_target.cursor == 2)
				{
					PID_Motor1.Target -= Tar_Step;
				}
				else if (UI_target.cursor == 3)
				{
					PID_Motor2.Target -= Tar_Step;
				}
				else if (UI_target.cursor == 4)
				{
					Tar_Step -= 1;
				}
			}
			else if (Edit_Mode == 0)
			{
				UI_MoveDown_Cursor(&UI_target);
			}
		}
		else if (UIpos == UI_serial.Num)
		{
			UI_MoveDown_Cursor(&UI_serial);
		}
	}
	/* 长按 */
	if (Key_GetState(KEY_DOWN, KEY_LONG))
	{
		if (UIpos == UI_root.Num)
		{

		}
		else if (UIpos == UI_start.Num)
		{

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
		}
		else if (UIpos == UI_test.Num)
		{
			if (Edit_Mode == 1)
			{
				if (UI_test.cursor == 2)
				{
					pPID_Motor->Target -= 1;
				}
			}
		}
		else if (UIpos == UI_target.Num)
		{
			if (Edit_Mode == 1)
			{
				if (UI_target.cursor == 2)
				{
					PID_Motor1.Target -= Tar_Step;
				}
				else if (UI_target.cursor == 3)
				{
					PID_Motor2.Target -= Tar_Step;
				}
				else if (UI_target.cursor == 4)
				{
					Tar_Step -= 1;
				}
			}
			else if (Edit_Mode == 0)
			{
				// 位置预留
			}
		}
	}
	/* 二连 */
	if (Key_GetState(KEY_DOWN, KEY_DOUBLE))
	{

	}
	/* 三连 */
	if (Key_GetState(KEY_DOWN, KEY_TRINPLE))
	{
		
	}
	/* 确定 */
	/* 单击 */
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
				UIpos = UI_test.Num;
				UI_Reset_Cursor(&UI_root);
				OLED_Clear();
			}
			else if (UI_root.cursor == 4)
			{
				// 位置预留
			}
		}
		else if (UIpos == UI_start.Num)
		{
			if (UI_start.cursor == 2)
			{
				UIpos = UI_go.Num;
				UI_Reset_Cursor(&UI_start);
				OLED_Clear();
				start_flag = 1;
				PID_Motor1.Target = Target_Speed;
				PID_Motor2.Target = Target_Speed;
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
		else if (UIpos == UI_test.Num)
		{
			if (UI_test.cursor == 2)
			{
				// 换到目标Target速度修改界面
				UIpos = UI_target.Num;
				UI_Reset_Cursor(&UI_test);
				OLED_Clear();
			}
			else if (UI_test.cursor == 3)
			{
				// 换到传感器Sensor数据检测界面
				UIpos = UI_sensor.Num;
				UI_Reset_Cursor(&UI_test);
				OLED_Clear();
			}
			else if (UI_test.cursor == 4)
			{
				UIpos = UI_serial.Num;
				UI_Reset_Cursor(&UI_test);
				OLED_Clear();
			}
		}
		else if (UIpos == UI_target.Num)
		{
			if (Edit_Mode == 1)
			{
				Edit_Mode = 0;
				UI_Show_Edit_Mode(Edit_Mode);
			}
			else if (Edit_Mode == 0)
			{
				if (UI_target.cursor == 2)
				{
					Edit_Mode = 1;
					UI_Show_Edit_Mode(Edit_Mode);
				}
				else if (UI_target.cursor == 3)
				{
					Edit_Mode = 1;
					UI_Show_Edit_Mode(Edit_Mode);
				}
				else if (UI_target.cursor == 4)
				{
					Edit_Mode = 1;
					UI_Show_Edit_Mode(Edit_Mode);
				}
			}
		}
		else if (UIpos == UI_sensor.Num)
		{
			// 没有什么可以按确定的，只能退出
		}
		else if (UIpos == UI_serial.Num)
		{
			if (UI_serial.cursor == 1)
			{
				Serial_Out_Mode = SERIAL_OUT_MODE_MOTOR_DATA;
			}
			else if (UI_serial.cursor == 2)
			{
				Serial_Out_Mode = SERIAL_OUT_MODE_SENSOR_DATA;
			}
			else if (UI_serial.cursor == 3)
			{
				// 位置预留
			}
			else if (UI_serial.cursor == 4)
			{
				// 位置预留
			}
		}
	}
	/* 长按 */
	if (Key_GetState(KEY_CONFIRM, KEY_LONG))
	{

	}
	/* 取消 */
	/* 单击 */
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
		else if (UIpos == UI_test.Num)
		{
			UIpos = UI_root.Num;
			UI_Reset_Cursor(&UI_test);
			OLED_Clear();
		}
		else if (UIpos == UI_target.Num)
		{
			if (Edit_Mode == 1)
			{
				Edit_Mode = 0;
				UI_Show_Edit_Mode(Edit_Mode);
			}
			else if (Edit_Mode == 0)
			{
				UIpos = UI_test.Num;
				UI_Reset_Cursor(&UI_target);
				OLED_Clear();
			}
		}
		else if (UIpos == UI_sensor.Num)
		{
			UIpos = UI_test.Num;
			UI_Reset_Cursor(&UI_sensor);
			OLED_Clear();
		}
		else if (UIpos == UI_serial.Num)
		{
			UIpos = UI_test.Num;
			UI_Reset_Cursor(&UI_serial);
			OLED_Clear();
		}
		else if (UIpos == UI_go.Num)
		{
			
		}
	}
	/* 长按 */
	if (Key_GetState(KEY_CANCEL, KEY_LONG))
	{
		if (UIpos == UI_go.Num)
		{
			UIpos = UI_root.Num;
			UI_Reset_Cursor(&UI_go);
			OLED_Clear();
			start_flag = 0;
			PID_TypedefStructReset(&PID_Motor1);
			PID_TypedefStructReset(&PID_Motor2);
			PID_TypedefStructReset(&PID_Sensor_Data);
			PID_Motor2.Target = 0;
			PID_Motor1.Target = 0;
		}
	}
}

void TIM1_UP_IRQHandler(void)
{
	static uint16_t count = 0;
	static uint16_t count2 = 0;
	static uint16_t count_untrack = 0;

	if (TIM_GetITStatus(TIM1, TIM_IT_Update) == SET)
	{
		Key_Tick();
		Encoder_Tick();
		Sensor_Tick();
		PID_Motor1.Current = -Encoder1_Count;
		PID_Motor2.Current = -Encoder2_Count;

		count ++;
		count2 ++;
		if (count2 >= 50)
		{
			if (start_flag == 1)
			{
				PID_Sensor_Caculate(&PID_Sensor_Data, ADDITION);
				uint8_t count_Signal = 0;
				for (int i = 0; i < 5; i++)
				{
					// 判断是否丢失目标
					count_Signal += Sensor_Data_Bit[i];
				}
				// 如果传感器都是未检测到黑线，那么就倒车
				if (count_Signal == 0)
				{
					count_untrack ++;
					if (count_untrack >= 2)
					{
						// 倒车
						PID_Motor1.Target = -0.5 * Target_Speed;
						PID_Motor2.Target = -0.5 * Target_Speed;
						count_untrack = 0;
					}
				}
				else
				{
					if (PID_Sensor_Data.Out > 0)
					{
						PID_Motor2.Target = Target_Speed - PID_Sensor_Data.Out;
						PID_Motor1.Target = Target_Speed;
					}
					else if (PID_Sensor_Data.Out < 0)
					{
						PID_Motor1.Target = Target_Speed + PID_Sensor_Data.Out;
						PID_Motor2.Target = Target_Speed;
					}
					// if (Sensor_Data_Bit[2] == 1)
					// {
					// 	if (PID_Motor2.Target <= Target_Speed)
					// 	{
					// 		PID_Motor2.Target += 4;
					// 	}
					// 	if (PID_Motor1.Target <= Target_Speed)
					// 	{
					// 		PID_Motor1.Target += 4;
					// 	}
					// }
				}
				
			}
			count2 = 0;
		}
		PID_Tick(&PID_Tick_Motor1);
		PID_Tick(&PID_Tick_Motor2);
		if (count >= 10)
		{
			// 串口输出部分
			if (Serial_Out_Mode == SERIAL_OUT_MODE_MOTOR_DATA)
			{
				Serial_Printf("Data:%.2f, %.2f, %.2f, %.2f, %.2f, %.2f\r\n",
					PID_Motor2.Target, PID_Motor2.Current, PID_Motor2.P, PID_Motor2.I, PID_Motor2.D, PID_Motor2.Out);
			}
			else if (Serial_Out_Mode == SERIAL_OUT_MODE_SENSOR_DATA)
			{
				Serial_Printf("Sensor:%d, %d, %d, %d, %d\r\n", 
					Sensor_Data_Bit[0], Sensor_Data_Bit[1], Sensor_Data_Bit[2], Sensor_Data_Bit[3], Sensor_Data_Bit[4]);
			}

			count = 0;
		}
		TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
	}
}
