#ifndef __UI_H
#define __UI_H
#include "stm32f10x.h"                  // Device header

typedef struct {
    char line1[16];
    char line2[16];
    char line3[16];
    char line4[16];
    uint8_t Num;
    uint8_t cursor;
    uint8_t cursor0;
    uint8_t exist_title;
} UI_typedef;

extern UI_typedef UI_root;
extern UI_typedef UI_start;
extern UI_typedef UI_PID;
extern UI_typedef UI_test;
extern UI_typedef UI_target;
extern UI_typedef UI_sensor;
extern UI_typedef UI_serial;
extern UI_typedef UI_go;


void UI_Init(void);
void UI_Show_PID(float Kp, float Ki, float Kd);
void UI_Show(UI_typedef *UI_Structure);
void UI_Show_Edit_Mode(uint8_t Mode);
void UI_Reset_Cursor(UI_typedef *UI_Structure);
void UI_MoveDown_Cursor(UI_typedef *UI_Structure);
void UI_MoveUp_Cursor(UI_typedef *UI_Structure);
uint8_t UI_GetMaxLine(UI_typedef *UI_Structure);
void UI_PID_Show_Motor_Num(uint8_t Motor_Num);
void UI_Show_test(int Target1, int Target2);
void UI_Show_test_PID_Struct(PID_Data_Typedef *pPID_Data_Structure1, PID_Data_Typedef *pPID_Data_Structure2, int Step);
void UI_test_Show_Motor_Num(uint8_t Motor_Num);
void UI_Show_Start(int Target1, int Target2);
void UI_Show_Sensor(uint8_t SensorData[5]);
void UI_Show_Serial(uint8_t Serial_Mode);


#endif
