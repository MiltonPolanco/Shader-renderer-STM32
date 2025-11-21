#include "celestial_body.h"
#include "planet_shader.h"
#include "../Drivers/LCD/lcd_driver.h"
#include <string.h>
#include <math.h>

void CelestialBody_Init(CelestialBody* body, const char* name, BodyType type)
{
    strncpy(body->name, name, MAX_NAME_LENGTH - 1);
    body->name[MAX_NAME_LENGTH - 1] = '\0';
    body->type = type;

    body->position = vec3_create(0, 0, 0);
    body->radius = 1.0f;
    body->mass = 1.0f;

    body->orbit_radius = 0.0f;
    body->orbit_speed = 0.0f;
    body->orbit_angle = 0.0f;
    body->orbit_tilt = 0.0f;

    body->rotation_angle = 0.0f;
    body->rotation_speed = 0.0f;

    body->color = 0xFFFF;
    body->shader_type = SHADER_MERCURY;

    body->parent = NULL;

    body->screen_x = 0;
    body->screen_y = 0;
    body->screen_radius = 0;
    body->is_visible = 0;
    body->distance_to_camera = 0.0f;

    body->prev_screen_x = -1000;
    body->prev_screen_y = -1000;
    body->prev_screen_radius = 0;
}

void CelestialBody_SetVisuals(CelestialBody* body, float radius, uint16_t color)
{
    body->radius = radius;
    body->color = color;
}

void CelestialBody_SetShader(CelestialBody* body, ShaderType shader)
{
    body->shader_type = shader;
}

void CelestialBody_SetOrbitalParams(CelestialBody* body, float orbit_radius, float orbit_speed, float orbit_tilt)
{
    body->orbit_radius = orbit_radius;
    body->orbit_speed = orbit_speed;
    body->orbit_tilt = orbit_tilt;
}

void CelestialBody_SetRotation(CelestialBody* body, float rotation_speed)
{
    body->rotation_speed = rotation_speed;
}

void CelestialBody_SetParent(CelestialBody* body, CelestialBody* parent)
{
    body->parent = parent;
}

void CelestialBody_Update(CelestialBody* body, float deltaTime)
{
    body->orbit_angle += body->orbit_speed * deltaTime;
    if (body->orbit_angle > TWO_PI) {
        body->orbit_angle -= TWO_PI;
    }

    if (body->orbit_radius > 0.0f) {
        Vector3 orbit_pos;
        orbit_pos.x = body->orbit_radius * cosf(body->orbit_angle);
        orbit_pos.z = body->orbit_radius * sinf(body->orbit_angle);
        orbit_pos.y = body->orbit_radius * sinf(body->orbit_tilt) * sinf(body->orbit_angle);

        if (body->parent != NULL) {
            body->position = vec3_add(body->parent->position, orbit_pos);
        } else {
            body->position = orbit_pos;
        }
    }

    body->rotation_angle += body->rotation_speed * deltaTime;
    if (body->rotation_angle > TWO_PI) {
        body->rotation_angle -= TWO_PI;
    }
}

void CelestialBody_RenderWithShader(CelestialBody* body, float time)
{
    if (!body->is_visible) return;

    int16_t r = body->screen_radius;

    for (int16_t dy = -r; dy <= r; dy++) {
        for (int16_t dx = -r; dx <= r; dx++) {
            float dist = sqrtf((float)(dx*dx + dy*dy));
            if (dist > r) continue;

            float z = sqrtf(r*r - dx*dx - dy*dy);

            Vector3 pos;
            pos.x = (float)dx / (float)r;
            pos.y = (float)dy / (float)r;
            pos.z = z / (float)r;

            Vector3 normal = vec3_normalize(pos);

            ShaderInput input;
            input.position = pos;
            input.normal = normal;
            input.time = time;

            uint16_t color;
            switch(body->shader_type) {
                case SHADER_MERCURY: color = Shader_Mercury(&input); break;
                case SHADER_VENUS: color = Shader_Venus(&input); break;
                case SHADER_EARTH: color = Shader_Earth(&input); break;
                case SHADER_JUPITER: color = Shader_Jupiter(&input); break;
                case SHADER_SATURN: color = Shader_Saturn(&input); break;
                case SHADER_NEPTUNE: color = Shader_Neptune(&input); break;
                default: color = body->color; break;
            }

            int16_t px = body->screen_x + dx;
            int16_t py = body->screen_y + dy;

            if (px >= 0 && px < LCD_WIDTH && py >= 0 && py < LCD_HEIGHT) {
                LCD_DrawPixel(px, py, color);
            }
        }
    }
}

void CelestialBody_Render(CelestialBody* body)
{
    if (!body->is_visible) return;

    if (body->type == BODY_TYPE_SUN) {
        return;
    }

    if (body->type != BODY_TYPE_MOON &&
        body->prev_screen_x >= 0 && body->prev_screen_x < LCD_WIDTH &&
        body->prev_screen_y >= 0 && body->prev_screen_y < LCD_HEIGHT) {

        int16_t clear_radius = body->prev_screen_radius + 2;
        LCD_FillCircle(body->prev_screen_x, body->prev_screen_y,
                      clear_radius, COLOR_SPACE);
    }

    body->prev_screen_x = body->screen_x;
    body->prev_screen_y = body->screen_y;
    body->prev_screen_radius = body->screen_radius;
}
