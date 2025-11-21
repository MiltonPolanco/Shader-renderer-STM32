#ifndef PLANET_SHADER_H
#define PLANET_SHADER_H

#include <stdint.h>
#include "../Utils/math3d.h"

typedef struct {
    Vector3 position;
    Vector3 normal;
    float time;
} ShaderInput;

uint16_t Shader_Mercury(ShaderInput* input);
uint16_t Shader_Venus(ShaderInput* input);
uint16_t Shader_Earth(ShaderInput* input);
uint16_t Shader_Jupiter(ShaderInput* input);
uint16_t Shader_Saturn(ShaderInput* input);
uint16_t Shader_Neptune(ShaderInput* input);

uint16_t RGB_To_RGB565(uint8_t r, uint8_t g, uint8_t b);
float Noise(float x, float y);
float FBM(float x, float y, int octaves);

#endif
