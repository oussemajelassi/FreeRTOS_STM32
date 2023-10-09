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
#include "string.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
	WritingTaskIDLE,
	WritingTaskLISTENING,
	WritingTaskDONE
}WritingTaskSTATE;
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

/* Definitions for ReadingTask */
osThreadId_t ReadingTaskHandle;
const osThreadAttr_t ReadingTask_attributes = {
		.name = "ReadingTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for WritingTask */
osThreadId_t WritingTaskHandle;
const osThreadAttr_t WritingTask_attributes = {
		.name = "WritingTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* USER CODE BEGIN PV */
char RTOS_USART_ORDERS_ch [20] ;
char * Heap_AllocatedMemory ;

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
void vReading(void *argument);
void vWriting(void *argument);

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
	/* creation of ReadingTask */
	ReadingTaskHandle = osThreadNew(vReading, NULL, &ReadingTask_attributes);

	/* creation of WritingTask */
	WritingTaskHandle = osThreadNew(vWriting, NULL, &WritingTask_attributes);

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

	/* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOD_CLK_ENABLE();

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* USER CODE BEGIN Header_vReading */
/**
 * @brief  Function implementing the ReadingTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_vReading */
void vReading(void *argument)
{
	/* USER CODE BEGIN 5 */
	uint32_t Recieved_Notification ;
	/* Infinite loop */
	for(;;)
	{
		Recieved_Notification = ulTaskNotifyTake(pdTRUE ,portMAX_DELAY ) ;
		if ( Recieved_Notification == 1 )
		{
			HAL_UART_Transmit(&huart3, "Hello From Task Read\r\n", 22, 200) ;
		}
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_vWriting */
/**
 * @brief Function implementing the WritingTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_vWriting */
void vWriting(void *argument)
{
	/* USER CODE BEGIN vWriting */
	WritingTaskSTATE WritingTaskCurrentState = WritingTaskIDLE ;
	uint8_t UART_Counter = 0 , UART_RecievedData = 0  ;
	HAL_StatusTypeDef UART_MessageRecieved = HAL_ERROR ;
	/* Infinite loop */
	for(;;)
	{
		switch ( WritingTaskCurrentState )
		{
		case WritingTaskIDLE :
			UART_MessageRecieved = HAL_UART_Receive(&huart3, &UART_RecievedData , 1 , 100) ;
			if ( UART_MessageRecieved == HAL_OK )
			{
				RTOS_USART_ORDERS_ch [ UART_Counter ] =(char) UART_RecievedData ;
				UART_Counter ++ ;
				WritingTaskCurrentState = WritingTaskLISTENING ;
				vTaskDelay( 1 ) ;

			}
			else
			{
				vTaskDelay( 30 ) ;
			}
			break ;

		case WritingTaskLISTENING :
			UART_MessageRecieved = HAL_UART_Receive(&huart3, &UART_RecievedData , 1 , 100) ;
			if ( UART_MessageRecieved == HAL_OK )
			{
				if ( UART_RecievedData == '\r')
				{
					WritingTaskCurrentState = WritingTaskDONE ;
				}
				else
				{
					RTOS_USART_ORDERS_ch[ UART_Counter ] = UART_RecievedData ;
					UART_Counter ++ ;
					vTaskDelay( 1 ) ;
				}
			}
			break ;

		case WritingTaskDONE :

			Heap_AllocatedMemory = (char *) pvPortMalloc ( UART_Counter * sizeof( char ) ) ;

			strcpy ( Heap_AllocatedMemory , RTOS_USART_ORDERS_ch  ) ;

			xTaskNotify( ReadingTaskHandle , 1 , eSetValueWithOverwrite ) ;

			break ;


		}
	}
	/* USER CODE END vWriting */
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
