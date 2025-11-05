#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Motor.h"
#include "Key.h"
#include "Encoder.h"

uint8_t KeyNum;
int8_t Speed; 

int main(void)
{
	OLED_Init();
	Motor_Init();
	Key_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	
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
		OLED_ShowSignedNum(1, 7, Speed, 3);
	}
}

void TIM2_IRQHandler(void)
{
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)		//判断是否是TIM2的更新事件触发的中断
	{
		Speed = Encoder_Get_TIM3();						    //每隔固定时间段读取一次编码器计数增量值，即为速度值
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);			//清除TIM2更新事件的中断标志位
															//中断标志位必须清除
															//否则中断将连续不断地触发，导致主程序卡死
	}
}
