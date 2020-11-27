/*
 * n1202.h
 *
 *  Created on: Apr 19, 2020
 *      Author: quard
 */

#ifndef INC_N1202_H_
#define INC_N1202_H_

#include "stdlib.h"

#include "stm32f1xx_hal.h"
#include "main.h"


//#define N1202_RST_PORT
//#define N1202_RST_PIN
//#define N1202_CS_PORT
//#define N1202_CS_PIN
//#define N1202_MOSI_PORT
//#define N1202_MOSI_PIN
//#define N1202_SCK_PORT
//#define N1202_SCK_PIN
//#define N1202_LED_PORT
//#define N1202_LED_PIN

#define N1202_CMD     0
#define N1202_DATA    1
#define N1202_WIDTH   96
#define N1202_HEIGHT  68
#define N1202_STRING  9

#define N1202_SET_Y_ADDR  0xB0
#define N1202_SET_X_ADDR4 0x00
#define N1202_SET_X_ADDR3 0x10

#define swap(a, b) {uint8_t t = a; a = b; b = t; }

void N1202_Init();
void N1202_Clear();
void N1202_Update();
void N1202_DrawPixel(uint8_t x, uint8_t y, uint8_t color);
void N1202_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color);
void N1202_DrawVLine(uint8_t x, uint8_t y, uint8_t height, uint8_t color);
void N1202_DrawHLine(uint8_t x, uint8_t y, uint8_t width, uint8_t color);
void N1202_DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);
void N1202_DrawFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color);


#endif /* INC_N1202_H_ */
