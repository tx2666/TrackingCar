#include "stm32f10x.h"                  // Device header
#include "Delay.h"

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

uint8_t Prev_KeyNum[KEY_COUNT] =    {0x0000};
uint8_t KeyNum[KEY_COUNT] =         {0x0000};
/**
 * 规定:
 * 0x0000 无按键按下        0000 0000
 * 0x0001 按下瞬间          0000 0001
 * 0x0002 松开瞬间          0000 0010
 * 0x0004 双击              0000 0100
 * 0x0008 三击              0000 1000
 * 0x0010 按下时            0001 0000
 * 0x0020 长按              0010 0000
 */

void Key_Init(void) 
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);       // 启用GPIOB外部时钟

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;               // 上拉输入模式
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

}
/**
 * @brief 置标志位
 * @param Key 要处理的按键.
 * 可以是这几个: KEY_UP, KEY_DOWN, KEY_CONFIRM, KEY_CANCEL
 * @param KeyFlag 要处理的标志位.
 * 可以是这几个: KEY_NONE, KEY_PRESS, KEY_RELEASE, KEY_DOUBLE, 
 *              KEY_TRINPLE, KEY_PRESSING, KEY_LONG
 * @retval 无
 */
void Key_SetFlag(uint8_t Key, uint8_t KeyFlag)
{
    KeyNum[Key] = KeyNum[Key] | KeyFlag;
}

/**
 * @brief 清除标志位
 * @param Key 要处理的按键.
 * 可以是这几个: KEY_UP, KEY_DOWN, KEY_CONFIRM, KEY_CANCEL
 * @param KeyFlag 要处理的标志位.
 * 可以是这几个: KEY_NONE, KEY_PRESS, KEY_RELEASE, KEY_DOUBLE, 
 *              KEY_TRINPLE, KEY_PRESSING, KEY_LONG
 * @retval 无
 */
void Key_ClearFlag(uint8_t Key, uint8_t KeyFlag)
{
    KeyNum[Key] = KeyNum[Key] & (~KeyFlag);
    // 逻辑：按位取反之后只有要置0的位置是0，其它都是1，这样那个位置就变成0了
}

/**
 * @brief 获取标志位状态，并清空标志位
 * @param Key 要处理的按键.
 * 可以是这几个: KEY_UP, KEY_DOWN, KEY_CONFIRM, KEY_CANCEL
 * @param KeyFlag 要处理的标志位.
 * 可以是这几个: KEY_NONE, KEY_PRESS, KEY_RELEASE, KEY_DOUBLE, 
 *              KEY_TRINPLE, KEY_LONG
 * @retval 1或者0；1表示该标志位置1
 */
uint8_t Key_GetState(uint8_t Key, uint8_t KeyFlag)
{
    if (KeyNum[Key] & KeyFlag != 0x0)
    {
        if (KeyFlag == KEY_PRESSING)
            return 1;
        // 防止小天才输入KEY_PRESSING导致检测部分出问题
        Key_ClearFlag(Key, KeyFlag);
        return 1;
    }
    return 0;
}

void Key_Tick(void)
{
    static uint16_t count[KEY_COUNT][4] = {0};
    /**
     * 说明
     * count[0][0] 用于扫描
     * count[ ][1] 用于测量按下时长
     * count[ ][2] 用于测量松开后持续时间
     * count[ ][3] 用于计次数判断多次点击
     * 
     * 第一个下标是按钮根据按钮来分配
     * 0 0 专门用来总计时
     */
    if (count[0][0] >= 20)
    {    
        for (int i = 0; i < KEY_COUNT; i++)
        {
            Prev_KeyNum[i] = KeyNum[i];
        }
        /* 检测状态部分 */
        /* UP */
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0)
        {
            Key_SetFlag(KEY_UP, KEY_PRESSING);
        }
        else
        {
            Key_ClearFlag(KEY_UP, KEY_PRESSING);
        }
        /* Down */
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) == 0)
        {
            Key_SetFlag(KEY_DOWN, KEY_PRESSING);
        }
        else
        {
            Key_ClearFlag(KEY_DOWN, KEY_PRESSING);
        }
        /* Confirm */
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_14) == 0)
        {
            Key_SetFlag(KEY_CONFIRM, KEY_PRESSING);
        }
        else
        {
            Key_ClearFlag(KEY_CONFIRM, KEY_PRESSING);
        }
        /* Cancel */
        if (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_15) == 0)
        {
            Key_SetFlag(KEY_CANCEL, KEY_PRESSING);
        }
        else
        {
            Key_ClearFlag(KEY_CANCEL, KEY_PRESSING);
        }
        /* 判断部分 */
        for (int i = 0; i < KEY_COUNT; i++)
        {
            /* 刚按下 */
            if      (Prev_KeyNum[i] & KEY_PRESSING == 0x0 && KeyNum[i] & KEY_PRESSING != 0x0)
            {
                // 上一次按钮pressing为0，这一次为1时，认为按钮刚刚按下
                // 只有pressing置0时，第一个表达式的值才为0x0
                Key_SetFlag(i, KEY_PRESS);
                count[i][2] = 0;
            }
            /* 按住 */
            else if (Prev_KeyNum[i] & KEY_PRESSING != 0x0 && KeyNum[i] & KEY_PRESSING != 0x0)
            {
                count[i][1] += 20;
                if (count[1] >= 5*20)
                {
                    // 时长大于100ms，置一次标志位
                    Key_SetFlag(i, KEY_LONG);
                    count[i][1] = 0;
                } 
            }
            else if (Prev_KeyNum[i] & KEY_PRESSING != 0x0 && KeyNum[i] & KEY_PRESSING == 0x0)
            {
                // 上一次为1，这一次为0
                count[i][1] = 0;  // 松开之后，长按计数归零
                count[i][3] ++;  // 松开之后，连续点击次数加1
                Key_SetFlag(i, KEY_RELEASE);
                count[i][2] += 20;  // 松开时间的计数器开始计时
                // 由于是每20ms触发一次，所以一次增加20
            }

            if (count[i][2] > 0 && KeyNum[i] & KEY_PRESSING == 0x0)
            {
                count[i][2] += 20;
                if (count[i][2] >= 20*5)
                {
                    if (count[i][3] >= 3)               // 3连
                    {
                        Key_SetFlag(i, KEY_TRINPLE);
                    }
                    else if (count[i][3] >= 2)          // 2连
                    {
                        Key_SetFlag(i, KEY_DOUBLE);
                    }
                    else if (count[i][3] >= 1)
                    {
                        Key_SetFlag(i, KEY_PRESS);
                    }
                    
                    count[i][2] = 0;
                }
            }
        }
        
        count[0][0] = 0;
    }
}
