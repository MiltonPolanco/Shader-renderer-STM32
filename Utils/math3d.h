#ifndef MATH3D_H
#define MATH3D_H

#include <math.h>
#include <stdint.h>

#define PI 3.14159265359f
#define TWO_PI (2.0f * PI)
#define HALF_PI (PI / 2.0f)
#define DEG_TO_RAD(x) ((x) * PI / 180.0f)

typedef struct {
    float x, y, z;
} Vector3;

typedef struct {
    float x, y;
} Vector2;

typedef struct {
    float m[4][4];
} Matrix4x4;

// Funciones matemáticas
float fast_sqrt(float x);
float fast_sin(float x);
float fast_cos(float x);
float clamp(float value, float min, float max);
float lerp(float a, float b, float t);

// Vector3
Vector3 vec3_create(float x, float y, float z);
Vector3 vec3_add(Vector3 a, Vector3 b);
Vector3 vec3_sub(Vector3 a, Vector3 b);
Vector3 vec3_scale(Vector3 v, float s);
float vec3_dot(Vector3 a, Vector3 b);
Vector3 vec3_cross(Vector3 a, Vector3 b);
float vec3_length(Vector3 v);
Vector3 vec3_normalize(Vector3 v);
float vec3_distance(Vector3 a, Vector3 b);

// Vector2
Vector2 vec2_create(float x, float y);

// Matrix4x4
Matrix4x4 mat4_identity(void);
Matrix4x4 mat4_multiply(Matrix4x4 a, Matrix4x4 b);
Vector3 mat4_multiply_vector(Matrix4x4 m, Vector3 v);
Matrix4x4 mat4_look_at(Vector3 eye, Vector3 target, Vector3 up);
Matrix4x4 mat4_perspective(float fov, float aspect, float near, float far);

#endif
