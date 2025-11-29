#ifndef __MOTOR_H
#define __MOTOR_H

extern uint16_t Target_Speed;

void Motor_Init(void);
void Motor1_SetSpeed(int16_t Speed);
void Motor2_SetSpeed(int16_t Speed);


#endif
