#ifndef __PID_H
#define __PID_H

/* 变量类型部分 */
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

/* 函数部分 */
void PID_TypedefStructInit(PID_Data_Typedef *PID_Struct);							// 结构体初始化赋值
void PID_Motor_Control(uint8_t Motor_Num, PID_Data_Typedef *pid, PID_Mode Mode);
void PID_TypedefStructReset(PID_Data_Typedef *PID_Struct);							// 除参数外全部清零
void PID_Tick(PID_Tick_Typedef *PID_Tick_Structure);

#endif
