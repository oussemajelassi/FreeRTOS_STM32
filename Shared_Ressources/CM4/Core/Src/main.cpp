/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2023 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "CircularBuffer.h"
#include "SharedRessources.h"
#include "semphr.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#ifndef HSEM_ID_0
#define HSEM_ID_0 (0U) /* HW semaphore 0*/
#endif
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

UART_HandleTypeDef huart3;

/* Definitions for ProducerTask_1 */
osThreadId_t ProducerTask_1Handle;
const osThreadAttr_t ProducerTask_1_attributes = {
		.name = "ProducerTask_1",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ProducerTask_2 */
osThreadId_t ProducerTask_2Handle;
const osThreadAttr_t ProducerTask_2_attributes = {
		.name = "ProducerTask_2",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ProducerTask_3 */
osThreadId_t ProducerTask_3Handle;
const osThreadAttr_t ProducerTask_3_attributes = {
		.name = "ProducerTask_3",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ProducerTask_4 */
osThreadId_t ProducerTask_4Handle;
const osThreadAttr_t ProducerTask_4_attributes = {
		.name = "ProducerTask_4",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ProducerTask_5 */
osThreadId_t ProducerTask_5Handle;
const osThreadAttr_t ProducerTask_5_attributes = {
		.name = "ProducerTask_5",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Consumer_1 */
osThreadId_t Consumer_1Handle;
const osThreadAttr_t Consumer_1_attributes = {
		.name = "Consumer_1",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Consumer_2 */
osThreadId_t Consumer_2Handle;
const osThreadAttr_t Consumer_2_attributes = {
		.name = "Consumer_2",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */

CircularBuffer SharedBuffer ( 15 ) ;
SemaphoreHandle_t SharedBufferMutex ;
SemaphoreHandle_t SharedBufferEmptySlots ;
SemaphoreHandle_t SharedBufferFilledSlots ;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
void vProducer_1(void *argument);
void vProducer_2(void *argument);
void vProducer_3(void *argument);
void vProducer_4(void *argument);
void vProducer_5(void *argument);
void vConsumer_1(void *argument);
void vConsumer_2(void *argument);

/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */


	/* USER CODE END 1 */

	/* USER CODE BEGIN Boot_Mode_Sequence_1 */

	/* USER CODE END Boot_Mode_Sequence_1 */
	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART3_UART_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Init scheduler */
	osKernelInitialize();

	/* USER CODE BEGIN RTOS_MUTEX */
	/* add mutexes, ... */
	SharedBufferMutex  = xSemaphoreCreateMutex() ;
	SharedBufferEmptySlots = xSemaphoreCreateCounting ( SharedBuffer.size   , SharedBuffer.size   )  ;
	SharedBufferFilledSlots   = xSemaphoreCreateCounting ( SharedBuffer.size   , 0 ) ;
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of ProducerTask_1 */
	ProducerTask_1Handle = osThreadNew(vProducer_1, NULL, &ProducerTask_1_attributes);

	/* creation of ProducerTask_2 */
	ProducerTask_2Handle = osThreadNew(vProducer_2, NULL, &ProducerTask_2_attributes);

	/* creation of ProducerTask_3 */
	ProducerTask_3Handle = osThreadNew(vProducer_3, NULL, &ProducerTask_3_attributes);

	/* creation of ProducerTask_4 */
	ProducerTask_4Handle = osThreadNew(vProducer_4, NULL, &ProducerTask_4_attributes);

	/* creation of ProducerTask_5 */
	ProducerTask_5Handle = osThreadNew(vProducer_5, NULL, &ProducerTask_5_attributes);

	/* creation of Consumer_1 */
	Consumer_1Handle = osThreadNew(vConsumer_1, NULL, &Consumer_1_attributes);

	/* creation of Consumer_2 */
	Consumer_2Handle = osThreadNew(vConsumer_2, NULL, &Consumer_2_attributes);

	/* USER CODE BEGIN RTOS_THREADS */
	/* add threads, ... */
	/* USER CODE END RTOS_THREADS */

	/* USER CODE BEGIN RTOS_EVENTS */
	/* add events, ... */
	/* USER CODE END RTOS_EVENTS */

	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	while (1)
	{
		/* USER CODE END WHILE */

		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief USART3 Initialization Function
 * @param None
 * @retval None
 */
static void MX_USART3_UART_Init(void)
{

	/* USER CODE BEGIN USART3_Init 0 */

	/* USER CODE END USART3_Init 0 */

	/* USER CODE BEGIN USART3_Init 1 */

	/* USER CODE END USART3_Init 1 */
	huart3.Instance = USART3;
	huart3.Init.BaudRate = 115200;
	huart3.Init.WordLength = UART_WORDLENGTH_8B;
	huart3.Init.StopBits = UART_STOPBITS_1;
	huart3.Init.Parity = UART_PARITY_NONE;
	huart3.Init.Mode = UART_MODE_TX_RX;
	huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
	huart3.Init.OverSampling = UART_OVERSAMPLING_16;
	huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
	huart3.Init.ClockPrescaler = UART_PRESCALER_DIV1;
	huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
	if (HAL_UART_Init(&huart3) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetTxFifoThreshold(&huart3, UART_TXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_SetRxFifoThreshold(&huart3, UART_RXFIFO_THRESHOLD_1_8) != HAL_OK)
	{
		Error_Handler();
	}
	if (HAL_UARTEx_DisableFifoMode(&huart3) != HAL_OK)
	{
		Error_Handler();
	}
	/* USER CODE BEGIN USART3_Init 2 */

	/* USER CODE END USART3_Init 2 */

}

/**
 * @brief GPIO Initialization Function
 * @param None
 * @retval None
 */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = {0};

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOG_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0|GPIO_PIN_14, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOD, GPIO_PIN_15, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOG, GPIO_PIN_6, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOE, GPIO_PIN_1, GPIO_PIN_RESET);

	/*Configure GPIO pin : PB0 */
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PB14 */
	GPIO_InitStruct.Pin = GPIO_PIN_14;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*Configure GPIO pin : PD15 */
	GPIO_InitStruct.Pin = GPIO_PIN_15;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

	/*Configure GPIO pin : PG6 */
	GPIO_InitStruct.Pin = GPIO_PIN_6;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOG, &GPIO_InitStruct);

	/*Configure GPIO pin : PE1 */
	GPIO_InitStruct.Pin = GPIO_PIN_1;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_vProducer_1 */
/**
 * @brief  Function implementing the ProducerTask_1 thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_vProducer_1 */
void vProducer_1(void *argument)
{
	/* USER CODE BEGIN 5 */
	Producer Producer_1 ( GPIOB , GPIO_PIN_0 , 2 ) ;
	/* Infinite loop */
	for(;;)
	{
		while  ( Producer_1.ProducedQuantity_u8 )
		{
			if ( xSemaphoreTake(SharedBufferEmptySlots , portMAX_DELAY ) ==pdTRUE )
			{
				xSemaphoreTake(SharedBufferMutex , 0 ) ;
				Producer_1.ProducedQuantity_u8 -- ;
				Producer_1.InsertItem(&SharedBuffer) ;
				xSemaphoreGive(SharedBufferMutex) ;
				xSemaphoreGive(SharedBufferFilledSlots) ;

			}
			else
			{
				break  ;
			}
		}

		vTaskDelay(5) ;
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_vProducer_2 */
/**
 * @brief Function implementing the ProducerTask_2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_vProducer_2 */
void vProducer_2(void *argument)
{
	/* USER CODE BEGIN vProducer_2 */
	Producer Producer_2 ( GPIOB , GPIO_PIN_14 , 1 ) ;
	/* Infinite loop */
	for(;;)
	{
		while ( ( xSemaphoreTake(SharedBufferEmptySlots , portMAX_DELAY ) ==pdTRUE )  && ( Producer_2.ProducedQuantity_u8 ) )
		{
			xSemaphoreTake(SharedBufferMutex , 0 ) ;
			Producer_2.ProducedQuantity_u8 -- ;
			Producer_2.InsertItem(&SharedBuffer) ;
			xSemaphoreGive(SharedBufferMutex) ;
			xSemaphoreGive(SharedBufferFilledSlots) ;
		}
		vTaskDelay(5) ;
	}
	/* USER CODE END vProducer_2 */
}

/* USER CODE BEGIN Header_vProducer_3 */
/**
 * @brief Function implementing the ProducerTask_3 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_vProducer_3 */
void vProducer_3(void *argument)
{
	/* USER CODE BEGIN vProducer_3 */
	Producer Producer_3 (GPIOD, GPIO_PIN_15 , 6 ) ;
	/* Infinite loop */
	for(;;)
	{
		while ( ( xSemaphoreTake(SharedBufferEmptySlots , portMAX_DELAY ) ==pdTRUE )  && ( Producer_3.ProducedQuantity_u8 ) )
		{
			xSemaphoreTake(SharedBufferMutex , 0 ) ;
			Producer_3.ProducedQuantity_u8 -- ;
			Producer_3.InsertItem(&SharedBuffer) ;
			xSemaphoreGive(SharedBufferMutex) ;
			xSemaphoreGive(SharedBufferFilledSlots) ;

		}
		vTaskDelay(5) ;
	}
	/* USER CODE END vProducer_3 */
}

/* USER CODE BEGIN Header_vProducer_4 */
/**
 * @brief Function implementing the ProducerTask_4 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_vProducer_4 */
void vProducer_4(void *argument)
{
	/* USER CODE BEGIN vProducer_4 */
	Producer Producer_4 (GPIOG, GPIO_PIN_6 , 5  ) ;
	/* Infinite loop */
	for(;;)
	{

		while ( ( xSemaphoreTake(SharedBufferEmptySlots , portMAX_DELAY ) ==pdTRUE )  && ( Producer_4.ProducedQuantity_u8 ) )
		{
			xSemaphoreTake(SharedBufferMutex , 0 ) ;
			Producer_4.ProducedQuantity_u8 -- ;
			Producer_4.InsertItem(&SharedBuffer) ;
			xSemaphoreGive(SharedBufferMutex) ;
			xSemaphoreGive(SharedBufferFilledSlots) ;
		}
		vTaskDelay(5) ;
	}
	/* USER CODE END vProducer_4 */
}

/* USER CODE BEGIN Header_vProducer_5 */
/**
 * @brief Function implementing the ProducerTask_5 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_vProducer_5 */
void vProducer_5(void *argument)
{
	/* USER CODE BEGIN vProducer_5 */
	Producer Producer_5 (GPIOE, GPIO_PIN_1 , 2);
	/* Infinite loop */
	for(;;)
	{
		while ( ( xSemaphoreTake(SharedBufferEmptySlots , portMAX_DELAY ) ==pdTRUE )  && ( Producer_5.ProducedQuantity_u8 ) )
		{
			xSemaphoreTake(SharedBufferMutex , 0 ) ;
			Producer_5.ProducedQuantity_u8 -- ;
			Producer_5.InsertItem(&SharedBuffer) ;
			xSemaphoreGive(SharedBufferMutex) ;
			xSemaphoreGive(SharedBufferFilledSlots) ;
		}
		vTaskDelay(5) ;
	}
	/* USER CODE END vProducer_5 */
}

/* USER CODE BEGIN Header_vConsumer_1 */
/**
 * @brief Function implementing the Consumer_1 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_vConsumer_1 */
void vConsumer_1(void *argument)
{
	/* USER CODE BEGIN vConsumer_1 */
	Consumer Consumer_1  ;
	/* Infinite loop */
	for(;;)
	{
		while ( ( xSemaphoreTake(SharedBufferFilledSlots , portMAX_DELAY ) ==pdTRUE )  && ( Consumer_1.NeededQuantity_u8 ) )
		{
			xSemaphoreTake(SharedBufferMutex , 0 ) ;
			Consumer_1.NeededQuantity_u8  -- ;
			Consumer_1.RetrieveItem(&SharedBuffer) ;
			xSemaphoreGive(SharedBufferMutex) ;
			xSemaphoreGive(SharedBufferEmptySlots) ;
		}

		vTaskDelay(5) ;

	}
	/* USER CODE END vConsumer_1 */
}

/* USER CODE BEGIN Header_vConsumer_2 */
/**
 * @brief Function implementing the Consumer_2 thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_vConsumer_2 */
void vConsumer_2(void *argument)
{
	/* USER CODE BEGIN vConsumer_2 */
	Consumer Consumer_2  ;
	/* Infinite loop */
	for(;;)
	{

		while ( ( xSemaphoreTake(SharedBufferFilledSlots , 10 ) ==pdTRUE )  && ( Consumer_2.NeededQuantity_u8 ) )
		{
			xSemaphoreTake(SharedBufferMutex , 0 ) ;
			Consumer_2.NeededQuantity_u8  -- ;
			Consumer_2.RetrieveItem(&SharedBuffer) ;
			xSemaphoreGive(SharedBufferMutex) ;
			xSemaphoreGive(SharedBufferEmptySlots) ;
		}

		vTaskDelay(5) ;

	}
	/* USER CODE END vConsumer_2 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
