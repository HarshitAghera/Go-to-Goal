#include "UART_STM32f4.h"

void UART_Init(UART_HandleTypeDef *huart , USART_TypeDef *uart , uint32_t BaudRate , GPIO_TypeDef  *GPIOx , 
							 uint16_t GPIO_Pin_Tx , uint16_t GPIO_Pin_Rx , uint8_t Alter_Func)
{
	huart->Instance = uart;
  huart->Init.BaudRate = BaudRate;
  huart->Init.WordLength = UART_WORDLENGTH_8B;
  huart->Init.StopBits = UART_STOPBITS_1;
  huart->Init.Parity = UART_PARITY_NONE;
  huart->Init.Mode = UART_MODE_TX_RX;
  huart->Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart->Init.OverSampling = UART_OVERSAMPLING_8;
	
  HAL_UART_Init(huart);
  __HAL_UART_ENABLE(huart);
	GPIO_InitTypeDef GPIO_InitStruct;
 
  GPIO_InitStruct.Pin = GPIO_Pin_Tx ;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = Alter_Func;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_Pin_Rx ;
	  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
	
void UART_InterruptEnable(UART_HandleTypeDef *huart , IRQn_Type IRQ , uint8_t PreemptPriority , uint8_t SubPriority )
{
	HAL_NVIC_EnableIRQ(IRQ);
	HAL_NVIC_SetPriority(IRQ,PreemptPriority,SubPriority);
	__HAL_UART_ENABLE_IT(huart , UART_IT_RXNE);
}

void UART_TransmitChar(UART_HandleTypeDef *huart , uint8_t c)
{
	while(huart->gState == HAL_UART_STATE_BUSY_TX);
	HAL_UART_Transmit(huart , &c , 1 , TimeOut);
}

void UART_TransmitString(UART_HandleTypeDef *huart , char *c)
{
	//uint8_t size = std::strlen(c);
	//while(huart->gState == HAL_UART_STATE_BUSY_TX);
	//HAL_UART_Transmit(huart ,  (uint8_t *)c , size , TimeOut*size);
}

void UART_TransmitInteger(UART_HandleTypeDef *huart , int32_t Num)
{
	/*std::stringstream Str;
	std::string NumStr;
	char s[20];
																												//Increase Flash Size So..... Just..... Fuck it
	Str << Num;
	NumStr = Str.str();
	std::strcpy(s , NumStr.c_str());
	
	UART_TransmitString( huart , (uint8_t *)s);
	
	*/
	
	if(Num<0)
	{
		UART_TransmitChar(huart , '-');
		Num=(-1)*Num;
	}
	
	if(Num >= 10)
	{
    UART_TransmitInteger(huart , Num/10);
    Num = Num%10;
  }
  UART_TransmitChar(huart , Num+'0'); 
}

void UART_TransmitFloat(UART_HandleTypeDef *huart , float Num)
{
	UART_TransmitInteger(huart , Num);
	
	int32_t temp = (Num - (int)Num)*10000;
	
	UART_TransmitChar(huart , '.');
	
	if(temp<0)
		temp *= -1;
	
	if(temp<1000)
	{
		UART_TransmitChar(huart , '0');
		if(temp<100)
		{
			UART_TransmitChar(huart , '0');
			if(temp<10)
			{
				UART_TransmitChar(huart , '0');
			}
		}
	}
	
	UART_TransmitInteger(huart , temp);
}
	

void UART_NewLine(UART_HandleTypeDef *huart)
{
	UART_TransmitChar(huart , 0x0D);
}


uint8_t UART_ReceiveChar(UART_HandleTypeDef *huart)
{
	uint8_t c;
	HAL_UART_Receive(huart , &c , 1 , TimeOut);
	return c;
}


void UART_ReceiveString(UART_HandleTypeDef *huart , uint8_t *c , uint8_t size)
{
	HAL_UART_Receive(huart , c , size , size*TimeOut);
}

