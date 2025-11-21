#ifndef CAMERA_H
#define CAMERA_H

#include "math3d.h"
#include "celestial_body.h"

typedef struct {
    Vector3 position;
    Vector3 target;
    Vector3 up;

    float fov;
    float aspect;
    float near_plane;
    float far_plane;


    float angle;
    float distance;
    float height;

    // Controles
    uint8_t mode_3d;

    // Sistema de warp
    uint8_t is_warping;
    Vector3 warp_start_pos;
    Vector3 warp_target_pos;
    float warp_progress;
    float warp_duration;

    // Matrices
    Matrix4x4 view_matrix;
    Matrix4x4 projection_matrix;
    Matrix4x4 view_projection;

} Camera;

// Funciones de inicialización
void Camera_Init(Camera* cam, float fov, float aspect);

// Funciones de actualización
void Camera_Update(Camera* cam, float deltaTime);
void Camera_UpdateMatrices(Camera* cam);


void Camera_Rotate(Camera* cam, float delta_angle);
void Camera_Zoom(Camera* cam, float delta_distance);
void Camera_MoveVertical(Camera* cam, float delta);
void Camera_SetMode3D(Camera* cam, uint8_t enable);

// Sistema de warp
void Camera_WarpTo(Camera* cam, Vector3 target, float duration);
void Camera_WarpToBody(Camera* cam, CelestialBody* body, float duration);

// Proyección
Vector2 Camera_WorldToScreen(Camera* cam, Vector3 world_pos, int screen_width, int screen_height);
uint8_t Camera_IsPointVisible(Camera* cam, Vector3 world_pos);

#endif // CAMERA_H
