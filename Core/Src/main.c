/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
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
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ringbuff.h"
#include "protocol.h"
#include "commands.h"
#include <stdarg.h>
#include <string.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef uint8_t bool;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile buffer_t Tx = {0}, Rx = {0};
framecontent frame;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART3) {

		Rx.RXbuffIdx++;
		if (Rx.RXbuffIdx >= sizeOfBuffer) {
			Rx.RXbuffIdx = 0;
		}
		HAL_UART_Receive_IT(&huart3, &Rx.array[Rx.RXbuffIdx], 1);
	}
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart){
	if(Tx.TXbuffIdx != Tx.RXbuffIdx){
		uint8_t tempChar = Tx.array[Tx.TXbuffIdx];
		Tx.TXbuffIdx++;

		if(Tx.TXbuffIdx >= sizeOfBuffer){
			Tx.TXbuffIdx = 0;
		}
		HAL_UART_Transmit_IT(&huart3, &tempChar, 1);
	}
}

void USART_Send(char* message, ...){
	char tempMsg[105];
	int i;
	volatile int send_idx = Tx.RXbuffIdx;

	va_list arglist;
	va_start(arglist, message);
	vsprintf(tempMsg, message, arglist);
	va_end(arglist);

	for (i = 0; i < strlen(tempMsg); i++) {
		Tx.array[send_idx] = tempMsg[i];
		send_idx++;
		if (send_idx >= sizeOfBuffer) {
			send_idx = 0;
		}
	}

	__disable_irq();
	if ((Tx.RXbuffIdx == Tx.TXbuffIdx) && (__HAL_UART_GET_FLAG(&huart3, UART_FLAG_TXE)==SET)) {
		Tx.RXbuffIdx = send_idx;
		uint8_t tmp = Tx.array[Tx.TXbuffIdx];
		Tx.TXbuffIdx++;
		if (Tx.TXbuffIdx >= sizeOfBuffer)
			Tx.TXbuffIdx = 0;

		HAL_UART_Transmit_IT(&huart3, &tmp, 1);
		}else{
			Tx.RXbuffIdx = send_idx;
		}
	__enable_irq();
}
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

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_USART3_UART_Init();
	/* USER CODE BEGIN 2 */

	/* USER CODE END 2 */

	/* Infinite loop */
	/* USER CODE BEGIN WHILE */
	HAL_UART_Receive_IT(&huart3, &Rx.array[Rx.RXbuffIdx], 1);
	bool receiving = false;
	bool processing = false;
	command_t response;
	uint8_t frameChar = '\0';
	uint8_t chars = 0;
	while (1)
	{
		if(Rx.RXbuffIdx != Rx.frameBeginIdx)
		{
			frameChar = Rx.array[Rx.frameBeginIdx];
			Rx.frameBeginIdx = Rx.RXbuffIdx; // odczytano znak i zaktualizowanie indeksu pomocniczego

			if(frameChar == '$')
			{
				receiving = true;
				//Rx.frameEndIdx = 0;
			}

			if(receiving)
			{
				Rx.tempArray[chars] = frameChar;
				chars++;

			}
			if(frameChar == '#')
			{
				receiving = false;
				processing = true;
				chars = 0;
				//Rx.frameEndIdx = 0;
			}
		}

		if(processing)
		{
			frame = parseRxBuffer();
			if(frame.frameOK)
			{
				response = commands();
				USART_Send(response.TX_payload);
			}
			else
			{
				USART_Send("Frame Error");
			}
			processing = false;
		}
	}
    /* USER CODE BEGIN 3 */


  	  /* USER CODE END 3 */
  /* USER CODE END WHILE */
}


/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure LSE Drive Capability
  */
  HAL_PWR_EnableBkUpAccess();

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_BYPASS;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 96;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Activate the Over-Drive mode
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

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
