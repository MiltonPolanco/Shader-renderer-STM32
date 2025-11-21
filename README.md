# Planetary Shader Rendering on STM32

## Demostración 

![Simulación](video-shader-renderer.gif)

## Descripción

Proyecto de renderizado procedural de planetas del sistema solar implementado en un microcontrolador STM32F446RE. El sistema renderiza 6 planetas utilizando únicamente shaders procedurales sin texturas precargadas, aprovechando técnicas de ruido fractal y múltiples capas de procesamiento.

El renderizado se ejecuta en tiempo real sobre un display LCD ILI9341, escribiendo directamente píxel por píxel sin utilizar framebuffer. Cada planeta implementa entre 4 y 5 capas de cálculo para lograr efectos visuales complejos como nubes animadas, bandas atmosféricas, cráteres y efectos de iluminación.

## Especificaciones Técnicas

### Hardware
- **MCU:** STM32F446RE (ARM Cortex-M4F, 84 MHz, 128 KB RAM)
- **RAM:** 128KB (uso aproximado: ~15KB)
- **Display:** LCD ILI9341 (320x240 píxeles, RGB565)
- **Interfaz:** Bus paralelo de 8 bits

### Rendimiento
- **FPS:** 5-8 frames por segundo
- **Resolución de renderizado:** 320x240 píxeles
- **Uso de RAM:** ~15KB (ahorro de 150KB al no usar framebuffer)

## Planetas Implementados

### 1. Mercury
Planeta rocoso con superficie craterizada de color gris. Implementa 4 capas de Fractal Brownian Motion (FBM) para generar textura de cráteres con variaciones de profundidad y rugosidad superficial.

### 2. Venus
Planeta amarillo con atmósfera densa. Utiliza 5 capas de ruido animado para simular movimiento de nubes espesas.

### 3. Earth
Planeta terrestre con océanos azules, continentes verdes y nubes blancas animadas. Implementa más de 5 capas de procesamiento con alpha blending para las nubes.

### 4. Jupiter
Gigante gaseoso con bandas horizontales naranjas y tormentas. Utiliza 5 capas de ruido con turbulencia direccional para crear las características bandas atmosféricas. 

### 5. Saturn
Planeta beige con bandas atmosféricas sutiles. Implementa 4 capas de ruido de baja intensidad para lograr la apariencia suave característica. 

### 6. Neptune
Planeta azul con energía dinámica y efecto glow pulsante.

## Técnicas de Shader Implementadas

### Funciones Base de Ruido

#### `Noise(x, y)`
Generador de ruido pseudoaleatorio basado en función hash. Toma coordenadas enteras y produce valores entre 0 y 1 de manera determinística.

```c
float hash = sin(x * 12.9898f + y * 78.233f) * 43758.5453f;
return hash - floor(hash);
```

#### `SmoothNoise(x, y)`
Suaviza el ruido base promediando el valor del píxel con sus 4 vecinos inmediatos. Reduce artefactos de alta frecuencia.

```c
float corners = (Noise(x-1,y-1) + Noise(x+1,y-1) + 
                 Noise(x-1,y+1) + Noise(x+1,y+1)) / 16.0f;
float sides = (Noise(x-1,y) + Noise(x+1,y) + 
               Noise(x,y-1) + Noise(x,y+1)) / 8.0f;
float center = Noise(x,y) / 4.0f;
return corners + sides + center;
```

#### `InterpolatedNoise(x, y)`
Implementa interpolación bilinear entre valores de ruido para eliminar discontinuidades. Calcula valores fraccionarios entre píxeles.

```c
float fractional_x = x - integer_x;
float fractional_y = y - integer_y;
float v1 = SmoothNoise(integer_x, integer_y);
float v2 = SmoothNoise(integer_x + 1, integer_y);
float v3 = SmoothNoise(integer_x, integer_y + 1);
float v4 = SmoothNoise(integer_x + 1, integer_y + 1);
float i1 = lerp(v1, v2, fractional_x);
float i2 = lerp(v3, v4, fractional_x);
return lerp(i1, i2, fractional_y);
```

#### `FBM(x, y)` - Fractal Brownian Motion
Suma múltiples octavas de ruido con frecuencias y amplitudes progresivas. Base fundamental para generar patrones naturales complejos.

```c
float total = 0.0f;
float frequency = 1.0f;
float amplitude = 1.0f;
float persistence = 0.5f;

for (int i = 0; i < octaves; i++) {
    total += InterpolatedNoise(x * frequency, y * frequency) * amplitude;
    frequency *= 2.0f;
    amplitude *= persistence;
}
```

### Funciones de Procesamiento

#### `Smoothstep(edge0, edge1, x)`
Función de interpolación suave hermítica. Utilizada para transiciones graduales entre regiones (océano-tierra, atmósfera-espacio).

```c
float t = clamp((x - edge0) / (edge1 - edge0), 0.0f, 1.0f);
return t * t * (3.0f - 2.0f * t);
```

#### Iluminación Difusa
Modelo de iluminación Lambertiana usando producto punto entre normal de superficie y dirección de luz.

```c
vec3 normal = normalize(surface_normal);
vec3 light_dir = normalize(light_position - fragment_position);
float diffuse = max(dot(normal, light_dir), 0.0f);
```

#### Alpha Blending
Composición de capas semitransparentes para efectos atmosféricos (nubes).

```c
result_color = source_color * alpha + dest_color * (1.0f - alpha);
```

### Módulos Principales

#### `main.c`
- Inicialización de periféricos (GPIO, SPI, FPU)
- Loop de renderizado principal
- Detección de pulsación de botón (debouncing)
- Cambio de planeta activo
- Renderizado de UI (barra indicadora, feedback visual)

#### `planet_shader.c`
- Implementación de funciones de ruido (Noise, SmoothNoise, InterpolatedNoise)
- Función FBM con parámetros configurables
- Shaders específicos de cada planeta
- Funciones de colorización RGB565

#### `celestial_body.c`
- Estructura de datos de planetas
- Conversión de coordenadas cartesianas a esféricas
- Mapeo UV para texturas procedurales
- Cálculo de normales para iluminación
- Renderizado píxel por píxel

#### `lcd_driver.c`
- Comunicación con ILI9341 vía bus paralelo
- Comandos de inicialización del display
- Escritura directa de píxeles RGB565
- Funciones de ventana de renderizado

#### `math3d.c`
- Operaciones vectoriales (dot, cross, normalize)
- Interpolación lineal y smoothstep
- Funciones trigonométricas optimizadas
- Utilidades de mapeo de rangos

## Autor

**Milton Polanco**  
Universidad del Valle de Guatemala  
2025

## Video

https://youtu.be/Z_CTkrWBHt8

---

Desarrollado con STM32CubeIDE y bibliotecas STM32 HAL.