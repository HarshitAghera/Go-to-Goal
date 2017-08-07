#include "stm32f4xx_hal.h"
#include "Basic_Init.h"
#include "UART_STM32f4.h"
#include "GoToGoal.h"
#include "PID_MovingArray.h"



TIM_HandleTypeDef htim22,htim23,htim7;
UART_HandleTypeDef huart1 , huart3;
I2C_HandleTypeDef hi2c1;

extern volatile uint16_t Hd;
Go2Goal MainGoal;


int main(void)
{
	HAL_Init();

  SystemClock_Config();

	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_TIM2_CLK_ENABLE();
	__HAL_RCC_TIM7_CLK_ENABLE();
	__HAL_RCC_USART3_CLK_ENABLE();
	__HAL_RCC_USART1_CLK_ENABLE();
	__HAL_RCC_I2C1_CLK_ENABLE();
	
	GPIO_Init(GPIOA , GPIO_PIN_5 , GPIO_MODE_OUTPUT_PP , GPIO_NOPULL , GPIO_SPEED_FREQ_LOW);		
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_5,GPIO_PIN_SET);
	
	
	PWM_Init(&htim23 , TIM2 , TIM_CHANNEL_3 , 1024 , GPIOB , GPIO_PIN_10 , GPIO_AF1_TIM2);				//Right PWM
	PWM_Init(&htim22 , TIM2 , TIM_CHANNEL_2 , 1024 , GPIOB , GPIO_PIN_3 , GPIO_AF1_TIM2);				//Left PWM
	HAL_TIM_PWM_Start(&htim23 , TIM_CHANNEL_3);
	HAL_TIM_PWM_Start(&htim22 , TIM_CHANNEL_2);
	
	GPIO_Init(GPIOB , GPIO_PIN_4 , GPIO_MODE_OUTPUT_PP , GPIO_NOPULL , GPIO_SPEED_FREQ_LOW);		//Right Dir
	GPIO_Init(GPIOA , GPIO_PIN_10 , GPIO_MODE_OUTPUT_PP , GPIO_NOPULL , GPIO_SPEED_FREQ_LOW);		//Left Dir

	ExtInt_Init(GPIOB , GPIO_PIN_1 , GPIO_MODE_IT_RISING , GPIO_NOPULL , 0 ,0);									//Right Encoder
	ExtInt_Init(GPIOB , GPIO_PIN_2 , GPIO_MODE_IT_RISING , GPIO_NOPULL , 1 ,0);									//Left Encoder
	
	//GPIO_Init(GPIOA , GPIO_PIN_11 , GPIO_MODE_INPUT , GPIO_NOPULL , GPIO_SPEED_FREQ_VERY_HIGH);	//Right Encoder
	//GPIO_Init(GPIOA , GPIO_PIN_12 , GPIO_MODE_INPUT , GPIO_NOPULL , GPIO_SPEED_FREQ_VERY_HIGH);	//Left Encoder
	
	UART_Init(&huart3 , USART3 , 9600 , GPIOC , GPIO_PIN_10 , GPIO_PIN_11 , GPIO_AF7_USART3);
	//UART_InterruptEnable(&huart3 , USART3_IRQn , 4 , 0);
	
	//UART_Init(&huart1 , USART1 , 9600 , GPIOB , GPIO_PIN_6 , GPIO_PIN_7 , GPIO_AF7_USART1);
	//UART_InterruptEnable(&huart1 , USART1_IRQn , 5 , 0);

	I2C_Init(&hi2c1 , I2C1 , 400000 , GPIOB , GPIO_PIN_6 , GPIO_PIN_7 , GPIO_AF4_I2C1);
	HMC5883L_Init(&hi2c1);
	
	
	MainGoal.LeftWheel.Init(0.26,0.0,1.68);   //Set Kp,Ki and Kd for PIDs
  MainGoal.RightWheel.Init(0.26,0.0,1.65);
  MainGoal.CurPhi.Init(0.01745*5,0.0,0.0);	//No PID at 0.01745
 
    
  Go2Goal::LeftWheelSpeed.Init(10);
  Go2Goal::RightWheelSpeed.Init(10);
	
	MainGoal.desiredBotPosition.x = 00;
	MainGoal.desiredBotPosition.y = 200;
	
	MainGoal.Phi_Refernce = HMC5883L_GetHeadings(&hi2c1);
	MainGoal.curBotPosition.phi = 90;

	MainGoal.desiredDiffState.leftRPM = 50;
	MainGoal.desiredDiffState.rightRPM = 140;
	
	Timer_InterruptEnable(&htim7 , TIM7 , 32768 , TIM7_IRQn , 2 , 0);														//Interrupt Timer
		
	
	while(1)
	{
	
	}
	
}
	
	
	