#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Serial.h"
#include <math.h>

#define MAX_OUT ((float)1000)

typedef struct {
	float Magnification;  	// 倍数，表示最终结果要乘多少倍
	float Kp;				// 比例项系数
	float Ki;				// 积分项系数
	float Kd;				// 微分项系数
	float Current;			// 当前速度
	float Target;			// 目标速度
	float P;				// 比例项结果
	float I;				// 积分项结果
	float D;				// 微分项结果
	float Out;				// 输出
	float Error1;			// 上次误差
	float Error2;			// 上上次误差
	float Error0;			// 当前误差
	float SumError;			// 误差积分
	uint16_t Count1;
	uint16_t Count2;
} PID_Data_Typedef;

typedef enum {
	POSTION = 0,
	ADDITION = 1
} PID_Mode;

typedef struct
{
	uint8_t Motor_Num;
	PID_Mode Mode;
	PID_Data_Typedef *pPID_Data_Structure;
} PID_Tick_Typedef;


/**
 * @brief PID调控电机速度函数
 * @param Motor_Num 电机编号，从1开始编号
 * @param pData 存储PID参数等相关数据的结构体地址
 * @param Mode PID模式，POSTION，ADDITION
 * @retval 无，仅对pid对应的结构体数据进行修改
 */
void PID_Motor_Control(uint8_t Motor_Num, PID_Data_Typedef *pData, PID_Mode Mode)
{
	/* 变量传递 */ 
	pData->Error2 = pData->Error1;
	pData->Error1 = pData->Error0;
	// 正值代表大于
	// 负值代表小于
	pData->Error0 = pData->Target - pData->Current;
	
	float kp = pData->Kp;
	float ki = pData->Ki;
	float kd = pData->Kd;

	float OutP = 0, OutI = 0, OutD = 0;

	/* 位置式PID */
	if (Mode == POSTION)
	{
		// P
		OutP = kp * pData->Error0;
		// I
		OutI = ki * pData->SumError;
		// D 系数*(本次误差-上次误差)
		OutD = kd * (pData->Error0 - pData->Error1);
		// Out
		pData->Out = OutP + OutI + OutD;
	}
	/* 增量式PID */
	else if (Mode == ADDITION)
	{
		// P
		OutP = kp * (pData->Error0 - pData->Error1);
		// I
		OutI = ki * pData->Error0;
		// D
		OutD = kd * ((pData->Error0 - pData->Error1) - (pData->Error1 - pData->Error2));
		// Out
		pData->Out += OutP + OutI + OutD;
	}



	pData->P = OutP;
	pData->I = OutI;
	pData->D = OutD;
	/* 输出限幅 */
	if (pData->Out >= MAX_OUT) pData->Out = MAX_OUT;
	else if (pData->Out <= -MAX_OUT) pData->Out = -MAX_OUT;
}

void PID_TypedefStructInit(PID_Data_Typedef *PID_Struct)
{
	PID_Struct->Magnification = 1;
	PID_Struct->Kp = 1;
	PID_Struct->Ki = 2;
	PID_Struct->Kd = -0.1;
	PID_Struct->Current = 0;
	PID_Struct->Target = 0;
	PID_Struct->P = 0;
	PID_Struct->I = 0;
	PID_Struct->D = 0;
	PID_Struct->Out = 0;
	PID_Struct->Error1 = 0;
	PID_Struct->Error2 = 0;	
	PID_Struct->Error0 = 0;
	PID_Struct->SumError = 0;
	PID_Struct->Count1 = 0;
	PID_Struct->Count2 = 0;
}

void PID_TypedefStructReset(PID_Data_Typedef *PID_Struct)
{
	PID_Struct->Current = 0;
	// PID_Struct->Target = 0;
	PID_Struct->P = 0;
	PID_Struct->I = 0;
	PID_Struct->D = 0;
	PID_Struct->Out = 0;
	PID_Struct->Error1 = 0;
	PID_Struct->Error2 = 0;	
	PID_Struct->Error0 = 0;
	PID_Struct->SumError = 0;
	PID_Struct->Count1 = 0;
	PID_Struct->Count2 = 0;
}

void PID_Tick(PID_Tick_Typedef *PID_Tick_Structure)
{
	if (PID_Tick_Structure->pPID_Data_Structure->Count1 >= 10)
	{
		PID_Motor_Control(PID_Tick_Structure->Motor_Num,
			 (PID_Tick_Structure->pPID_Data_Structure),
			  PID_Tick_Structure->Mode);
		if (PID_Tick_Structure->Motor_Num == 1)
		{
			Motor1_SetSpeed(PID_Tick_Structure->pPID_Data_Structure->Out);
		}
		else if (PID_Tick_Structure->Motor_Num == 2)
		{
			Motor2_SetSpeed(PID_Tick_Structure->pPID_Data_Structure->Out);
		}
		PID_Tick_Structure->pPID_Data_Structure->Count1 = 0;
	}
	PID_Tick_Structure->pPID_Data_Structure->Count1++;
}
