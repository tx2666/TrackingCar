#include "stm32f10x.h"                  // Device header
#include "Motor.h"
#include "Serial.h"
#include <math.h>
#include "Sensor.h"

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

/**
 * @brief 偏离误差计算
 * @param pData 存储PID参数等相关数据的结构体地址
 * @retval 无
 */
void PID_Sensor_Error_Caculate(PID_Data_Typedef *pData)
{
	float Severe = 3.0;
	float Light = 0.5;
	float Err = 0;
	// 黑色的输出为0，白色输出为1
	if (Sensor_Data_Bit[0] == 1)
	{
		Err -= Severe;
	}
	if (Sensor_Data_Bit[1] == 1)
	{
		Err -= Light;
	}
	if (Sensor_Data_Bit[2] == 1)
	{
		Err += 0;
	}
	if (Sensor_Data_Bit[3] == 1)
	{
		Err += Light;
	}
	if (Sensor_Data_Bit[4] == 1)
	{
		Err += Severe;
	}
	// 赋值
	pData->Error0 = Err;
}

/**
 * @brief 对传感器使用的PID调控算法
 * @param pData 存储PID参数等相关数据的结构体地址
 * @param Mode PID模式，POSTION，ADDITION
 * @retval 无
 */
void PID_Sensor_Caculate(PID_Data_Typedef *pData, PID_Mode Mode)
{
	/* 变量传递 */ 
	pData->Error2 = pData->Error1;
	pData->Error1 = pData->Error0;
	/* 误差计算 */
	PID_Sensor_Error_Caculate(pData);

	float kp = pData->Kp;
	float ki = pData->Ki;
	float kd = pData->Kd;

	float Out_P = 0;
	float Out_I = 0;
	float Out_D = 0;
	
	if (Mode == POSTION)
	{
		// 位置式PID

	}
	else if (Mode == ADDITION)
	{
		// 增量式PID
		// P
		Out_P = kp * (pData->Error0 - pData->Error1);
		// I
		Out_I = ki * pData->Error0;
		// 积分限幅
		if (Out_I >= 50)
		{
			Out_I = 50;
		}
		else if (Out_I <= -50)
		{
			Out_I = -50;
		}
		// D
		Out_D = kd * ((pData->Error0 - pData->Error1) - (pData->Error1 - pData->Error2));
		// Out
		pData->Out += Out_P + Out_I + Out_D;
	}

	pData->P = Out_P;
	pData->I = Out_I;
	pData->D = Out_D;

	// 输出限幅
	if (pData->Out >= (Target_Speed + 90.0))
	{
		pData->Out = (Target_Speed + 90.0);
	}
	else if (pData->Out <= -(Target_Speed + 90.0))
	{
		pData->Out = -(Target_Speed + 90.0);
	}
}

void PID_TypedefStructInit(PID_Data_Typedef *PID_Struct)
{
	PID_Struct->Magnification = 1;
	PID_Struct->Kp = 1.0;
	PID_Struct->Ki = 1.3;
	PID_Struct->Kd = -0.2;
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
