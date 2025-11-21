/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Solar System Shader Showcase - STM32F446RE
  ******************************************************************************
  */
/* USER CODE END Header */

#include "main.h"

/* USER CODE BEGIN Includes */
#include "../../Drivers/LCD/lcd_driver.h"
#include "../../Utils/math3d.h"
#include "../../SolarSystem/celestial_body.h"
#include "../../SolarSystem/camera.h"
#include "../../SolarSystem/solar_system.h"
#include "../../Graphics/renderer.h"
#include <stdio.h>
#include <string.h>
/* USER CODE END Includes */

SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
Camera camera;
SolarSystem solarSystem;
uint32_t lastTick = 0;
float deltaTime = 0.0f;
uint32_t frameCount = 0;
float fps = 0.0f;
uint32_t fpsTimer = 0;

ShaderType currentShader = SHADER_MERCURY;
uint8_t needsRedraw = 1;
uint32_t lastButtonTime = 0;
uint8_t lastButtonState = 0;

const char* shaderNames[SHADER_COUNT] = {
    "MERCURY",
    "VENUS",
    "EARTH",
    "JUPITER",
    "SATURN",
    "NEPTUNE"
};
/* USER CODE END PV */

void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void Game_Init(void);
void Game_Update(void);
void Game_Render(void);
void Game_ProcessInput(void);
void DrawShaderInfo(void);
/* USER CODE END PFP */

/* USER CODE BEGIN 0 */

void Game_Init(void)
{
    LCD_Init();
    LCD_Clear(COLOR_SPACE);

    LCD_FillRect(60, 100, 200, 40, COLOR_BLUE);
    HAL_Delay(500);

    float aspect = (float)LCD_WIDTH / (float)LCD_HEIGHT;
    Camera_Init(&camera, 60.0f, aspect);

    camera.distance = 200.0f;
    camera.height = 100.0f;
    camera.angle = 0.0f;

    SolarSystem_Init(&solarSystem);
    solarSystem.time_scale = 0.5f;

    Renderer_Init();

    lastTick = HAL_GetTick();
    fpsTimer = lastTick;
    lastButtonTime = lastTick;

    LCD_Clear(COLOR_SPACE);
    Renderer_DrawStars(12345, 80);
}

void Game_Update(void)
{
    uint32_t currentTick = HAL_GetTick();
    deltaTime = (currentTick - lastTick) / 1000.0f;
    lastTick = currentTick;

    if (deltaTime > 0.1f) deltaTime = 0.1f;
    if (deltaTime < 0.001f) deltaTime = 0.001f;

    Game_ProcessInput();

    Camera_Update(&camera, deltaTime);
    SolarSystem_Update(&solarSystem, deltaTime);

    frameCount++;
    if (currentTick - fpsTimer >= 1000) {
        fps = frameCount;
        frameCount = 0;
        fpsTimer = currentTick;
    }
}

void Game_Render(void)
{
    if (needsRedraw) {
        LCD_Clear(COLOR_SPACE);
        Renderer_DrawStars(12345, 80);
        needsRedraw = 0;
    }

    SolarSystem_RenderWithShaders(&solarSystem, &camera, solarSystem.total_time);

    DrawShaderInfo();
}

void Game_ProcessInput(void)
{
    uint8_t buttonPressed = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);

    uint32_t currentTick = HAL_GetTick();

    if (buttonPressed && !lastButtonState && (currentTick - lastButtonTime > 300)) {
        LCD_FillCircle(160, 120, 30, COLOR_WHITE);
        HAL_Delay(50);

        currentShader = (currentShader + 1) % SHADER_COUNT;
        SolarSystem_SetPlanetShader(&solarSystem, currentShader);
        needsRedraw = 1;
        lastButtonTime = currentTick;
    }

    lastButtonState = buttonPressed;
}

void DrawShaderInfo(void)
{
    uint16_t shader_colors[6] = {
        0x8410,
        0xFFE0,
        0x047F,
        0xFD40,
        0xFE80,
        0x001F
    };

    LCD_FillRect(0, 0, 320, 30, 0x0000);
    LCD_FillRect(5, 5, 150, 20, shader_colors[currentShader]);

    uint8_t buttonState = (HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_13) == GPIO_PIN_RESET);
    if (buttonState) {
        LCD_FillCircle(170, 15, 10, COLOR_GREEN);
    } else {
        LCD_FillCircle(170, 15, 10, 0x2104);
    }
}

/* USER CODE END 0 */

int main(void)
{
  HAL_Init();
  SystemClock_Config();
  MX_GPIO_Init();
  MX_SPI1_Init();

  Game_Init();

  while (1)
  {
    Game_Update();
    Game_Render();
  }
}

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 16;
  RCC_OscInitStruct.PLL.PLLN = 336;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV4;
  RCC_OscInitStruct.PLL.PLLQ = 2;
  RCC_OscInitStruct.PLL.PLLR = 2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_SPI1_Init(void)
{
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_2LINES;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  HAL_GPIO_WritePin(GPIOC, LCD_RST_Pin|LCD_D1_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOA, LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin|LCD_D0_Pin|LCD_D2_Pin, GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOB, LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin|LCD_D4_Pin|SD_SS_Pin, GPIO_PIN_RESET);

  GPIO_InitStruct.Pin = LCD_RST_Pin|LCD_D1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_RD_Pin|LCD_WR_Pin|LCD_RS_Pin|LCD_D7_Pin|LCD_D0_Pin|LCD_D2_Pin;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LCD_CS_Pin|LCD_D6_Pin|LCD_D3_Pin|LCD_D5_Pin|LCD_D4_Pin|SD_SS_Pin;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = GPIO_PIN_13;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}

void Error_Handler(void)
{
  __disable_irq();
  while (1)
  {
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t *file, uint32_t line)
{
}
#endif
