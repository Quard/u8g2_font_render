/*
 * SSD1306.c
 *
 *  Created on: May 16, 2020
 *      Author: quard
 */
#include "SSD1306.h"


#define SSD1306_WriteCommand(display, command)  SSD1306_I2C_Write((display), 0x00, (command))
#define SSD1306_WriteData(display, data)        SSD1306_I2C_Write((display), 0x40, (data))

//static uint8_t buffer_with_cmd[128 * 64 / 8 + 1] = {0}, *buffer = buffer_with_cmd + 1;

HAL_StatusTypeDef SSD1306_I2C_Write(SSD1306_HandleTypeDef *disp, uint8_t reg, uint8_t data) {
	uint8_t dt[2] = {reg, data};

	return HAL_I2C_Master_Transmit(disp->hi2c, disp->addr, dt, 2, SSD1306_I2C_TIMEOUT);
}

static uint8_t init_script[] = {
	0xAE,  // display off
	0x20,  // memory addressing mode
	0x10,  // 00 - horizontal; 01 - vertical; 10 - page (reset); 11 - invalid
	0xB0,  // page start for page addressing mode
	0xC8,  // COM output scan direction
	0x00,  // low column address
	0x10,  // high column address
	0x40,  // start line address
	0x8F,  //	0x81,  // contrast
	0xFF,
	0xA1,  // segment remap 0 to 127
	0xA6,  // normal display
	0xA8,  // multiplex ratio (1 to 64)
	0x3F,
	0xA4,  // 0xA4 - output follow RAM content; 0xA5 - output ignore RAM content
	0xD3,  // display offset
	0x00,  // no offset
	0xD5,  // display clock divide ratio/oscillator frequency
	0xF0,  // divide ratio
	0xD9,  // pre-charge period
	0x22,  // ^^
	0xDA,  // com pins hardware configuration
	0x00,  //	0x12,  // ^^
	0xDB,  // VCOMH
	0x20,  // 0x20 - 0.77xVcc
	0x8D,  // DC-DC enable
	0x14,
	0xAF,  // display on
};

HAL_StatusTypeDef SSD1306_Init(SSD1306_HandleTypeDef *disp) {
	if (HAL_I2C_IsDeviceReady(disp->hi2c, disp->addr, 5, 1000) != HAL_OK) {
		return HAL_ERROR;
	}

	disp->buffer = malloc(disp->height * disp->width / 8 + 1);

	if (disp->height == 32) {
		init_script[22] = 0x02;
	} else if (disp->height == 64) {
		init_script[22] = 0x12;
	}

	for (uint8_t i=0; i<sizeof(init_script); i++) {
		SSD1306_WriteCommand(disp, init_script[i]);
	}

	SSD1306_Fill(disp, SSD1306_COLOR_BLACK);
	SSD1306_Update(disp);

	return HAL_OK;
}

HAL_StatusTypeDef SSD1306_Update(SSD1306_HandleTypeDef *disp) {
//	buffer_with_cmd[0] = 0x40;
	disp->buffer[0] = 0x40;

	return HAL_I2C_Master_Transmit(disp->hi2c, disp->addr, disp->buffer, disp->width * disp->height / 8 + 1, HAL_MAX_DELAY);
//	HAL_StatusTypeDef stat = HAL_I2C_Master_Transmit_DMA(disp->hi2c	, disp->addr, buffer_with_cmd, disp->width * disp->height / 8 + 1);
//	if (stat == HAL_OK) {
//		while (HAL_DMA_GetState(disp->hi2c->hdmatx) != HAL_DMA_STATE_READY) {
//			HAL_Delay(5);
//		}
//	}
//
//	return stat;
}

HAL_StatusTypeDef SSD1306_Fill(SSD1306_HandleTypeDef *disp, uint8_t color) {
	memset(disp->buffer, (color == SSD1306_COLOR_BLACK) ? 0x00 : 0xFF, disp->width * disp->height / 8 + 1);

	return HAL_OK;
}

HAL_StatusTypeDef SSD1306_DrawPixel(SSD1306_HandleTypeDef *disp, uint8_t x, uint8_t y, uint8_t color) {
	if (x >= disp->width || y >= disp->height) {
		return HAL_ERROR;
	}

	if (color == SSD1306_COLOR_WHITE) {
		disp->buffer[x + (y / 8) * disp->width + 1] |= 1 << (y % 8);
	} else {
		disp->buffer[x + (y / 8) * disp->width + 1] &= ~(1 << (y % 8));
	}

	return HAL_OK;
}

//HAL_StatusTypeDef SSD1306_DrawChar(SSD1306_HandleTypeDef *disp, uint8_t x, uint8_t y, uint8_t ch, uint8_t color) {
//	if (x >= disp->width || y >= disp->height) {
//		return HAL_ERROR;
//	}
//
//	if (ch < 128)                ch -= 32;
//	if (ch >= 144 && ch <= 175)  ch -= 48;
//	if (ch >= 128 && ch <= 143)  ch += 16;
//	if (ch >= 176 && ch <= 191)  ch -= 48;
//	if (ch > 191)                return HAL_ERROR;
//
//	for (uint8_t i=0; i<6; i++) {
//		uint8_t line;
//		if (i == 5) { line = 0x00; }
//		else { line = font[(ch * 5) + i]; }
//		for (uint8_t j=0; j < 8; j++) {
//			if (line & 0x01) {
//				SSD1306_DrawPixel(disp, x+i, y+j, color);
//			} else {
//				SSD1306_DrawPixel(disp, x+i, y+j, !color);
//			}
//			line >>= 1;
//		}
//	}
//
//	return HAL_OK;
//}
//
//HAL_StatusTypeDef SSD1306_Print(SSD1306_HandleTypeDef *disp, uint8_t x, uint8_t y, char *str, uint8_t color) {
//	unsigned char type = *str;
//	if (type >= 128) x -= 3;
//	while (*str) {
//		HAL_StatusTypeDef stat = SSD1306_DrawChar(disp, x, y, *str++, color);
//		if (stat != HAL_OK) {
//			return stat;
//		}
//		unsigned char type = *str;
//		if (type >= 128) { x += 3; }
//		else { x += 6; }
//	}
//
//	return HAL_OK;
//}
