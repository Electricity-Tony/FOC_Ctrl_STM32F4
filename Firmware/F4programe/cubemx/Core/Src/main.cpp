/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "can.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_otg.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "usart_cpp/bsp_usart.hpp"
#include "Beep/bsp_beep.h"
#include "timer/timer.hpp"
#include "st7735s/bsp_st7735s.h"
#include "Can/bsp_can.h"
#include "tle5012b/bsp_tle5012b.h"
#include "Key_cpp/bsp_key.hpp"
#include "foc_cpp/bsp_foc.hpp"
#include "app_encoder/app_encoder.hpp"
#include "serialdebug/bsp_serialdebug.hpp"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM1_Init();
  MX_ADC1_Init();
  MX_CAN1_Init();
  MX_I2C2_Init();
  MX_SPI1_Init();
  MX_SPI3_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_TIM5_Init();
  MX_TIM8_Init();
  MX_TIM14_Init();
  MX_USART1_UART_Init();
  MX_USB_OTG_FS_PCD_Init();
  MX_TIM4_Init();
  MX_ADC2_Init();
  /* USER CODE BEGIN 2 */

  /* 按键初始化 */
  Button_L.clean();
  Button_M.clean();
  Button_R.clean();

  /* 电机初始化 */
  motor.set_encoder(&HallEncoder);
  motor.encoder_dir = REVERSE;
  motor.set_PID_IQ(&motor_pid_IQ);
  motor.set_PID_ID(&motor_pid_ID);
  motor.set_PID_IN(&motor_pid_in);
  motor.set_PID_OUT(&motor_pid_out);
  motor.set_voltage_limit(12, 13);
  motor.set_ZeroElectricalAngle(0.874368f); // 三孔小电机
  // motor.set_ZeroElectricalAngle(2.365873f);
  motor.init();

  /* motor 电流采样初始化 */
  motor.set_current_sensor(&foc_current_sensor);
  motor._current_sensor->init(0, 1, 2);
  //  motor._current_sensor->init(3, 4, 5);
  motor._current_sensor->calibration(100);

  /* 串口初始化 */
  USART_Debug_DMA_Init();

  /* CAN 初始化 */
  bsp_can_Init();

  /* st7735s 显示屏初始化 */
  bsp_st7735s_init(R0);

  /* 播放启动音乐 */
  // bsp_beep_Music_Play(warning);

  /* 默认电机上电 */
  // motor.enable();
  motor.disable();
  // motor.set_speed(60);
  motor.set_speed(60, openloop);
  // motor.set_current(-0.5);
  // motor.set_speed(180);
  // motor.set_degree(360);

  /* vofa+ 初始化 */
  // serial_test.check_frame_length();

  /* 定时器初始化 */
  timer_init();

  // motor.init_ZeroElectricalAngle(1000);
  // motor.init_ZeroElectricalAngle(1000);
  // motor.init_ZeroElectricalAngle(1000);
  // motor.init_ZeroElectricalAngle(1000);
  // motor.init_ZeroElectricalAngle(1000);
  // motor.init_ZeroElectricalAngle(1000);
  // motor.init_ZeroElectricalAngle(1000);
  // motor.init_ZeroElectricalAngle(1000);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

    // printf("hello world");

    // bsp_beep_Music_Handle();
    // printf("tle5012b = %f \r\n", ReadAngle());
    // printf("tle5012b = %f \r\n", motor._encoder->date);
    // printf("%5f  %5f  %5f\r\n",motor.Uu, motor.Uv, motor.Uw);
    // printf("Button State: %4d %4d %4d \r\n", Button_L.OUT_State, Button_M.OUT_State, Button_R.OUT_State);
    // printf("%5f  %5f  %5f\r\n",
    //        motor._current_sensor->phase_u.data,
    //        motor._current_sensor->phase_v.data,
    //        motor._current_sensor->phase_w.data);

    serial_test.config_data(motor.target_degree);
    serial_test.config_data(motor.degree);
    serial_test.config_data(motor.target_speed);
    serial_test.config_data(motor.speed);
    // serial_test.config_data(motor.target_current);
    // serial_test.config_data(motor.I_q);
    // serial_test.config_data(motor.U_q);

    serial_test.config_data(motor._current_sensor->phase_u.data);
    serial_test.config_data(motor._current_sensor->phase_v.data);
    serial_test.config_data(motor._current_sensor->phase_w.data);

    //  serial_test.config_data(motor.Svpwm_Mod.ta);
    // serial_test.config_data(motor.Svpwm_Mod.tb);
    // serial_test.config_data(motor.Svpwm_Mod.tc);

    serial_test.send_frame();

    // printf("%5f  %5f\r\n",
    //        motor.I_q,
    //        motor.I_d);

    if (Button_M.OUT_State == Button_ON)
    {
      motor.enable();
    }
    else
    {
      motor.disable();
    }

    HAL_Delay(1);
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
   * in the RCC_OscInitTypeDef structure.
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 4;
  RCC_OscInitStruct.PLL.PLLN = 168;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
