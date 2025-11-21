#include "solar_system.h"
#include "../Drivers/LCD/lcd_driver.h"
#include <string.h>
#include <stdlib.h>
#include <math.h>

void SolarSystem_Init(SolarSystem* sys)
{
    sys->body_count = 0;
    sys->time_scale = 1.0f;
    sys->total_time = 0.0f;

    SolarSystem_CreateDefaultSystem(sys);
}

void SolarSystem_CreateDefaultSystem(SolarSystem* sys)
{
    CelestialBody body;

    CelestialBody_Init(&body, "Planet", BODY_TYPE_PLANET);
    CelestialBody_SetVisuals(&body, 60.0f, COLOR_EARTH);
    CelestialBody_SetRotation(&body, 0.2f);
    CelestialBody_SetShader(&body, SHADER_MERCURY);
    SolarSystem_AddBody(sys, body);
}

void SolarSystem_AddBody(SolarSystem* sys, CelestialBody body)
{
    if (sys->body_count < MAX_BODIES) {
        sys->bodies[sys->body_count] = body;
        sys->body_count++;
    }
}

void SolarSystem_Update(SolarSystem* sys, float deltaTime)
{
    float scaled_time = deltaTime * sys->time_scale;
    sys->total_time += scaled_time;

    for (uint8_t i = 0; i < sys->body_count; i++) {
        CelestialBody_Update(&sys->bodies[i], scaled_time);
    }
}

void SolarSystem_SortByDistance(SolarSystem* sys, Camera* cam)
{
    for (uint8_t i = 0; i < sys->body_count; i++) {
        sys->bodies[i].distance_to_camera =
            vec3_distance(cam->position, sys->bodies[i].position);
    }

    for (uint8_t i = 0; i < sys->body_count - 1; i++) {
        for (uint8_t j = 0; j < sys->body_count - i - 1; j++) {
            if (sys->bodies[j].distance_to_camera < sys->bodies[j + 1].distance_to_camera) {
                CelestialBody temp = sys->bodies[j];
                sys->bodies[j] = sys->bodies[j + 1];
                sys->bodies[j + 1] = temp;
            }
        }
    }
}

void SolarSystem_Render(SolarSystem* sys, Camera* cam)
{
    SolarSystem_SortByDistance(sys, cam);

    for (uint8_t i = 0; i < sys->body_count; i++) {
        CelestialBody* body = &sys->bodies[i];

        Vector2 screen_pos = Camera_WorldToScreen(cam, body->position, LCD_WIDTH, LCD_HEIGHT);

        body->screen_x = (int16_t)screen_pos.x;
        body->screen_y = (int16_t)screen_pos.y;

        body->screen_radius = (int16_t)(body->radius * 1.2f);

        if (body->screen_radius < 2) body->screen_radius = 2;
        if (body->screen_radius > 100) body->screen_radius = 100;

        body->is_visible = 1;

        CelestialBody_Render(body);
    }
}

void SolarSystem_RenderWithShaders(SolarSystem* sys, Camera* cam, float time)
{
    SolarSystem_SortByDistance(sys, cam);

    for (uint8_t i = 0; i < sys->body_count; i++) {
        CelestialBody* body = &sys->bodies[i];

        Vector2 screen_pos = Camera_WorldToScreen(cam, body->position, LCD_WIDTH, LCD_HEIGHT);

        body->screen_x = (int16_t)screen_pos.x;
        body->screen_y = (int16_t)screen_pos.y;

        body->screen_radius = (int16_t)(body->radius * 1.2f);

        if (body->screen_radius < 2) body->screen_radius = 2;
        if (body->screen_radius > 100) body->screen_radius = 100;

        body->is_visible = 1;

        CelestialBody_RenderWithShader(body, time);
    }
}

void SolarSystem_RenderOrbits(SolarSystem* sys, Camera* cam)
{
    for (uint8_t i = 0; i < sys->body_count; i++) {
        CelestialBody* body = &sys->bodies[i];

        if (body->type != BODY_TYPE_PLANET) continue;
        if (body->orbit_radius < 1.0f) continue;

        uint16_t orbit_color = 0x632C;

        int num_points = 100;

        for (int j = 0; j < num_points; j++) {
            float angle = (float)j / (float)num_points * TWO_PI;

            Vector3 orbit_point;
            orbit_point.x = body->orbit_radius * cosf(angle);
            orbit_point.z = body->orbit_radius * sinf(angle);
            orbit_point.y = 0.0f;

            Vector2 screen_pos = Camera_WorldToScreen(cam, orbit_point, LCD_WIDTH, LCD_HEIGHT);

            int16_t sx = (int16_t)screen_pos.x;
            int16_t sy = (int16_t)screen_pos.y;

            LCD_DrawPixel(sx, sy, orbit_color);
            LCD_DrawPixel(sx + 1, sy, orbit_color);
        }
    }
}

CelestialBody* SolarSystem_GetBodyByName(SolarSystem* sys, const char* name)
{
    for (uint8_t i = 0; i < sys->body_count; i++) {
        if (strcmp(sys->bodies[i].name, name) == 0) {
            return &sys->bodies[i];
        }
    }
    return NULL;
}

CelestialBody* SolarSystem_GetBodyByIndex(SolarSystem* sys, uint8_t index)
{
    if (index < sys->body_count) {
        return &sys->bodies[index];
    }
    return NULL;
}

void SolarSystem_SetPlanetShader(SolarSystem* sys, ShaderType shader)
{
    if (sys->body_count > 0) {
        sys->bodies[0].shader_type = shader;
    }
}
