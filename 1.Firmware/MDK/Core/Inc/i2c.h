/**
  ******************************************************************************
  * @file    i2c.h
  * @brief   This file contains all the function prototypes for
  *          the i2c.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __I2C_H__
#define __I2C_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */
#define I2C_WR	0		/* Ð´¿ØÖÆbit */
#define I2C_RD	1		/* ¶Á¿ØÖÆbit */

#define GPIO_I2C_Port       GPIOB
#define I2C_SCL_Pin         GPIO_PIN_10
#define I2C_SDA_Pin         GPIO_PIN_11

#define I2C_SCL_1()  HAL_GPIO_WritePin(GPIO_I2C_Port, I2C_SCL_Pin, GPIO_PIN_SET)		    /* SCL = 1 */
#define I2C_SCL_0()  HAL_GPIO_WritePin(GPIO_I2C_Port, I2C_SCL_Pin, GPIO_PIN_RESET)		  /* SCL = 0 */

#define I2C_SDA_1()  HAL_GPIO_WritePin(GPIO_I2C_Port, I2C_SDA_Pin, GPIO_PIN_SET)		    /* SDA = 1 */
#define I2C_SDA_0()  HAL_GPIO_WritePin(GPIO_I2C_Port, I2C_SDA_Pin, GPIO_PIN_RESET)		  /* SDA = 0 */

#define I2C_SDA_READ()  HAL_GPIO_ReadPin(GPIO_I2C_Port, I2C_SDA_Pin)
/* USER CODE END Includes */

extern I2C_HandleTypeDef hi2c1;
extern I2C_HandleTypeDef hi2c2;

/* USER CODE BEGIN Private defines */
void i2c_Start(void);
void i2c_Stop(void);
void i2c_SendByte(uint8_t _ucByte);
uint8_t i2c_ReadByte(uint8_t ack);
uint8_t i2c_WaitAck(void);
void i2c_Ack(void);
void i2c_NAck(void);
void i2c_GPIO_Config(void);
uint8_t i2c_CheckDevice(uint8_t _Address);
/* USER CODE END Private defines */

void MX_I2C1_Init(void);
void MX_I2C2_Init(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __I2C_H__ */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
