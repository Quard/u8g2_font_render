/*
 * n1202.c
 *
 *  Created on: Apr 19, 2020
 *      Author: quard
 */
#include "n1202.h"


extern uint8_t _N1202_RAM[N1202_WIDTH * N1202_STRING];


void N1202_SendByte(uint8_t mode, uint8_t c) {
	HAL_GPIO_WritePin(N1202_CS_PORT, N1202_CS_PIN, GPIO_PIN_RESET);

	if (mode) {
		HAL_GPIO_WritePin(N1202_MOSI_PORT, N1202_MOSI_PIN, GPIO_PIN_SET);
	} else {
		HAL_GPIO_WritePin(N1202_MOSI_PORT, N1202_MOSI_PIN, GPIO_PIN_RESET);
	}

	HAL_GPIO_WritePin(N1202_SCK_PORT, N1202_SCK_PIN, GPIO_PIN_SET);
	for (uint8_t i=0; i<8; i++) {
		HAL_GPIO_WritePin(N1202_SCK_PORT, N1202_SCK_PIN, GPIO_PIN_RESET);
		if (c & 0x80) {
			HAL_GPIO_WritePin(N1202_MOSI_PORT, N1202_MOSI_PIN, GPIO_PIN_SET);
		} else {
			HAL_GPIO_WritePin(N1202_MOSI_PORT, N1202_MOSI_PIN, GPIO_PIN_RESET);
		}
		HAL_GPIO_WritePin(N1202_SCK_PORT, N1202_SCK_PIN, GPIO_PIN_SET);
		c <<= 1;
	}
	HAL_GPIO_WritePin(N1202_SCK_PORT, N1202_SCK_PIN, GPIO_PIN_RESET);
}

void N1202_Clear() {
	for (uint32_t i=0; i < N1202_WIDTH * N1202_STRING; i++) {
		_N1202_RAM[i] = 0x00;
	}
}

void N1202_Update() {
	for (uint8_t l=0; l < N1202_STRING; l++) {
		N1202_SendByte(N1202_CMD, N1202_SET_Y_ADDR | l);
		N1202_SendByte(N1202_CMD, N1202_SET_X_ADDR4);
		N1202_SendByte(N1202_CMD, N1202_SET_X_ADDR3);
		for (uint8_t x=0; x < N1202_WIDTH; x++) {
			N1202_SendByte(N1202_DATA, _N1202_RAM[(N1202_WIDTH * l) + x]);
		}
	}
}

void N1202_DrawPixel(uint8_t x, uint8_t y, uint8_t color) {
	if (x > N1202_WIDTH || y > N1202_HEIGHT) {
		return;
	}

	if (color) {
		_N1202_RAM[x + (y / 8) * N1202_WIDTH] |= 1 << (y % 8);
	} else {
		_N1202_RAM[x + (y / 8) * N1202_WIDTH] &= ~(1 << (y % 8));
	}
}

void N1202_DrawLine(uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, uint8_t color) {
	int steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}
	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);
	int err = dx / 2;
	int ystep;
	if (y0 - y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			N1202_DrawPixel(y0, x0, color);
		} else {
			N1202_DrawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}

void N1202_DrawVLine(uint8_t x, uint8_t y, uint8_t height, uint8_t color) {
	N1202_DrawLine(x, y, x, y + height - 1, color);
}

void N1202_DrawHLine(uint8_t x, uint8_t y, uint8_t width, uint8_t color) {
	N1202_DrawLine(x, y, x + width - 1, y, color);
}

void N1202_DrawRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
	N1202_DrawHLine(x, y, width, color);
	N1202_DrawHLine(x, y + height - 1, width, color);
	N1202_DrawVLine(x, y, height, color);
	N1202_DrawVLine(x + width - 1, y, height, color);
}

void N1202_DrawFillRect(uint8_t x, uint8_t y, uint8_t width, uint8_t height, uint8_t color) {
	for (uint8_t i=x; i < x + width; i++) {
		N1202_DrawVLine(i, y, height, color);
	}
}

void N1202_Init() {
	HAL_GPIO_WritePin(N1202_RST_PORT, N1202_RST_PIN, GPIO_PIN_RESET);
	HAL_Delay(5);
	HAL_GPIO_WritePin(N1202_RST_PORT, N1202_RST_PIN, GPIO_PIN_SET);

	HAL_GPIO_WritePin(N1202_SCK_PORT, N1202_SCK_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(N1202_MOSI_PORT, N1202_MOSI_PIN, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(N1202_CS_PORT, N1202_CS_PIN, GPIO_PIN_RESET);

	HAL_Delay(5);
	HAL_GPIO_WritePin(N1202_CS_PORT, N1202_CS_PIN, GPIO_PIN_SET);

	N1202_SendByte(N1202_CMD, 0xE2);  // reset
	HAL_Delay(5);
	N1202_SendByte(N1202_CMD, 0x3D);
	N1202_SendByte(N1202_CMD, 0x02);
	N1202_SendByte(N1202_CMD, 0xE1);
	N1202_SendByte(N1202_CMD, 0x90);
	N1202_SendByte(N1202_CMD, 0xA4);
	N1202_SendByte(N1202_CMD, 0x2F);
	N1202_SendByte(N1202_CMD, 0xA0);
	N1202_SendByte(N1202_CMD, 0xAF);

	HAL_Delay(10);
	N1202_Clear();
	N1202_Update();
}
