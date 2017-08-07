#ifndef __STM32F4xx_IT_H
#define __STM32F4xx_IT_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx.h"
#include "Basic_Init.h"
#include "UART_STM32f4.h"
#include "GoToGoal.h"
#include "PID_MovingArray.h"

#ifdef __cplusplus
extern "C" 
{ 

void SysTick_Handler(void);
void TIM7_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);

int32_t roundof(float Num);
	
inline void Graph_Plot(void);
inline void Send_WheelState(void);
inline void Send_Pos(void);
inline void Send_Uni(void);
	 
}
#endif

#endif /* __STM32F4xx_IT_H */
