#ifndef CELESTIAL_BODY_H
#define CELESTIAL_BODY_H

#include "math3d.h"
#include <stdint.h>

typedef enum {
    BODY_TYPE_SUN,
    BODY_TYPE_PLANET,
    BODY_TYPE_MOON
} BodyType;

typedef struct CelestialBody {
    char name[20];
    BodyType type;
    float radius;
    uint16_t color;
    Vector3 position;
    float rotation;
    float rotation_speed;
    float orbit_radius;
    float orbit_angle;
    float orbit_speed;
    float orbit_tilt;
    struct CelestialBody* parent;
    int16_t screen_x;
    int16_t screen_y;
    int16_t screen_radius;
    float distance_to_camera;
    uint8_t is_visible;
} CelestialBody;

void CelestialBody_Init(CelestialBody* body, const char* name, BodyType type);
void CelestialBody_SetOrbitalParams(CelestialBody* body, float radius, float speed, float tilt);
void CelestialBody_SetRotation(CelestialBody* body, float speed);
void CelestialBody_SetVisuals(CelestialBody* body, float radius, uint16_t color);
void CelestialBody_SetParent(CelestialBody* body, CelestialBody* parent);
void CelestialBody_Update(CelestialBody* body, float deltaTime);
void CelestialBody_UpdatePosition(CelestialBody* body);

#endif
