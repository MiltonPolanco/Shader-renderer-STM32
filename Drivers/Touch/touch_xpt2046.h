#ifndef TOUCH_XPT2046_H
#define TOUCH_XPT2046_H

#include "main.h"
#include <stdint.h>

#define TOUCH_CS_LOW()   HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET)
#define TOUCH_CS_HIGH()  HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET)

#define XPT2046_CMD_X    0x90
#define XPT2046_CMD_Y    0xD0

#define TS_MINX 200
#define TS_MAXX 3800
#define TS_MINY 200
#define TS_MAXY 3800

typedef struct {
    int16_t x;
    int16_t y;
    uint8_t pressed;
} TouchPoint;

void Touch_Init(SPI_HandleTypeDef* hspi);
uint8_t Touch_Read(TouchPoint* point);
uint16_t Touch_ReadRaw(uint8_t command);

#endif
