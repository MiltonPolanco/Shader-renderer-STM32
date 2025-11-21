#ifndef RENDERER_H
#define RENDERER_H

#include "../SolarSystem/camera.h"
#include <stdint.h>

typedef struct {
    int16_t x;
    int16_t y;
    uint8_t brightness;
} Star;

void Renderer_Init(void);
void Renderer_DrawStars(uint32_t seed, uint8_t count);

#endif
