#include "stm32f4xx_it.h"
using namespace std;
extern Go2Goal MainGoal;

struct Go2Goal::differentialState Go2Goal::curDiffState;		//Static Declaration
MovingArray Go2Goal::LeftWheelSpeed;
MovingArray Go2Goal::RightWheelSpeed;
long Go2Goal::ticks[2];

extern TIM_HandleTypeDef htim7;
extern TIM_HandleTypeDef htim2;
extern UART_HandleTypeDef huart3 , huart1;
extern I2C_HandleTypeDef hi2c1;


uint8_t i = 0;

void SysTick_Handler(void)
{
  HAL_IncTick();
  HAL_SYSTICK_IRQHandler();
}

void TIM7_IRQHandler(void)
{
	Go2Goal::calculateDiffState();
	MainGoal.calculatePos(HMC5883L_GetHeadings(&hi2c1));
	MainGoal.desiredDiffState = MainGoal.transformUniToDiff(MainGoal.getDesiredUnicycleState(MainGoal.curBotPosition,MainGoal.desiredBotPosition));
	//MainGoal.desiredDiffState.leftRPM = 50;
	//MainGoal.desiredDiffState.rightRPM = 140;
	MainGoal.getWheelOutputs(Go2Goal::curDiffState , MainGoal.desiredDiffState);
	
	ActuateLeftWheel(MainGoal.LeftWheelPWM.Dir , roundof(MainGoal.LeftWheelPWM.PWM));
	ActRightWheel(MainGoal.RightWheelPWM.Dir , roundof(MainGoal.RightWheelPWM.PWM));
	
	Send_Pos();
	//Send_WheelState();
	UART_NewLine(&huart3);
	__HAL_TIM_CLEAR_IT(&htim7, TIM_IT_UPDATE);    			//HAL_TIM_IRQHandler(&htim7);
	
}

void EXTI1_IRQHandler(void)					//Left Encoder
{
	Go2Goal::ticks[MainGoal.left]++;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1);
}

void EXTI2_IRQHandler(void)					//right Encoder
{
	Go2Goal::ticks[MainGoal.right]++;
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2);
}


int32_t roundof(float Num)
{
	int32_t temp = Num - (int)Num;
	
	if(temp > 0.5)
		return(Num + 0.5);
	else
		return(Num);
}


inline void Graph_Plot(void)
 {
	 UART_TransmitChar(&huart3 , 0xAB);
	 UART_TransmitChar(&huart3 , 0xCD);
	 UART_TransmitChar(&huart3 , 0x08);
	 UART_TransmitChar(&huart3 , 0x00);
	 
	 UART_TransmitChar(&huart3 , (int)MainGoal.curDiffState.leftRPM & 0x00FF);
	 UART_TransmitChar(&huart3 , (((int)MainGoal.curDiffState.leftRPM & 0xFF00) >> 8));
	 UART_TransmitChar(&huart3 , (int)MainGoal.curDiffState.rightRPM & 0x00FF);
	 UART_TransmitChar(&huart3 , (((int)MainGoal.curDiffState.rightRPM & 0xFF00) >> 8));
	 
	 UART_TransmitChar(&huart3 , (int)MainGoal.desiredDiffState.leftRPM & 0x00FF);
	 UART_TransmitChar(&huart3 , (((int)MainGoal.desiredDiffState.leftRPM & 0xFF00) >> 8));
	 UART_TransmitChar(&huart3 , (int)MainGoal.desiredDiffState.rightRPM & 0x00FF);
	 UART_TransmitChar(&huart3 , (((int)MainGoal.desiredDiffState.rightRPM & 0xFF00) >> 8));
	 
	 /*UART_TransmitChar(&huart3 , (int)MainGoal.curBotPosition.y & 0x00FF);
	 UART_TransmitChar(&huart3 , (((int)MainGoal.curBotPosition.y & 0xFF00) >> 8));
	 UART_TransmitChar(&huart3 , (int)MainGoal.curBotPosition.phi & 0x00FF);
	 UART_TransmitChar(&huart3 , (((int)MainGoal.curBotPosition.phi & 0xFF00) >> 8));*/
 }
 

inline void Send_WheelState(void)
{
	UART_TransmitChar(&huart3 , 'r');
	UART_TransmitInteger(&huart3 , Go2Goal::curDiffState.rightRPM);
	UART_TransmitChar(&huart3 , 'l');
	UART_TransmitInteger(&huart3 , Go2Goal::curDiffState.leftRPM);
	UART_TransmitChar(&huart3 , 'r');
	UART_TransmitInteger(&huart3 , MainGoal.LeftWheelPWM.PWM);
	UART_TransmitChar(&huart3 , 'l');
	UART_TransmitInteger(&huart3 , MainGoal.RightWheelPWM.PWM);
}

inline void Send_Pos(void)
{
	UART_TransmitChar(&huart3 , 'x');
	UART_TransmitInteger(&huart3 , MainGoal.curBotPosition.x);
	UART_TransmitChar(&huart3 , 'y');
	UART_TransmitInteger(&huart3 , MainGoal.curBotPosition.y);
	UART_TransmitChar(&huart3 , 'p');
	UART_TransmitInteger(&huart3 , MainGoal.curBotPosition.phi);
}
inline void Send_Uni(void)
{
	
}
