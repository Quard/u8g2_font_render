/*
 * SSD_1306.h
 *
 *  Created on: May 16, 2020
 *      Author: quard
 */

#ifndef INC_SSD1306_H_
#define INC_SSD1306_H_

#include "stm32f1xx_hal.h"
#include "stdlib.h"
#include "string.h"



#ifndef SSD1306_I2C_TIMEOUT
#define SSD1306_I2C_TIMEOUT 500
#endif

#define SSD1306_COLOR_BLACK 0x00
#define SSD1306_COLOR_WHITE 0x01

typedef struct {
	I2C_HandleTypeDef *hi2c;
	uint8_t            addr;  // usually 0x78 or 0x7A
	uint8_t            width;
	uint8_t            height;
	uint8_t           *buffer;
} SSD1306_HandleTypeDef;

HAL_StatusTypeDef SSD1306_Init(SSD1306_HandleTypeDef *disp);
HAL_StatusTypeDef SSD1306_Update(SSD1306_HandleTypeDef *disp);
HAL_StatusTypeDef SSD1306_Fill(SSD1306_HandleTypeDef *disp, uint8_t color);
HAL_StatusTypeDef SSD1306_DrawPixel(SSD1306_HandleTypeDef *disp, uint8_t x, uint8_t y, uint8_t color);
HAL_StatusTypeDef SSD1306_DrawChar(SSD1306_HandleTypeDef *disp, uint8_t x, uint8_t y, uint8_t ch, uint8_t color);
//HAL_StatusTypeDef SSD1306_Print(SSD1306_HandleTypeDef *disp, uint8_t x, uint8_t y, char *str, uint8_t color);

#endif /* INC_SSD1306_H_ */
