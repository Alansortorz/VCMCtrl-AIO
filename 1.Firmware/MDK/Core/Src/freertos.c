/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart.h"
#include "math.h"
#include "stdlib.h"
#include "oled.h"
#include "AM26C32.h"
#include "imu.h"
#include "param.h"
#include "tof200f_uart.h"
#include "TOF200F.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

Sensor_Original_Data Sensor_data;
Main_Data main_data;

uint16_t distance;
float Filter_distance;


/* USER CODE END Variables */
/* Definitions for defaultTask */
osThreadId_t defaultTaskHandle;
const osThreadAttr_t defaultTask_attributes = {
  .name = "defaultTask",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void StartDefaultTask(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

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
  /* creation of defaultTask */
  defaultTaskHandle = osThreadNew(StartDefaultTask, NULL, &defaultTask_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_StartDefaultTask */
/**
  * @brief  Function implementing the defaultTask thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_StartDefaultTask */
void StartDefaultTask(void *argument)
{
  /* USER CODE BEGIN StartDefaultTask */
  float AccData[3];
  float GyroData[3];
  float g_xiamen;
  /* Infinite loop */	
  for(;;)
  {
	  /** grating sensor Test **/
	  main_data.Actual_Position = VCMEncoderGetPosition();
	  printf("%f\n", main_data.Actual_Position);
	  
	  /** IMU Test **/
	  MPU6050ReadAcc(Sensor_data.Accdata);
	  MPU6050ReadGyro(Sensor_data.Gyrodata);
	  MPU6050_ReturnTemp(&(Sensor_data.Temp));
	  g_xiamen = 9.7830;
	  AccData[0] = g_xiamen*(float)(Sensor_data.Accdata[0])/16384;
	  AccData[1] = g_xiamen*(float)(Sensor_data.Accdata[1])/16384;
	  AccData[2] = g_xiamen*(float)(Sensor_data.Accdata[2])/16384;
	  GyroData[0] = (float)(Sensor_data.Gyrodata[0]/131.0);
	  GyroData[1] = (float)(Sensor_data.Gyrodata[1]/131.0);
	  GyroData[2] = (float)(Sensor_data.Gyrodata[2]/131.0);
	  //printf("AccData:%d,%d,%d\n", Sensor_data.Accdata[0], Sensor_data.Accdata[1], Sensor_data.Accdata[2]);
	  //printf("GyroData:%f,%f,%f,%f,%f,%f\n", AccData[0], AccData[1], AccData[2], GyroData[0], GyroData[1], GyroData[2]);
	  
	  /** TOF Senser Test **/
	  //Sensor_data.TOF_Counts = tof200f_trans();
	  //Filter_distance = TOF_HEXConvertToEngineerValye();
	  //printf("%f\n", Filter_distance);
	  
	  HAL_Delay(500);
	  HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin);
  }
  /* USER CODE END StartDefaultTask */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
