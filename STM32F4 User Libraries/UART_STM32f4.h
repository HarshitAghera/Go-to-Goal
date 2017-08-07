#ifndef UART_STM32f4
#define UART_STM32f4

#include <string.h>
#include "stm32f4xx_hal.h"

#define TimeOut 10

void UART_Init(UART_HandleTypeDef *huart , USART_TypeDef *uart , uint32_t BaudRate , GPIO_TypeDef  *GPIOx , 
							 uint16_t GPIO_Pin_Tx , uint16_t GPIO_Pin_Rx , uint8_t Alter_Func);
void UART_InterruptEnable(UART_HandleTypeDef *huart , IRQn_Type IRQ , uint8_t PreemptPriority , uint8_t SubPriority );
void UART_NewLine(UART_HandleTypeDef *huart);

void UART_TransmitChar(UART_HandleTypeDef *huart , uint8_t c);
void UART_TransmitString(UART_HandleTypeDef *huart , char *c);
void UART_TransmitInteger(UART_HandleTypeDef *huart , int32_t Num);
void UART_TransmitFloat(UART_HandleTypeDef *huart , float Num);

uint8_t UART_ReceiveChar(UART_HandleTypeDef *huart);
void UART_ReceiveString(UART_HandleTypeDef *huart , uint8_t *c , uint8_t size);

#endif