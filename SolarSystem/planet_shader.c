#include "planet_shader.h"
#include <math.h>

uint16_t RGB_To_RGB565(uint8_t r, uint8_t g, uint8_t b)
{
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

float Noise(float x, float y)
{
    int n = (int)(x * 1000.0f) + (int)(y * 1000.0f) * 57;
    n = (n << 13) ^ n;
    float result = (1.0f - ((n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0f);
    return result * 0.5f + 0.5f;
}

float SmoothNoise(float x, float y)
{
    float corners = (Noise(x-1, y-1) + Noise(x+1, y-1) + Noise(x-1, y+1) + Noise(x+1, y+1)) / 16.0f;
    float sides = (Noise(x-1, y) + Noise(x+1, y) + Noise(x, y-1) + Noise(x, y+1)) / 8.0f;
    float center = Noise(x, y) / 4.0f;
    return corners + sides + center;
}

float InterpolatedNoise(float x, float y)
{
    int integer_X = (int)x;
    float fractional_X = x - integer_X;

    int integer_Y = (int)y;
    float fractional_Y = y - integer_Y;

    float v1 = SmoothNoise(integer_X, integer_Y);
    float v2 = SmoothNoise(integer_X + 1, integer_Y);
    float v3 = SmoothNoise(integer_X, integer_Y + 1);
    float v4 = SmoothNoise(integer_X + 1, integer_Y + 1);

    float i1 = v1 * (1.0f - fractional_X) + v2 * fractional_X;
    float i2 = v3 * (1.0f - fractional_X) + v4 * fractional_X;

    return i1 * (1.0f - fractional_Y) + i2 * fractional_Y;
}

float FBM(float x, float y, int octaves)
{
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;

    for (int i = 0; i < octaves; i++) {
        total += InterpolatedNoise(x * frequency, y * frequency) * amplitude;
        maxValue += amplitude;
        amplitude *= 0.5f;
        frequency *= 2.0f;
    }

    return total / maxValue;
}

float Smoothstep(float edge0, float edge1, float x)
{
    float t = (x - edge0) / (edge1 - edge0);
    if (t < 0.0f) t = 0.0f;
    if (t > 1.0f) t = 1.0f;
    return t * t * (3.0f - 2.0f * t);
}

uint16_t Shader_Mercury(ShaderInput* input)
{
    float light = vec3_dot(input->normal, vec3_normalize(vec3_create(1, 1, 1)));
    light = (light + 1.0f) * 0.5f;
    light = Smoothstep(0.0f, 1.0f, light);

    float crater = FBM(input->position.x * 8.0f, input->position.z * 8.0f, 4);
    crater = Smoothstep(0.3f, 0.7f, crater);

    float detail = FBM(input->position.x * 20.0f, input->position.z * 20.0f, 2) * 0.1f;

    float base = 0.5f + crater * 0.3f + detail;

    uint8_t gray = (uint8_t)(base * light * 180.0f + 40.0f);
    return RGB_To_RGB565(gray, gray * 0.95f, gray * 0.9f);
}

uint16_t Shader_Venus(ShaderInput* input)
{
    float light = vec3_dot(input->normal, vec3_normalize(vec3_create(1, 1, 1)));
    light = (light + 1.0f) * 0.5f;
    light = Smoothstep(0.0f, 1.0f, light);

    float clouds1 = FBM(input->position.x * 4.0f + input->time * 0.05f,
                        input->position.z * 4.0f, 5);
    float clouds2 = FBM(input->position.x * 8.0f - input->time * 0.03f,
                        input->position.z * 8.0f, 3) * 0.5f;

    float clouds = clouds1 + clouds2;
    clouds = Smoothstep(0.3f, 0.8f, clouds);

    float yellow = 0.7f + clouds * 0.3f;

    uint8_t r = (uint8_t)(yellow * light * 250.0f);
    uint8_t g = (uint8_t)(yellow * light * 220.0f);
    uint8_t b = (uint8_t)(yellow * light * 120.0f);

    return RGB_To_RGB565(r, g, b);
}

uint16_t Shader_Earth(ShaderInput* input)
{
    float light = vec3_dot(input->normal, vec3_normalize(vec3_create(1, 1, 1)));
    light = (light + 1.0f) * 0.5f;
    light = Smoothstep(0.0f, 1.0f, light);

    float continents = FBM(input->position.x * 5.0f, input->position.z * 5.0f, 5);
    continents = Smoothstep(0.35f, 0.55f, continents);

    uint8_t r, g, b;

    if (continents > 0.5f) {
        float green_variation = FBM(input->position.x * 15.0f, input->position.z * 15.0f, 2);
        r = (uint8_t)(30.0f * light + green_variation * 20.0f);
        g = (uint8_t)((120.0f + green_variation * 40.0f) * light);
        b = (uint8_t)(30.0f * light);
    } else {
        float ocean_depth = FBM(input->position.x * 12.0f, input->position.z * 12.0f, 3);
        r = (uint8_t)(10.0f * light);
        g = (uint8_t)((80.0f + ocean_depth * 30.0f) * light);
        b = (uint8_t)((140.0f + ocean_depth * 40.0f) * light);
    }

    float clouds = FBM(input->position.x * 10.0f + input->time * 0.1f,
                       input->position.z * 10.0f, 3);
    clouds = Smoothstep(0.55f, 0.75f, clouds);

    if (clouds > 0.5f) {
        float cloud_alpha = (clouds - 0.5f) * 2.0f;
        r = (uint8_t)(r * (1.0f - cloud_alpha) + 240.0f * cloud_alpha * light);
        g = (uint8_t)(g * (1.0f - cloud_alpha) + 240.0f * cloud_alpha * light);
        b = (uint8_t)(b * (1.0f - cloud_alpha) + 250.0f * cloud_alpha * light);
    }

    return RGB_To_RGB565(r, g, b);
}

uint16_t Shader_Jupiter(ShaderInput* input)
{
    float light = vec3_dot(input->normal, vec3_normalize(vec3_create(1, 1, 1)));
    light = (light + 1.0f) * 0.5f;
    light = Smoothstep(0.0f, 1.0f, light);

    float bands = sinf(input->position.y * 10.0f + input->time * 0.05f) * 0.5f + 0.5f;
    bands = Smoothstep(0.2f, 0.8f, bands);

    float turbulence = FBM(input->position.x * 8.0f + input->time * 0.02f,
                           input->position.y * 3.0f, 5);

    float storms = FBM(input->position.x * 15.0f, input->position.y * 15.0f, 3);
    storms = Smoothstep(0.6f, 0.8f, storms) * 0.3f;

    float combined = bands * 0.6f + turbulence * 0.3f + storms;

    uint8_t r = (uint8_t)((180.0f + combined * 75.0f) * light);
    uint8_t g = (uint8_t)((130.0f + combined * 60.0f) * light);
    uint8_t b = (uint8_t)((80.0f + combined * 40.0f) * light);

    return RGB_To_RGB565(r, g, b);
}

uint16_t Shader_Saturn(ShaderInput* input)
{
    float light = vec3_dot(input->normal, vec3_normalize(vec3_create(1, 1, 1)));
    light = (light + 1.0f) * 0.5f;
    light = Smoothstep(0.0f, 1.0f, light);

    float bands = sinf(input->position.y * 8.0f) * 0.5f + 0.5f;
    bands = Smoothstep(0.3f, 0.7f, bands);

    float detail = FBM(input->position.x * 10.0f, input->position.y * 5.0f, 4) * 0.2f;

    float color_var = bands + detail;

    uint8_t r = (uint8_t)((220.0f + color_var * 35.0f) * light);
    uint8_t g = (uint8_t)((190.0f + color_var * 30.0f) * light);
    uint8_t b = (uint8_t)((140.0f + color_var * 25.0f) * light);

    return RGB_To_RGB565(r, g, b);
}

uint16_t Shader_Neptune(ShaderInput* input)
{
    float light = vec3_dot(input->normal, vec3_normalize(vec3_create(1, 1, 1)));
    light = (light + 1.0f) * 0.5f;
    light = Smoothstep(0.0f, 1.0f, light);

    float energy = FBM(input->position.x * 6.0f + input->time * 0.2f,
                       input->position.z * 6.0f + input->time * 0.15f, 5);

    float storms = FBM(input->position.x * 12.0f - input->time * 0.1f,
                       input->position.y * 12.0f, 4);
    storms = Smoothstep(0.5f, 0.8f, storms);

    float glow = sinf(input->time * 1.5f + energy * 6.28f) * 0.5f + 0.5f;
    glow = Smoothstep(0.3f, 0.7f, glow);

    float combined = energy * 0.5f + storms * 0.3f + glow * 0.2f;

    uint8_t r = (uint8_t)((20.0f + combined * 80.0f) * light);
    uint8_t g = (uint8_t)((80.0f + combined * 100.0f) * light);
    uint8_t b = (uint8_t)((180.0f + combined * 75.0f) * light);

    return RGB_To_RGB565(r, g, b);
}
