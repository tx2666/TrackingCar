#include "stm32f10x.h"                  // Device header
#include "OLED.h"
#include <string.h>

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

UI_typedef root;
UI_typedef start;
UI_typedef PID;

void UI_Init(void)
{
    OLED_Init();
    /* 主菜单 */
    root.exist_title = 0;
    strcpy(root.line1, "Start");
    strcpy(root.line2, "PID");
    strcpy(root.line3, "");
    strcpy(root.line4, "");
    root.cursor  = 1;
    root.cursor0 = 1;
    root.Num = 0;
    /* Start菜单 */
    start.exist_title = 1;
    strcpy(start.line1, "Start");
    strcpy(start.line2, "");
    strcpy(start.line3, "");
    strcpy(start.line4, "");
    start.cursor  = 2;
    start.cursor0 = 2;
    start.Num = 1;
    /* PID */
    start.exist_title = 1;
    strcpy(PID.line1, "PID");
    strcpy(PID.line2, "Kp");
    strcpy(PID.line3, "Ki");
    strcpy(PID.line4, "Kd");
    PID.cursor  = 2;
    PID.cursor0 = 2;
    PID.Num = 2;
}

/**
  * @brief PID菜单下的参数显示函数
  * @param Kp 
  * @param Ki 
  * @param Kd
  * @retval 无
  */
void Show_PID(float Kp, float Ki, float Kd)
{

}

/**
  * @brief 刷新页面
  * @param UI_Structure 界面文字信息的存储函数
  * @retval 无
  */
void Show_UI(UI_typedef *UI_Structure)
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

