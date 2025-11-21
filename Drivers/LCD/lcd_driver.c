#include "lcd_driver.h"
#include <stdlib.h>

static uint16_t lcd_width = LCD_WIDTH;
static uint16_t lcd_height = LCD_HEIGHT;

// Escribir byte en el bus de datos paralelo
static void LCD_WriteDataBus(uint8_t data)
{
    HAL_GPIO_WritePin(LCD_D0_GPIO_Port, LCD_D0_Pin, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D1_GPIO_Port, LCD_D1_Pin, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D2_GPIO_Port, LCD_D2_Pin, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D3_GPIO_Port, LCD_D3_Pin, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D4_GPIO_Port, LCD_D4_Pin, (data & 0x10) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D5_GPIO_Port, LCD_D5_Pin, (data & 0x20) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D6_GPIO_Port, LCD_D6_Pin, (data & 0x40) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(LCD_D7_GPIO_Port, LCD_D7_Pin, (data & 0x80) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LCD_WR_LOW();
    __NOP(); __NOP(); __NOP(); __NOP();
    LCD_WR_HIGH();
    __NOP(); __NOP();
}

static void LCD_WriteCommand(uint8_t cmd)
{
    LCD_CS_LOW();
    LCD_RS_LOW();
    LCD_WriteDataBus(cmd);
    LCD_CS_HIGH();
}

static void LCD_WriteData(uint8_t data)
{
    LCD_CS_LOW();
    LCD_RS_HIGH();
    LCD_WriteDataBus(data);
    LCD_CS_HIGH();
}

static void LCD_WriteData16(uint16_t data)
{
    LCD_CS_LOW();
    LCD_RS_HIGH();
    LCD_WriteDataBus(data >> 8);
    LCD_WriteDataBus(data & 0xFF);
    LCD_CS_HIGH();
}

static void LCD_SetWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    if (x1 >= lcd_width) x1 = lcd_width - 1;
    if (y1 >= lcd_height) y1 = lcd_height - 1;

    LCD_WriteCommand(LCD_CMD_CASET);
    LCD_WriteData(x0 >> 8);
    LCD_WriteData(x0 & 0xFF);
    LCD_WriteData(x1 >> 8);
    LCD_WriteData(x1 & 0xFF);

    LCD_WriteCommand(LCD_CMD_PASET);
    LCD_WriteData(y0 >> 8);
    LCD_WriteData(y0 & 0xFF);
    LCD_WriteData(y1 >> 8);
    LCD_WriteData(y1 & 0xFF);

    LCD_WriteCommand(LCD_CMD_RAMWR);
}

void LCD_Init(void)
{
    LCD_RD_HIGH();
    LCD_CS_HIGH();

    // Reset Hardware
    LCD_RST_HIGH();
    HAL_Delay(10);
    LCD_RST_LOW();
    HAL_Delay(20);
    LCD_RST_HIGH();
    HAL_Delay(120);

    // Software Reset
    LCD_WriteCommand(0x01);
    HAL_Delay(150);

    // Power Control A
    LCD_WriteCommand(0xCB);
    LCD_WriteData(0x39);
    LCD_WriteData(0x2C);
    LCD_WriteData(0x00);
    LCD_WriteData(0x34);
    LCD_WriteData(0x02);

    // Power Control B
    LCD_WriteCommand(0xCF);
    LCD_WriteData(0x00);
    LCD_WriteData(0xC1);
    LCD_WriteData(0x30);

    // Driver timing control A
    LCD_WriteCommand(0xE8);
    LCD_WriteData(0x85);
    LCD_WriteData(0x00);
    LCD_WriteData(0x78);

    // Driver timing control B
    LCD_WriteCommand(0xEA);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);

    // Power on sequence control
    LCD_WriteCommand(0xED);
    LCD_WriteData(0x64);
    LCD_WriteData(0x03);
    LCD_WriteData(0x12);
    LCD_WriteData(0x81);

    // Pump ratio control
    LCD_WriteCommand(0xF7);
    LCD_WriteData(0x20);

    // Power Control 1
    LCD_WriteCommand(0xC0);
    LCD_WriteData(0x23);

    // Power Control 2
    LCD_WriteCommand(0xC1);
    LCD_WriteData(0x10);

    // VCOM Control 1
    LCD_WriteCommand(0xC5);
    LCD_WriteData(0x3e);
    LCD_WriteData(0x28);

    // VCOM Control 2
    LCD_WriteCommand(0xC7);
    LCD_WriteData(0x86);

    // Memory Access Control
    LCD_WriteCommand(LCD_CMD_MADCTL);
    LCD_WriteData(0x28);

    // Pixel Format Set
    LCD_WriteCommand(LCD_CMD_COLMOD);
    LCD_WriteData(0x55);

    // Frame Rate Control
    LCD_WriteCommand(0xB1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x1B);

    // Display Function Control
    LCD_WriteCommand(0xB6);
    LCD_WriteData(0x0A);
    LCD_WriteData(0xA2);
    LCD_WriteData(0x27);
    LCD_WriteData(0x00);

    // Enable 3 gamma control
    LCD_WriteCommand(0xF2);
    LCD_WriteData(0x00);

    // Gamma Set
    LCD_WriteCommand(0x26);
    LCD_WriteData(0x01);

    // Positive Gamma Correction
    LCD_WriteCommand(0xE0);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x31);
    LCD_WriteData(0x2B);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x08);
    LCD_WriteData(0x4E);
    LCD_WriteData(0xF1);
    LCD_WriteData(0x37);
    LCD_WriteData(0x07);
    LCD_WriteData(0x10);
    LCD_WriteData(0x03);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x09);
    LCD_WriteData(0x00);

    // Negative Gamma Correction
    LCD_WriteCommand(0xE1);
    LCD_WriteData(0x00);
    LCD_WriteData(0x0E);
    LCD_WriteData(0x14);
    LCD_WriteData(0x03);
    LCD_WriteData(0x11);
    LCD_WriteData(0x07);
    LCD_WriteData(0x31);
    LCD_WriteData(0xC1);
    LCD_WriteData(0x48);
    LCD_WriteData(0x08);
    LCD_WriteData(0x0F);
    LCD_WriteData(0x0C);
    LCD_WriteData(0x31);
    LCD_WriteData(0x36);
    LCD_WriteData(0x0F);

    // Column Address Set
    LCD_WriteCommand(LCD_CMD_CASET);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData((lcd_width - 1) >> 8);
    LCD_WriteData((lcd_width - 1) & 0xFF);

    // Page Address Set
    LCD_WriteCommand(LCD_CMD_PASET);
    LCD_WriteData(0x00);
    LCD_WriteData(0x00);
    LCD_WriteData((lcd_height - 1) >> 8);
    LCD_WriteData((lcd_height - 1) & 0xFF);

    // Sleep Out
    LCD_WriteCommand(LCD_CMD_SLPOUT);
    HAL_Delay(120);

    // Display ON
    LCD_WriteCommand(LCD_CMD_DISPON);
    HAL_Delay(50);

    // Limpieza inicial
    for (int i = 0; i < 3; i++) {
        LCD_Clear(COLOR_BLACK);
        HAL_Delay(20);
    }
}

void LCD_Clear(uint16_t color)
{
    LCD_SetWindow(0, 0, lcd_width - 1, lcd_height - 1);

    LCD_CS_LOW();
    LCD_RS_HIGH();

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    for (uint32_t i = 0; i < (uint32_t)lcd_width * lcd_height; i++) {
        LCD_WriteDataBus(hi);
        LCD_WriteDataBus(lo);
    }

    LCD_CS_HIGH();
}

void LCD_DrawPixel(int16_t x, int16_t y, uint16_t color)
{
    if (x < 0 || x >= lcd_width || y < 0 || y >= lcd_height)
        return;

    LCD_SetWindow(x, y, x, y);
    LCD_WriteData16(color);
}

void LCD_FillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
    if (x >= lcd_width || y >= lcd_height) return;
    if (x < 0) { w += x; x = 0; }
    if (y < 0) { h += y; y = 0; }
    if (x + w > lcd_width) w = lcd_width - x;
    if (y + h > lcd_height) h = lcd_height - y;
    if (w <= 0 || h <= 0) return;

    LCD_SetWindow(x, y, x + w - 1, y + h - 1);

    LCD_CS_LOW();
    LCD_RS_HIGH();

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    for (int32_t i = 0; i < (int32_t)w * h; i++) {
        LCD_WriteDataBus(hi);
        LCD_WriteDataBus(lo);
    }

    LCD_CS_HIGH();
}

void LCD_DrawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color)
{
    int16_t dx = abs(x1 - x0);
    int16_t dy = abs(y1 - y0);
    int16_t sx = (x0 < x1) ? 1 : -1;
    int16_t sy = (y0 < y1) ? 1 : -1;
    int16_t err = dx - dy;

    while (1) {
        LCD_DrawPixel(x0, y0, color);

        if (x0 == x1 && y0 == y1) break;

        int16_t e2 = 2 * err;
        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }
        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

void LCD_DrawCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    int16_t f = 1 - r;
    int16_t ddF_x = 1;
    int16_t ddF_y = -2 * r;
    int16_t x = 0;
    int16_t y = r;

    LCD_DrawPixel(x0, y0 + r, color);
    LCD_DrawPixel(x0, y0 - r, color);
    LCD_DrawPixel(x0 + r, y0, color);
    LCD_DrawPixel(x0 - r, y0, color);

    while (x < y) {
        if (f >= 0) {
            y--;
            ddF_y += 2;
            f += ddF_y;
        }
        x++;
        ddF_x += 2;
        f += ddF_x;

        LCD_DrawPixel(x0 + x, y0 + y, color);
        LCD_DrawPixel(x0 - x, y0 + y, color);
        LCD_DrawPixel(x0 + x, y0 - y, color);
        LCD_DrawPixel(x0 - x, y0 - y, color);
        LCD_DrawPixel(x0 + y, y0 + x, color);
        LCD_DrawPixel(x0 - y, y0 + x, color);
        LCD_DrawPixel(x0 + y, y0 - x, color);
        LCD_DrawPixel(x0 - y, y0 - x, color);
    }
}

// NUEVA FUNCIÓN - FillCircle con clipping correcto
void LCD_FillCircle(int16_t x0, int16_t y0, int16_t r, uint16_t color)
{
    // Dibujar usando líneas verticales con clipping
    for (int16_t y = -r; y <= r; y++) {
        int16_t x_extent = (int16_t)sqrtf(r * r - y * y);

        int16_t y_pos = y0 + y;
        int16_t x_start = x0 - x_extent;
        int16_t x_end = x0 + x_extent;

        // Clip Y
        if (y_pos < 0 || y_pos >= lcd_height) continue;

        // Clip X
        if (x_start < 0) x_start = 0;
        if (x_end >= lcd_width) x_end = lcd_width - 1;

        // Si después de clip no hay nada que dibujar, skip
        if (x_start > x_end) continue;

        // Dibujar línea horizontal
        LCD_DrawHLine(x_start, y_pos, x_end - x_start + 1, color);
    }
}

void LCD_DrawHLine(int16_t x, int16_t y, int16_t w, uint16_t color)
{
    LCD_FillRect(x, y, w, 1, color);
}

void LCD_DrawVLine(int16_t x, int16_t y, int16_t h, uint16_t color)
{
    LCD_FillRect(x, y, 1, h, color);
}
