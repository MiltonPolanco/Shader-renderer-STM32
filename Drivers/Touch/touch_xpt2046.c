#include "touch_xpt2046.h"

static SPI_HandleTypeDef* touch_spi;

void Touch_Init(SPI_HandleTypeDef* hspi)
{
    touch_spi = hspi;
    TOUCH_CS_HIGH();
    HAL_Delay(100);
}

uint16_t Touch_ReadRaw(uint8_t command)
{
    uint8_t tx[3] = {command, 0x00, 0x00};
    uint8_t rx[3] = {0};

    TOUCH_CS_LOW();
    HAL_Delay(1);
    HAL_SPI_TransmitReceive(touch_spi, tx, rx, 3, 1000);
    HAL_Delay(1);
    TOUCH_CS_HIGH();

    uint16_t value = ((rx[1] << 8) | rx[2]) >> 3;
    return value;
}

uint8_t Touch_Read(TouchPoint* point)
{
    uint16_t x1 = Touch_ReadRaw(XPT2046_CMD_X);
    HAL_Delay(5);
    uint16_t x2 = Touch_ReadRaw(XPT2046_CMD_X);
    HAL_Delay(5);
    uint16_t x3 = Touch_ReadRaw(XPT2046_CMD_X);

    uint16_t y1 = Touch_ReadRaw(XPT2046_CMD_Y);
    HAL_Delay(5);
    uint16_t y2 = Touch_ReadRaw(XPT2046_CMD_Y);
    HAL_Delay(5);
    uint16_t y3 = Touch_ReadRaw(XPT2046_CMD_Y);

    uint16_t x_raw = (x1 + x2 + x3) / 3;
    uint16_t y_raw = (y1 + y2 + y3) / 3;

    if (x_raw < 100 || x_raw > 4000 || y_raw < 100 || y_raw > 4000) {
        point->pressed = 0;
        return 0;
    }

    point->x = (int16_t)((x_raw - TS_MINX) * 320 / (TS_MAXX - TS_MINX));
    point->y = (int16_t)((y_raw - TS_MINY) * 240 / (TS_MAXY - TS_MINY));

    if (point->x < 0) point->x = 0;
    if (point->x >= 320) point->x = 319;
    if (point->y < 0) point->y = 0;
    if (point->y >= 240) point->y = 239;

    point->pressed = 1;
    return 1;
}
