#ifndef LCD_DRIVER_H
#define LCD_DRIVER_H

#include "main.h"
#include <stdint.h>

// Configuración del LCD
#define LCD_WIDTH  320
#define LCD_HEIGHT 240

// Comandos LCD
#define LCD_CMD_SWRESET     0x01
#define LCD_CMD_SLPOUT      0x11
#define LCD_CMD_DISPON      0x29
#define LCD_CMD_CASET       0x2A
#define LCD_CMD_PASET       0x2B
#define LCD_CMD_RAMWR       0x2C
#define LCD_CMD_MADCTL      0x36
#define LCD_CMD_COLMOD      0x3A

// Colores RGB565
#define COLOR_BLACK       0x0000
#define COLOR_WHITE       0xFFFF
#define COLOR_RED         0xF800
#define COLOR_GREEN       0x07E0
#define COLOR_BLUE        0x001F
#define COLOR_YELLOW      0xFFE0
#define COLOR_CYAN        0x07FF
#define COLOR_MAGENTA     0xF81F
#define COLOR_ORANGE      0xFD20
#define COLOR_GRAY        0x8410

// Colores del sistema solar
#define COLOR_SUN         0xFEA0
#define COLOR_MERCURY     0x8410
#define COLOR_VENUS       0xFFE0
#define COLOR_EARTH       0x047F
#define COLOR_MARS        0xF800
#define COLOR_JUPITER     0xFD40
#define COLOR_SATURN      0xFE80
#define COLOR_URANUS      0x067F
#define COLOR_NEPTUNE     0x001F
#define COLOR_SPACE       0x0000
#define COLOR_STAR        0xFFFF

// Macros para control de pines
#define LCD_CS_LOW()    HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_RESET)
#define LCD_CS_HIGH()   HAL_GPIO_WritePin(LCD_CS_GPIO_Port, LCD_CS_Pin, GPIO_PIN_SET)
#define LCD_RS_LOW()    HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET)
#define LCD_RS_HIGH()   HAL_GPIO_WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET)
#define LCD_WR_LOW()    HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_RESET)
#define LCD_WR_HIGH()   HAL_GPIO_WritePin(LCD_WR_GPIO_Port, LCD_WR_Pin, GPIO_PIN_SET)
#define LCD_RD_LOW()    HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_RESET)
#define LCD_RD_HIGH()   HAL_GPIO_WritePin(LCD_RD_GPIO_Port, LCD_RD_Pin, GPIO_PIN_SET)
#define LCD_RST_LOW()   HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_RESET)
#define LCD_RST_HIGH()  HAL_GPIO_WritePin(LCD_RST_GPIO_Port, LCD_RST_Pin, GPIO_PIN_SET)

// Funciones públicas del LCD
void LCD_Init(void);
void LCD_Clear(uint16_t color);
void LCD_DrawPixel(int16_t x, int16_t y, uint16_t color);
void LCD_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color);
void LCD_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void LCD_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color);
void LCD_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color);
void LCD_DrawHLine(int16_t x, int16_t y, int16_t w, uint16_t color);
void LCD_DrawVLine(int16_t x, int16_t y, int16_t h, uint16_t color);

#endif // LCD_DRIVER_H
