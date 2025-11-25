#include "stm32f10x.h"

#define SENSOR_ID_1     ((uint8_t)1)
#define SENSOR_ID_2     ((uint8_t)2)
#define SENSOR_ID_3     ((uint8_t)3)
#define SENSOR_ID_4     ((uint8_t)4)
#define SENSOR_ID_5     ((uint8_t)5)

uint8_t Sensor_Data_Bit[5];

/**
 * @brief 传感器初始化
 * @retval 无
 */
void Sensor_Init(void)
{
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3 | GPIO_Pin_4 | GPIO_Pin_5;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_15;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
}

/**
 * @brief 获取传感器1输入位
 * @retval 0或1
 */
uint8_t Sensor1_GetState(void)
{
    uint8_t temp = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12);
    return temp;
}

/**
 * @brief 获取传感器2输入位
 * @retval 0或1
 */
uint8_t Sensor2_GetState(void)
{
    uint8_t temp = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_15);
    return temp;
}

/**
 * @brief 获取传感器3输入位
 * @retval 0或1
 */
uint8_t Sensor3_GetState(void)
{
    uint8_t temp = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3);
    return temp;
}

/**
 * @brief 获取传感器4输入位
 * @retval 0或1
 */
uint8_t Sensor4_GetState(void)
{
    uint8_t temp = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_4);
    return temp;
}

/**
 * @brief 获取传感器5输入位
 * @retval 0或1
 */
uint8_t Sensor5_GetState(void)
{
    uint8_t temp = GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_5);
    return temp;
}

/**
 * @brief 获取指定位置的位
 * @param Sensor_id 传感器编号，可以是SENSOR_ID_1，SENSOR_ID_2，SENSOR_ID_3，SENSOR_ID_4，SENSOR_ID_4
 * @retval 0或1
 */
uint8_t Sensor_GetState(uint8_t Sensor_id)
{
    if (Sensor_id == SENSOR_ID_1)
    {
        return Sensor1_GetState();
    }
    else if (Sensor_id == SENSOR_ID_2)
    {
        return Sensor2_GetState();
    }
    else if (Sensor_id == SENSOR_ID_3)
    {
        return Sensor3_GetState();
    }
    else if (Sensor_id == SENSOR_ID_4)
    {
        return Sensor4_GetState();
    }
    else if (Sensor_id == SENSOR_ID_5)
    {
        return Sensor5_GetState();
    }
    return 0;
}

/**
 * @brief 固定时间更新一次传感器数据
 * @retval 无
 */
void Sensor_Tick(void)
{
    static uint16_t count = 0;
    count ++;
    if (count >= 20)
    {
        Sensor_Data_Bit[0] = Sensor1_GetState();
        Sensor_Data_Bit[1] = Sensor2_GetState();
        Sensor_Data_Bit[2] = Sensor3_GetState();
        Sensor_Data_Bit[3] = Sensor4_GetState();
        Sensor_Data_Bit[4] = Sensor5_GetState();
        count = 0;
    }
}
