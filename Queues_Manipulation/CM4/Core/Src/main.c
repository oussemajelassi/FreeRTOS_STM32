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
#include "queue.h"
#include "stdlib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef enum
{
	CommandingTaskIDLE,
	CommandingTaskLISTENING,
	CommandingTaskDONE
}CommandingTaskSTATE;
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

/* Definitions for CommandingTask */
osThreadId_t CommandingTaskHandle;
const osThreadAttr_t CommandingTask_attributes = {
		.name = "CommandingTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for ExecutingTask */
osThreadId_t ExecutingTaskHandle;
const osThreadAttr_t ExecutingTask_attributes = {
		.name = "ExecutingTask",
		.stack_size = 128 * 4,
		.priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for CommandQueue */
QueueHandle_t CommandQueueHandle ;


/* Definitions for FeedbackQueue */
QueueHandle_t FeedbackQueueHandle;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_USART3_UART_Init(void);
void vCommanding(void *argument);
void vExecuting(void *argument);

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
	/* USER CODE END RTOS_MUTEX */

	/* USER CODE BEGIN RTOS_SEMAPHORES */
	/* add semaphores, ... */
	/* USER CODE END RTOS_SEMAPHORES */

	/* USER CODE BEGIN RTOS_TIMERS */
	/* start timers, add new ones, ... */
	/* USER CODE END RTOS_TIMERS */

	/* Create the queue(s) */
	/* creation of CommandQueue */


	/* creation of FeedbackQueue */
	//FeedbackQueueHandle = osMessageQueueNew (10, sizeof(uint32_t), &FeedbackQueue_attributes);

	/* USER CODE BEGIN RTOS_QUEUES */
	/* add queues, ... */
	CommandQueueHandle = xQueueCreate(10,sizeof(uint32_t)) ;
	/* USER CODE END RTOS_QUEUES */

	/* Create the thread(s) */
	/* creation of CommandingTask */
	CommandingTaskHandle = osThreadNew(vCommanding, NULL, &CommandingTask_attributes);

	/* creation of ExecutingTask */
	ExecutingTaskHandle = osThreadNew(vExecuting, NULL, &ExecutingTask_attributes);

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

/* USER CODE BEGIN Header_vCommanding */
/**
 * @brief  Function implementing the CommandingTask thread.
 * @param  argument: Not used
 * @retval None
 */
/* USER CODE END Header_vCommanding */
void vCommanding(void *argument)
{
	/* USER CODE BEGIN 5 */
	CommandingTaskSTATE CommandingTask_CurrentState = CommandingTaskIDLE ;
	uint8_t UART_Counter = 0 , UART_RecievedData = 0  ;
	HAL_StatusTypeDef UART_MessageRecieved = HAL_ERROR ;
	char * Delay_Command = NULL ;
	char RTOS_USART_ORDERS_ch [20] ;
	BaseType_t Queue_Filled_b = pdFALSE ;
	uint32_t  pDelayCommand_u32 = 0 ;
	/* Infinite loop */
	for(;;)
	{
		switch ( CommandingTask_CurrentState )
		{
		case CommandingTaskIDLE :
			UART_MessageRecieved = HAL_UART_Receive(&huart3, &UART_RecievedData, 1, 100) ;
			if  ( UART_MessageRecieved == HAL_OK )
			{
				RTOS_USART_ORDERS_ch [ UART_Counter ++ ] = UART_RecievedData ;
				CommandingTask_CurrentState = CommandingTaskLISTENING ;
				vTaskDelay(1) ;
			}
			else
			{
				vTaskDelay(50) ;
			}
			break;
		case  CommandingTaskLISTENING :
			UART_MessageRecieved = HAL_UART_Receive(&huart3, &UART_RecievedData, 1, 100) ;
			if  ( UART_MessageRecieved == HAL_OK )
			{
				if ( UART_RecievedData == 13 )
				{
					CommandingTask_CurrentState = CommandingTaskDONE ;
					RTOS_USART_ORDERS_ch [ UART_Counter ++ ] = '\0'  ;
					vTaskDelay(1) ;
				}
				else
				{
					RTOS_USART_ORDERS_ch [ UART_Counter ++ ] = UART_RecievedData  ;
					vTaskDelay(1) ;
				}
			}

			break ;
		case CommandingTaskDONE :
			Delay_Command = strstr (RTOS_USART_ORDERS_ch ,"delay" ) ;
			if ( Delay_Command )
			{
				uint8_t Copy_counter = 0 ;
				uint8_t counter = 1 + ( int ) (strchr (RTOS_USART_ORDERS_ch, ' ' ) - RTOS_USART_ORDERS_ch)  ;
				while ( RTOS_USART_ORDERS_ch [counter] != '\0' )
				{
					* (Delay_Command + Copy_counter) = RTOS_USART_ORDERS_ch [counter ++ ] ;
					Copy_counter ++ ;
				}
				* (Delay_Command + Copy_counter) = '\0' ;
				HAL_UART_Transmit(&huart3, (uint8_t *) Delay_Command , strlen(Delay_Command) ,  100 ) ;
				pDelayCommand_u32 = atoi(Delay_Command) ;
				xQueueSend(CommandQueueHandle , &pDelayCommand_u32 , 10 ) ;
			}
			CommandingTask_CurrentState = CommandingTaskIDLE ;
			memset ( RTOS_USART_ORDERS_ch , 0 ,UART_Counter * sizeof(char)) ;
			UART_Counter = 0 ;

			break ;

		}
	}
	/* USER CODE END 5 */
}

/* USER CODE BEGIN Header_vExecuting */
/**
 * @brief Function implementing the ExecutingTask thread.
 * @param argument: Not used
 * @retval None
 */
/* USER CODE END Header_vExecuting */
void vExecuting(void *argument)
{
	/* USER CODE BEGIN vExecuting */
	uint32_t pCommand_Buffer_u32 = 0 ;
	uint64_t RTOS_Ticks ;
	/* Infinite loop */
	for(;;)
	{
		if ( xQueueReceive(CommandQueueHandle, &pCommand_Buffer_u32, 10 ) )
		{
			HAL_UART_Transmit(&huart3,(uint8_t*) &pCommand_Buffer_u32 , 4 , 100 ) ;
		}
		HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_0) ;
		vTaskDelay(pCommand_Buffer_u32) ;
	}
	/* USER CODE END vExecuting */
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
