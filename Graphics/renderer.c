#include "renderer.h"
#include "../Drivers/LCD/lcd_driver.h"
#include <stdlib.h>

static Star stars[100];
static uint8_t stars_initialized = 0;

void Renderer_Init(void)
{
    stars_initialized = 0;
}

void Renderer_DrawStars(uint32_t seed, uint8_t count)
{
    if (count > 100) count = 100;

    if (!stars_initialized) {
        srand(seed);

        for (uint8_t i = 0; i < count; i++) {
            stars[i].x = rand() % LCD_WIDTH;
            stars[i].y = rand() % LCD_HEIGHT;
            stars[i].brightness = 128 + (rand() % 128);
        }

        stars_initialized = 1;
    }

    for (uint8_t i = 0; i < count; i++) {
        uint8_t b = stars[i].brightness;
        uint16_t star_color = ((b >> 3) << 11) | ((b >> 2) << 5) | (b >> 3);

        LCD_DrawPixel(stars[i].x, stars[i].y, star_color);

        if (i % 5 == 0) {
            LCD_DrawPixel(stars[i].x + 1, stars[i].y, star_color);
            LCD_DrawPixel(stars[i].x, stars[i].y + 1, star_color);
        }
    }
}
