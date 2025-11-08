#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"
#include "Serial.h"
#include "Timer.h"
#include "PID.h"
#include <string.h>
#include <stdlib.h>

#define MODE_POSITION_FOLLOW 0  // 位置跟随模式
#define MODE_SPEED_CONTROL 1    // 速度闭环控制模式

uint8_t KeyNum;

float Target = 0;          // 目标速度
float Actual, Out;
float Kp = 1.2f, Ki = 0.05f, Kd = 0.3f;  // 速度环PID参数（优化后）
float Error0, Error1, Error2;

char Serial_RxBuffer[20];  // 串口接收缓冲区
uint8_t Serial_RxIndex = 0;  // 接收索引
uint8_t Serial_RxComplete = 0;  // 接收完成标志

void Serial_ProcessCommand(void)
{
    if (Serial_RxComplete)
    {
        // 指令格式：@speed%123 或 @speed%-45
        if (strstr(Serial_RxBuffer, "@speed%") == Serial_RxBuffer)
        {
            char* numStr = Serial_RxBuffer + 6;  // 指向数字部分
            int32_t speed = atoi(numStr);
            
            // 限制速度范围（根据电机性能调整）
            if (speed > 300) speed = 300;
            if (speed < -300) speed = -300;
            
            Target = speed;  // 更新目标速度
            Serial_Printf("Set target speed: %d\r\n", (int)Target);
        }
        
        // 清空缓冲区和标志
        memset(Serial_RxBuffer, 0, sizeof(Serial_RxBuffer));
        Serial_RxIndex = 0;
        Serial_RxComplete = 0;
    }
}

int main(void)
{
	OLED_Init();
    Motor_Init();
    Key_Init();
    Encoder_Init();  // 初始化编码器
    Serial_Init();   // 初始化串口
    TIM2_Init();    // 初始化定时器2（10ms中断）
	
    while (1)
    {	
//		Mode_Switch();
//        Serial_ParseCommand();
//        OLED_UpdateDisplay();
//		Serial_ProcessCommand();
		
//		Kp = RP_GetValue(1) / 4095.0 * 2;
//		Ki = RP_GetValue(2) / 4095.0 * 2;
//		Kd = RP_GetValue(3) / 4095.0 * 2;
//		Target = RP_GetValue(4) / 4095.0 * 300 - 150;
		
//		OLED_Printf(0, 16, OLED_8X16, "Kp:%4.2f", Kp);
//		OLED_Printf(0, 32, OLED_8X16, "Ki:%4.2f", Ki);
//		OLED_Printf(0, 48, OLED_8X16, "Kd:%4.2f", Kd);
//		
//		OLED_Printf(64, 16, OLED_8X16, "Tar:%+04.0f", Target);
//		OLED_Printf(64, 32, OLED_8X16, "Act:%+04.0f", Actual);
//		OLED_Printf(64, 48, OLED_8X16, "Out:%+04.0f", Out);
//		
//		OLED_Update();
//		
//		Serial_Printf("%f,%f,%f\r\n", Target, Actual, Out);
		Motor_SetSpeed1(20);
	}
}

// 定时器2中断服务函数，每10ms执行一次
//void TIM2_IRQHandler(void)
//{
//    if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//    {
//		
//		static uint16_t Count;
//	
////		Key_Tick();
//			
//		Count ++;
//		if (Count >= 20)
//		{
//			Count = 0;
//				
//			Actual = Encoder_Get_TIM3();
//				
//			Error2 = Error1;
//			Error1 = Error0;
//			Error0 = Target - Actual;
//				
//			Out += Kp * (Error0 - Error1) + Ki * Error0
//					+ Kd * (Error0 - 2 * Error1 + Error2);
//				
//			if (Out > 100) {Out = 100;}
//			if (Out < -100) {Out = -100;}
//				
//			Motor_SetSpeed1((int8_t)Out);
//		}
//			
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//    }
//}

//void USART1_IRQHandler(void)
//{
//    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET)
//    {
//        uint8_t data = USART_ReceiveData(USART1);
//        
//        // 接收逻辑：遇到换行符视为指令结束
//        if (data == '\r' || data == '\n')
//        {
//            Serial_RxComplete = 1;
//        }
//        else if (Serial_RxIndex < sizeof(Serial_RxBuffer) - 1)
//        {
//            Serial_RxBuffer[Serial_RxIndex++] = data;
//        }
//        
//        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
//    }
//}
