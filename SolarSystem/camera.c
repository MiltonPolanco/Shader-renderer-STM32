#include "camera.h"
#include "../Drivers/LCD/lcd_driver.h"

void Camera_Init(Camera* cam, float fov, float aspect)
{
    cam->position = vec3_create(0, 50, 100);
    cam->target = vec3_create(0, 0, 0);
    cam->up = vec3_create(0, 1, 0);

    cam->fov = DEG_TO_RAD(fov);
    cam->aspect = aspect;
    cam->near_plane = 0.1f;
    cam->far_plane = 1000.0f;

    cam->angle = 0.0f;
    cam->distance = 150.0f;
    cam->height = 50.0f;

    cam->mode_3d = 0;

    cam->is_warping = 0;
    cam->warp_progress = 0.0f;
    cam->warp_duration = 1.0f;

    Camera_UpdateMatrices(cam);
}

void Camera_Update(Camera* cam, float deltaTime)
{
    // Procesar warp
    if (cam->is_warping) {
        cam->warp_progress += deltaTime / cam->warp_duration;

        if (cam->warp_progress >= 1.0f) {
            cam->warp_progress = 1.0f;
            cam->is_warping = 0;
            cam->position = cam->warp_target_pos;
        } else {
            // Interpolación suave
            float t = cam->warp_progress;
            t = t * t * (3.0f - 2.0f * t);

            cam->position.x = lerp(cam->warp_start_pos.x, cam->warp_target_pos.x, t);
            cam->position.y = lerp(cam->warp_start_pos.y, cam->warp_target_pos.y, t);
            cam->position.z = lerp(cam->warp_start_pos.z, cam->warp_target_pos.z, t);
        }
    } else {
        // Actualizar posición basada en ángulo y distancia
        cam->position.x = cam->distance * fast_cos(cam->angle);
        cam->position.z = cam->distance * fast_sin(cam->angle);
        cam->position.y = cam->height;
    }

    // Actualizar matrices
    Camera_UpdateMatrices(cam);
}

void Camera_UpdateMatrices(Camera* cam)
{
    // Matriz de vista
    cam->view_matrix = mat4_look_at(cam->position, cam->target, cam->up);

    // Matriz de proyección
    cam->projection_matrix = mat4_perspective(cam->fov, cam->aspect,
                                              cam->near_plane, cam->far_plane);

    // Combinar ambas matrices
    cam->view_projection = mat4_multiply(cam->projection_matrix, cam->view_matrix);
}

void Camera_Rotate(Camera* cam, float delta_angle)
{
    if (!cam->is_warping) {
        cam->angle += delta_angle;
        if (cam->angle > TWO_PI) cam->angle -= TWO_PI;
        if (cam->angle < 0) cam->angle += TWO_PI;
    }
}

void Camera_Zoom(Camera* cam, float delta_distance)
{
    if (!cam->is_warping) {
        cam->distance += delta_distance;
        cam->distance = clamp(cam->distance, 20.0f, 500.0f);
    }
}

void Camera_MoveVertical(Camera* cam, float delta)
{
    if (!cam->is_warping && cam->mode_3d) {
        cam->height += delta;
        cam->height = clamp(cam->height, -200.0f, 200.0f);
    }
}

void Camera_SetMode3D(Camera* cam, uint8_t enable)
{
    cam->mode_3d = enable;
}

void Camera_WarpTo(Camera* cam, Vector3 target, float duration)
{
    cam->is_warping = 1;
    cam->warp_start_pos = cam->position;
    cam->warp_target_pos = target;
    cam->warp_progress = 0.0f;
    cam->warp_duration = duration;
}

void Camera_WarpToBody(Camera* cam, CelestialBody* body, float duration)
{
    // Calcular posición de observación cerca del cuerpo
    Vector3 offset = vec3_create(body->radius * 5.0f, body->radius * 3.0f, body->radius * 5.0f);
    Vector3 target_pos = vec3_add(body->position, offset);

    Camera_WarpTo(cam, target_pos, duration);
}

// Proyección ortográfica con escala pequeña para que TODO quepa
Vector2 Camera_WorldToScreen(Camera* cam, Vector3 world_pos, int screen_width, int screen_height)
{


    float scale = 0.28f;  // Escala pequeña para que TODO quepa

    // Centrar perfectamente en pantalla
    float center_x = screen_width / 2.0f;
    float center_y = screen_height / 2.0f;


    float screen_x = (world_pos.x * scale) + center_x;
    float screen_y = (world_pos.z * scale) + center_y;

    Vector2 result;
    result.x = screen_x;
    result.y = screen_y;

    return result;
}

uint8_t Camera_IsPointVisible(Camera* cam, Vector3 world_pos)
{

    return 1;
}
