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

void UI_Init(void);
void Show_PID(float Kp, float Ki, float Kd);
void Show_UI(UI_typedef *UI_Structure);

#endif
