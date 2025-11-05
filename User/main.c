#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"
#include "Serial.h"
#include "Timer.h"
#include "PID.h"

uint8_t KeyNum;
int16_t Speed;
int16_t EncoderCnt;
int16_t TargetSpeed = 0;  // 目标速度
int16_t ActualSpeed;      // 实际速度
PID_t SpeedPID;           // 速度环PID结构体


int main(void)
{
	OLED_Init();
    Motor_Init();
    Key_Init();
    Encoder_Init();  // 初始化编码器
    Serial_Init();   // 初始化串口
    TIM2_Init();    // 初始化定时器2（10ms中断）
    TIM3_Init();
	
    while (1)
    {
        KeyNum = Key_GetNum();
        if (KeyNum == 1)
        {
            Speed += 20;
            if (Speed > 100)
            {
                Speed = -100;
            }
        }
        Motor_SetSpeed1(Speed);
    }
}

// 定时器2中断服务函数，每10ms执行一次
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
    {
        // 读取TIM3编码器的计数
        EncoderCnt = Encoder_Get_TIM3();
        
        // 通过串口发送编码器数值到上位机
        Serial_Printf("Encoder Count: %d\r\n", EncoderCnt);
        
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}
