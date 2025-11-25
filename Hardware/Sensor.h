#ifndef __SENSOR_H
#define __SENSOR_H

#include "stm32f10x.h"

extern uint8_t Sensor_Data_Bit[5];

void Sensor_Init(void);
uint8_t Sensor1_GetState(void);
uint8_t Sensor2_GetState(void);
uint8_t Sensor3_GetState(void);
uint8_t Sensor4_GetState(void);
uint8_t Sensor5_GetState(void);
uint8_t Sensor_GetState(uint8_t Sensor_id);
void Sensor_Tick(void);



#endif
