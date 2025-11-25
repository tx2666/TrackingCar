#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include <string.h>
#include <math.h>
#include "PID.h"

typedef struct {
    char line1[16];
    char line2[16];
    char line3[16];
    char line4[16];
    uint8_t Num;
    uint8_t cursor;
    uint8_t cursor0;
    uint8_t default_cursor;
    uint8_t exist_title;
} UI_typedef;

UI_typedef UI_root;
UI_typedef UI_start;
UI_typedef UI_PID;
UI_typedef UI_test;
UI_typedef UI_target;
UI_typedef UI_sensor;

/**
 * @brief 获取UI结构体含有字符串的行数
 * @param UI_Structure UI结构体
 * @retval uint8_t
 */
uint8_t UI_GetMaxLine(UI_typedef *UI_Structure)
{
    if (UI_Structure->line1[0] != '\0')
    {
        if (UI_Structure->line2[0] != '\0')
        {
            if (UI_Structure->line3[0] != '\0')
            {
                if (UI_Structure->line4[0] != '\0')
                {
                    return 4;
                }
                return 3;
            }
            return 2;
        }
        return 1;
    }
    return 0;
}

/**
 * @brief UI初始化函数
 * @retval 无
 */
void UI_Init(void)
{
    OLED_Init();
    /* 主菜单 0 */
    UI_root.exist_title = 0;
    strcpy(UI_root.line1, "Start");
    strcpy(UI_root.line2, "PID");
    strcpy(UI_root.line3, "Test");
    strcpy(UI_root.line4, "");
    UI_root.default_cursor = 1;
    UI_root.cursor  = 1;
    UI_root.cursor0 = 1;
    UI_root.Num = 0;
    /* Start 1 */
    UI_start.exist_title = 1;
    strcpy(UI_start.line1, "Start");
    strcpy(UI_start.line2, "Press to start");
    strcpy(UI_start.line3, "");
    strcpy(UI_start.line4, "");
    UI_start.default_cursor = 2;
    UI_start.cursor  = 2;
    UI_start.cursor0 = 2;
    UI_start.Num = 1;
    /* PID 2 */
    UI_PID.exist_title = 0;
    strcpy(UI_PID.line1, "PID");
    strcpy(UI_PID.line2, "Kp");
    strcpy(UI_PID.line3, "Ki");
    strcpy(UI_PID.line4, "Kd");
    UI_PID.default_cursor = 1;
    UI_PID.cursor  = 1;
    UI_PID.cursor0 = 1;
    UI_PID.Num = 2;
    /* Test 3 */
    UI_test.exist_title = 1;
    strcpy(UI_test.line1, "Test");
    strcpy(UI_test.line2, "Target");
    strcpy(UI_test.line3, "Sensor");
    strcpy(UI_test.line4, "");
    UI_test.default_cursor = 1;
    UI_test.cursor  = 1;
    UI_test.cursor0 = 1;
    UI_test.Num = 3;
    /* Target 4 */
    UI_target.exist_title = 1;
    strcpy(UI_target.line1, "Target");
    strcpy(UI_target.line2, "Tar1");
    strcpy(UI_target.line3, "Tar2");
    strcpy(UI_target.line4, "Step");
    UI_target.default_cursor = 2;
    UI_target.cursor  = 2;
    UI_target.cursor0 = 2;
    UI_target.Num = 4;
    /* Sensor 5 */
    UI_sensor.exist_title = 0;
    strcpy(UI_sensor.line1, "Sensor");
    strcpy(UI_sensor.line2, "");
    strcpy(UI_sensor.line3, "");
    strcpy(UI_sensor.line4, "");
    UI_sensor.default_cursor = 1;
    UI_sensor.cursor  = 1;
    UI_sensor.cursor0 = 1;
    UI_sensor.Num = 5;
}

/**
 * @brief 传感器数据展示界面下显示传感器状态
 * @retval 无
 */
void UI_Show_Sensor(uint8_t *SensorData)
{
    for (int i = 0; i < 5; i++)
    {
        OLED_ShowNum(3, 2+2*i, SensorData[i],1);
    }
}

/**
  * @brief PID菜单下的参数显示函数
  * @param Kp 比例项系数
  * @param Ki 积分项系数
  * @param Kd 微分项系数
  * @retval 无
  */
void UI_Show_PID(float Kp, float Ki, float Kd)
{
    int start_pos = 8;
    if (Kp < 0) OLED_ShowChar(2, start_pos, '-');
    else OLED_ShowChar(2, start_pos, '+');
    OLED_ShowNum(2, start_pos+1, fabs((int)Kp), 2);
    OLED_ShowChar(2, start_pos+3, '.');
    OLED_ShowNum(2, start_pos+4, (int)(fabs((Kp-(int)Kp)*100)), 2);

    if (Ki < 0) OLED_ShowChar(3, start_pos, '-');
    else OLED_ShowChar(3, start_pos, '+');
    OLED_ShowNum(3, start_pos+1, fabs((int)Ki), 2);
    OLED_ShowChar(3, start_pos+3, '.');
    OLED_ShowNum(3, start_pos+4, (int)(fabs((Ki-(int)Ki)*100)), 2);

    if (Kd < 0) OLED_ShowChar(4, start_pos, '-');
    else OLED_ShowChar(4, start_pos, '+');
    OLED_ShowNum(4, start_pos+1, fabs((int)Kd), 2);
    OLED_ShowChar(4, start_pos+3, '.');
    OLED_ShowNum(4, start_pos+4, (int)(fabs((Kd-(int)Kd)*100)), 2);
}

/**
 * @brief test测试界面的数据展示
 * @param Target1 目标1
 * @param Target2 目标2
 * @retval 无
 */
void UI_Show_test(int Target1, int Target2, int Step)
{
    OLED_ShowSignedNum(2, 8, Target1, 3);
    OLED_ShowSignedNum(3, 8, Target2, 3);
    OLED_ShowNum(4, 9, Step, 2);
}

/**
 * @brief UI_Show_test的封装版
 */
void UI_Show_test_PID_Struct(PID_Data_Typedef *pPID_Data_Structure1, PID_Data_Typedef *pPID_Data_Structure2, int Step)
{
    UI_Show_test(pPID_Data_Structure1->Target, pPID_Data_Structure2->Target, Step);
}

/**
 * @brief PID参数调节界面，电机编号展示
 * @param Motor_Num 电机编号
 * @retval 无
 */
void UI_PID_Show_Motor_Num(uint8_t Motor_Num)
{
    OLED_ShowChar(1, 5, '0'+Motor_Num);
}

/**
 * @brief test界面，电机编号展示
 * @param Motor_Num 电机编号
 * @retval 无
 */
void UI_test_Show_Motor_Num(uint8_t Motor_Num)
{
    OLED_ShowChar(1, 6, '0'+Motor_Num);
}

/**
 * @brief 显示编辑模式是否启用
 * @param Mode 1时显示Edit，0时清空Edit字符串
 * @retval 无
 */
void UI_Show_Edit_Mode(uint8_t Mode)
{
    if (Mode == 1)
    {
        OLED_ShowString(1, 13, "Edit");
    }
    else if (Mode == 0)
    {
        OLED_ShowString(1, 13, "    ");
    }
}

void UI_Reset_Cursor(UI_typedef *UI_Structure)
{
    UI_Structure->cursor0 = UI_Structure->cursor;
    UI_Structure->cursor = UI_Structure->default_cursor;
}

/**
 * @brief 向下移动光标
 * @param UI_Structure 界面文字信息的存储变量
 * @retval 无
 */
void UI_MoveDown_Cursor(UI_typedef *UI_Structure)
{
    UI_Structure->cursor0 = UI_Structure->cursor;
    UI_Structure->cursor++;
    if (UI_Structure->cursor > UI_GetMaxLine(UI_Structure))
    {
        if (UI_Structure->exist_title == 1)
        {
            UI_Structure->cursor = 2;
        }
        else 
        {
            UI_Structure->cursor = 1;
        }
    }
}

/**
 * @brief 向上移动光标
 * @param UI_Structure 界面文字信息的存储变量
 * @retval 无
 */
void UI_MoveUp_Cursor(UI_typedef *UI_Structure)
{
    UI_Structure->cursor0 = UI_Structure->cursor;
    UI_Structure->cursor--;
    if (UI_Structure->exist_title == 1)
    {
        if (UI_Structure->cursor <= 1)
        {
            UI_Structure->cursor = UI_GetMaxLine(UI_Structure);
        }
    }
    else 
    {
        if (UI_Structure->cursor <= 0)
        {
            UI_Structure->cursor = UI_GetMaxLine(UI_Structure);
        }
    }
}
/**
  * @brief 刷新页面
  * @param UI_Structure 界面文字信息的存储变量
  * @retval 无
  */
void UI_Show(UI_typedef *UI_Structure)
{
    if (UI_Structure->exist_title == 1)                 // 如果这个UI存在标题
    {
        OLED_ShowString(1, 1, UI_Structure->line1);     // 第一行顶格输出
    }
    else
    {
        OLED_ShowString(1, 2, UI_Structure->line1);     // 第一行空一格输出
    }
    OLED_ShowString(2, 2, UI_Structure->line2);         // 输出第二行
    OLED_ShowString(3, 2, UI_Structure->line3);         // 输出第三行
    OLED_ShowString(4, 2, UI_Structure->line4);         // 输出第四行

    if (UI_Structure->exist_title == 1)                 // 如果这个UI存在标题
    {
        if (UI_Structure->cursor <= 1)                  // 如果游标位置小于等于1
            UI_Structure->cursor = 2;                   // 则让游标移到二的位置
    }
    
    if (UI_Structure->cursor != UI_Structure->cursor0)  // 如果光标位置和上一次不一样
    {
        OLED_ShowChar(UI_Structure->cursor0, 1, ' ');   // 清空上一次光标
        UI_Structure->cursor0 = UI_Structure->cursor;
    }
    OLED_ShowChar(UI_Structure->cursor,  1, '>');       // 显示当前光标
}

