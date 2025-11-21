#include "math3d.h"
#include <stdlib.h>

// Aproximación rápida de sqrt usando instrucciones ARM
float fast_sqrt(float x)
{
    #ifdef __ARM_FEATURE_FPU
    float result;
    __asm volatile ("vsqrt.f32 %0, %1" : "=t"(result) : "t"(x));
    return result;
    #else
    return sqrtf(x);
    #endif
}

// Aproximación rápida de sin
float fast_sin(float x)
{
    // Normalizar a rango [-PI, PI]
    while (x > PI) x -= TWO_PI;
    while (x < -PI) x += TWO_PI;

    // Aproximación de Taylor (suficientemente precisa para gráficos)
    float x2 = x * x;
    return x * (1.0f - x2 * (0.16666667f - x2 * 0.00833333f));
}

// Aproximación rápida de cos
float fast_cos(float x)
{
    return fast_sin(x + HALF_PI);
}

float clamp(float value, float min, float max)
{
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

float lerp(float a, float b, float t)
{
    return a + (b - a) * t;
}

// ============= Vector3 =============

Vector3 vec3_create(float x, float y, float z)
{
    Vector3 v = {x, y, z};
    return v;
}

Vector3 vec3_add(Vector3 a, Vector3 b)
{
    Vector3 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    result.z = a.z + b.z;
    return result;
}

Vector3 vec3_sub(Vector3 a, Vector3 b)
{
    Vector3 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    result.z = a.z - b.z;
    return result;
}

Vector3 vec3_scale(Vector3 v, float s)
{
    Vector3 result;
    result.x = v.x * s;
    result.y = v.y * s;
    result.z = v.z * s;
    return result;
}

float vec3_dot(Vector3 a, Vector3 b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

Vector3 vec3_cross(Vector3 a, Vector3 b)
{
    Vector3 result;
    result.x = a.y * b.z - a.z * b.y;
    result.y = a.z * b.x - a.x * b.z;
    result.z = a.x * b.y - a.y * b.x;
    return result;
}

float vec3_length_squared(Vector3 v)
{
    return v.x * v.x + v.y * v.y + v.z * v.z;
}

float vec3_length(Vector3 v)
{
    return fast_sqrt(vec3_length_squared(v));
}

Vector3 vec3_normalize(Vector3 v)
{
    float len = vec3_length(v);
    if (len > 0.0001f) {
        float inv_len = 1.0f / len;
        return vec3_scale(v, inv_len);
    }
    return vec3_create(0, 0, 0);
}

float vec3_distance(Vector3 a, Vector3 b)
{
    return vec3_length(vec3_sub(b, a));
}

// ============= Vector2 =============

Vector2 vec2_create(float x, float y)
{
    Vector2 v = {x, y};
    return v;
}

Vector2 vec2_add(Vector2 a, Vector2 b)
{
    Vector2 result;
    result.x = a.x + b.x;
    result.y = a.y + b.y;
    return result;
}

Vector2 vec2_sub(Vector2 a, Vector2 b)
{
    Vector2 result;
    result.x = a.x - b.x;
    result.y = a.y - b.y;
    return result;
}

float vec2_length(Vector2 v)
{
    return fast_sqrt(v.x * v.x + v.y * v.y);
}

// ============= Matrix4x4 =============

Matrix4x4 mat4_identity(void)
{
    Matrix4x4 m = {0};
    m.m[0][0] = 1.0f;
    m.m[1][1] = 1.0f;
    m.m[2][2] = 1.0f;
    m.m[3][3] = 1.0f;
    return m;
}

Matrix4x4 mat4_multiply(Matrix4x4 a, Matrix4x4 b)
{
    Matrix4x4 result = {0};

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            result.m[i][j] =
                a.m[i][0] * b.m[0][j] +
                a.m[i][1] * b.m[1][j] +
                a.m[i][2] * b.m[2][j] +
                a.m[i][3] * b.m[3][j];
        }
    }

    return result;
}

Vector3 mat4_multiply_vector(Matrix4x4 m, Vector3 v)
{
    Vector3 result;
    float w;

    result.x = v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0] + m.m[3][0];
    result.y = v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1] + m.m[3][1];
    result.z = v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2] + m.m[3][2];
    w        = v.x * m.m[0][3] + v.y * m.m[1][3] + v.z * m.m[2][3] + m.m[3][3];

    if (w != 0.0f) {
        result.x /= w;
        result.y /= w;
        result.z /= w;
    }

    return result;
}

Matrix4x4 mat4_rotation_x(float angle)
{
    Matrix4x4 m = mat4_identity();
    float c = fast_cos(angle);
    float s = fast_sin(angle);

    m.m[1][1] = c;
    m.m[1][2] = -s;
    m.m[2][1] = s;
    m.m[2][2] = c;

    return m;
}

Matrix4x4 mat4_rotation_y(float angle)
{
    Matrix4x4 m = mat4_identity();
    float c = fast_cos(angle);
    float s = fast_sin(angle);

    m.m[0][0] = c;
    m.m[0][2] = s;
    m.m[2][0] = -s;
    m.m[2][2] = c;

    return m;
}

Matrix4x4 mat4_rotation_z(float angle)
{
    Matrix4x4 m = mat4_identity();
    float c = fast_cos(angle);
    float s = fast_sin(angle);

    m.m[0][0] = c;
    m.m[0][1] = -s;
    m.m[1][0] = s;
    m.m[1][1] = c;

    return m;
}

Matrix4x4 mat4_translation(float x, float y, float z)
{
    Matrix4x4 m = mat4_identity();
    m.m[3][0] = x;
    m.m[3][1] = y;
    m.m[3][2] = z;
    return m;
}

Matrix4x4 mat4_scale(float x, float y, float z)
{
    Matrix4x4 m = mat4_identity();
    m.m[0][0] = x;
    m.m[1][1] = y;
    m.m[2][2] = z;
    return m;
}

Matrix4x4 mat4_perspective(float fov, float aspect, float near, float far)
{
    Matrix4x4 m = {0};

    float tanHalfFov = tanf(fov / 2.0f);

    m.m[0][0] = 1.0f / (aspect * tanHalfFov);
    m.m[1][1] = 1.0f / tanHalfFov;
    m.m[2][2] = -(far + near) / (far - near);
    m.m[2][3] = -1.0f;
    m.m[3][2] = -(2.0f * far * near) / (far - near);

    return m;
}

Matrix4x4 mat4_look_at(Vector3 eye, Vector3 target, Vector3 up)
{
    Vector3 zaxis = vec3_normalize(vec3_sub(eye, target));
    Vector3 xaxis = vec3_normalize(vec3_cross(up, zaxis));
    Vector3 yaxis = vec3_cross(zaxis, xaxis);

    Matrix4x4 m = mat4_identity();

    m.m[0][0] = xaxis.x;
    m.m[1][0] = xaxis.y;
    m.m[2][0] = xaxis.z;
    m.m[3][0] = -vec3_dot(xaxis, eye);

    m.m[0][1] = yaxis.x;
    m.m[1][1] = yaxis.y;
    m.m[2][1] = yaxis.z;
    m.m[3][1] = -vec3_dot(yaxis, eye);

    m.m[0][2] = zaxis.x;
    m.m[1][2] = zaxis.y;
    m.m[2][2] = zaxis.z;
    m.m[3][2] = -vec3_dot(zaxis, eye);

    return m;
}
