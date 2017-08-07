#ifndef Basic_Init
#define Basic_Init

#define F_CPU 				84			//In MHz
#define HMC5883L_Adrr 0x3C
#include "stm32f4xx_hal.h"
#include "stdio.h"
#include "math.h"

void SystemClock_Config(void);
void GPIO_Init(GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin , uint32_t GPIO_Mode , uint8_t GPIO_Pull , uint8_t GPIO_Speed);
void PWM_Init(TIM_HandleTypeDef *htim , TIM_TypeDef *TIM , uint8_t Channel , uint64_t PulseWidth , 
							GPIO_TypeDef  *GPIOx , uint16_t GPIO_Pin , uint8_t Alter_Func);
void PWM_SetDutyCycle(TIM_HandleTypeDef *htim , uint8_t Channel , float DutyCycle);
void Timer_InterruptEnable(TIM_HandleTypeDef *htim , TIM_TypeDef *TIM , uint32_t InterruptTime_us , IRQn_Type IRQ , uint8_t PreemptPriority , uint8_t SubPriority);
void SYSTICKS_Init(void);
void ExtInt_Init(GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin , uint32_t GPIO_Mode , uint8_t GPIO_Pull , uint8_t PreemptPriority , uint8_t SubPriority);
void _Error_Handler(void);
void ActRightWheel(bool dir,uint16_t PWM);
void ActuateLeftWheel(bool dir,uint16_t PWM);

void I2C_Init(I2C_HandleTypeDef *hi2c , I2C_TypeDef *I2C , uint32_t ClockSpeed , GPIO_TypeDef  *GPIOx , 
							 uint16_t GPIO_Pin_SCL , uint16_t GPIO_Pin_SDA , uint8_t Alter_Func);
void HMC5883L_Init(I2C_HandleTypeDef *hi2c);
float HMC5883L_GetHeadings(I2C_HandleTypeDef *hi2c);
#endif

