#include "Basic_Init.h"

void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

    /**Configure the main internal regulator output voltage 
    */
  __HAL_RCC_PWR_CLK_ENABLE();

  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    _Error_Handler();
  }

    /**Initializes the CPU, AHB and APB busses clocks 
    */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    _Error_Handler();
		
  }
}

void GPIO_Init(GPIO_TypeDef  *GPIOx , uint16_t GPIO_Pin , uint32_t GPIO_Mode , uint8_t GPIO_Pull , uint8_t GPIO_Speed)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_Mode;
  GPIO_InitStruct.Pull = GPIO_Pull;
  GPIO_InitStruct.Speed = GPIO_Speed;
	
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}
 
void PWM_Init(TIM_HandleTypeDef* htim , TIM_TypeDef* TIM , uint8_t Channel , uint64_t PulseWidth , 
									GPIO_TypeDef  *GPIOx , uint16_t GPIO_Pin , uint8_t Alter_Func)
{
	
  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  TIM_OC_InitTypeDef sConfigOC;
	
  htim->Instance = TIM;
  htim->Init.Prescaler = F_CPU/8.0;
  htim->Init.CounterMode = TIM_COUNTERMODE_UP;
  htim->Init.Period = PulseWidth - 1;
  htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(htim) != HAL_OK)
  {
    _Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler();
  }

  if (HAL_TIM_PWM_Init(htim) != HAL_OK)
  {
    _Error_Handler();
  }

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler();
  }

  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(htim, &sConfigOC, Channel) != HAL_OK)
  {
    _Error_Handler();
  }

 
	GPIO_InitTypeDef GPIO_InitStruct;
 
  GPIO_InitStruct.Pin = GPIO_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = Alter_Func;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);

 
}

void PWM_SetDutyCycle(TIM_HandleTypeDef *htim , uint8_t Channel , float DutyCycle)
{
	switch(Channel)
	{
		case TIM_CHANNEL_1 :
			htim->Instance->CCR1 = DutyCycle*htim->Init.Period/100 - 1;
			break;
		
		case TIM_CHANNEL_2 :
			htim->Instance->CCR2 = DutyCycle*htim->Init.Period/100 - 1;
			break;
		
		case TIM_CHANNEL_3 :
			htim->Instance->CCR3 = DutyCycle*htim->Init.Period/100 - 1;
			break;
		
		case TIM_CHANNEL_4 :
			htim->Instance->CCR4 = DutyCycle*htim->Init.Period/100 - 1;
			break;
	}
}
	
void Timer_InterruptEnable(TIM_HandleTypeDef *htim , TIM_TypeDef *TIM , uint32_t InterruptTime_us , IRQn_Type IRQ , uint8_t PreemptPriority , uint8_t SubPriority)
{
	TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_MasterConfigTypeDef sMasterConfig;
  

  htim->Instance = TIM;
  htim->Init.Prescaler = F_CPU;
  htim->Init.CounterMode = TIM_COUNTERMODE_UP;
  htim->Init.Period = InterruptTime_us - 1;
  htim->Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  if (HAL_TIM_Base_Init(htim) != HAL_OK)
  {
    _Error_Handler();
  }

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(htim, &sClockSourceConfig) != HAL_OK)
  {
    _Error_Handler();
  }
	
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(htim, &sMasterConfig) != HAL_OK)
  {
    _Error_Handler();
  }

	HAL_TIM_Base_Start_IT(htim);
	HAL_NVIC_EnableIRQ(IRQ);
	HAL_NVIC_SetPriority(IRQ,PreemptPriority,SubPriority);
}

void SYSTICKS_Init(void)
{
	HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);
  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
  HAL_NVIC_SetPriority(SysTick_IRQn, 5 , 0);
}

void ExtInt_Init(GPIO_TypeDef *GPIOx , uint16_t GPIO_Pin , uint32_t GPIO_Mode , uint8_t GPIO_Pull , uint8_t PreemptPriority , uint8_t SubPriority)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	
	GPIO_InitStruct.Pin = GPIO_Pin;
  GPIO_InitStruct.Mode = GPIO_Mode;
  GPIO_InitStruct.Pull = GPIO_Pull;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	
	switch(GPIO_Pin)
	{
		case GPIO_PIN_0 :
			HAL_NVIC_SetPriority(EXTI0_IRQn, PreemptPriority , SubPriority);
			HAL_NVIC_EnableIRQ(EXTI0_IRQn);
			break;

		case GPIO_PIN_1 :
			HAL_NVIC_SetPriority(EXTI1_IRQn, PreemptPriority , SubPriority);
			HAL_NVIC_EnableIRQ(EXTI1_IRQn);
			break;

		case GPIO_PIN_2 :
			HAL_NVIC_SetPriority(EXTI2_IRQn, PreemptPriority , SubPriority);
			HAL_NVIC_EnableIRQ(EXTI2_IRQn);
			break;

		case GPIO_PIN_3 :
			HAL_NVIC_SetPriority(EXTI3_IRQn, PreemptPriority , SubPriority);
			HAL_NVIC_EnableIRQ(EXTI3_IRQn);
			break;

		case GPIO_PIN_4 :
			HAL_NVIC_SetPriority(EXTI4_IRQn, PreemptPriority , SubPriority);
			HAL_NVIC_EnableIRQ(EXTI4_IRQn);
			break;
		
		default :
			if(GPIO_Pin>=GPIO_PIN_5 && GPIO_Pin<=GPIO_PIN_9)
			{
				HAL_NVIC_SetPriority(EXTI9_5_IRQn, PreemptPriority , SubPriority);
				HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
			}
			else
			{
				HAL_NVIC_SetPriority(EXTI15_10_IRQn, PreemptPriority , SubPriority);
				HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);
			}
			break;
	}
		
}
		

void _Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  //while(1) 
  //{
  //}
  /* USER CODE END Error_Handler_Debug */ 
}



void ActuateLeftWheel(bool dir , uint16_t PWM)
{
	TIM2->CCR2 = PWM;
	
	if(dir)
	{
		HAL_GPIO_WritePin(GPIOA , GPIO_PIN_10 , GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOA , GPIO_PIN_10 , GPIO_PIN_RESET);
	}
}

void ActRightWheel(bool dir , uint16_t PWM)
{
	TIM2->CCR3 = PWM;
	
	if(dir)
	{
		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_4 , GPIO_PIN_SET);
	}
	else
	{
		HAL_GPIO_WritePin(GPIOB , GPIO_PIN_4 , GPIO_PIN_RESET);
	}

}


void I2C_Init(I2C_HandleTypeDef *hi2c , I2C_TypeDef *I2C , uint32_t ClockSpeed , GPIO_TypeDef  *GPIOx , 
							 uint16_t GPIO_Pin_SCL , uint16_t GPIO_Pin_SDA , uint8_t Alter_Func)
{
	hi2c->Instance = I2C;
  hi2c->Init.ClockSpeed = ClockSpeed;
  hi2c->Init.DutyCycle = I2C_DUTYCYCLE_2;
  hi2c->Init.OwnAddress1 = 0;
  hi2c->Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c->Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c->Init.OwnAddress2 = 0;
  hi2c->Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c->Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(hi2c) != HAL_OK)
  {
    _Error_Handler();
  }
	
	GPIO_InitTypeDef GPIO_InitStruct;
 
  GPIO_InitStruct.Pin = GPIO_Pin_SCL;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = Alter_Func;
  HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	
	GPIO_InitStruct.Pin = GPIO_Pin_SDA;
	HAL_GPIO_Init(GPIOx, &GPIO_InitStruct);
	
	HAL_I2C_Init(hi2c);

}

void HMC5883L_Init(I2C_HandleTypeDef *hi2c)
{
	uint8_t data[2];
	
	data[0] = 0;
	data[1] = 0x70;
	HAL_I2C_Master_Transmit(hi2c , HMC5883L_Adrr , data , 2 ,100);
	
	data[0] = 1;
	data[1] = 0xA0;
	HAL_I2C_Master_Transmit(hi2c , HMC5883L_Adrr , data , 2 ,100);
	
	data[0] = 2;
	data[1] = 0x00;
	HAL_I2C_Master_Transmit(hi2c , HMC5883L_Adrr , data , 2 ,100);
}

float HMC5883L_GetHeadings(I2C_HandleTypeDef *hi2c)
{
	uint8_t d=3;
	uint8_t raw[6];
	int16_t raw_x , raw_y;
	
	HAL_I2C_Master_Transmit(hi2c , HMC5883L_Adrr , &d , 1 ,100);	
	HAL_I2C_Master_Receive(hi2c , HMC5883L_Adrr | 0x01 , raw , 6 , 1000);
	
	raw_x = (raw[0] << 8);
	raw_x |= raw[1];
	raw_y = (raw[4] << 8);
	raw_y |= raw[5];
	
	return (atan2((double)raw_y,(double)raw_x) * 180 / 3.1416);
}


