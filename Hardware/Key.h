#ifndef __KEY_H
#define __KEY_H

#define KEY_NONE            0x0000
#define KEY_PRESS           0x0001
#define KEY_RELEASE         0x0002
#define KEY_DOUBLE          0x0004
#define KEY_TRINPLE         0x0008
#define KEY_PRESSING        0x0010
#define KEY_LONG            0x0020

#define KEY_COUNT 4

#define KEY_UP              0
#define KEY_DOWN            1
#define KEY_CONFIRM         2
#define KEY_CANCEL          3

void Key_Init(void);
void Key_SetFlag(uint8_t Key, uint8_t KeyFlag);
void Key_ClearFlag(uint8_t Key, uint8_t KeyFlag);
uint8_t Key_GetState(uint8_t Key, uint8_t KeyFlag);
void Key_Tick(void);


#endif
