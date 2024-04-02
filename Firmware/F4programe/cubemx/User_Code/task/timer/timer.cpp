/**
 * @file timer.cpp
 * @brief timer
 * @author Tony_Wang
 * @version 1.0
 * @date 2023-8-28
 * @copyright
 * @par 日志:
 *   V1.0 建立定时器调用库，避免每次都要翻阅资料
 *
 */

#include "timer.hpp"

/* 添加的功能函数头文件 */
#include "key_cpp/bsp_key.hpp"
#include "usart_cpp/bsp_usart.hpp"
#include "st7735s/bsp_st7735s.h"
#include "foc_cpp/bsp_foc.hpp"
void timer_init(void)
{
    /* 这里放需要启动定时器中断的定时器 */
    HAL_TIM_Base_Start_IT(&htim1);
    HAL_TIM_Base_Start_IT(&htim4);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim == &htim1)
    {
        static uint8_t Tick1 = 0;
        static uint8_t Tick2 = 0;
        static uint8_t Tick3 = 0;
        if (__HAL_TIM_IS_TIM_COUNTING_DOWN(&htim1)) // 50us 20k Hz
        {
            if (Tick1 < 1) // 1: 100us
            {
                Tick1++;
                motor.current_run(); // 电流闭环一次
            }
            else
            {
                Tick1 = 0;
                motor._current_sensor->adc_dma->getonce(); // 采样一次
            }

            if (Tick2 < 9) // 9:500us
            {
                Tick2++;
            }
            else
            {
                Tick2 = 0;
                // motor._encoder->get_date(); // 编码器采样
                motor.encoder_update();
            }

            if (Tick3 < 19) // 19:1000us
            {
                Tick3++;
            }
            else
            {
                Tick3 = 0;
                // motor.run();
                motor.angle_run();
                motor.speed_run();
            }
        }
    }

    /* 依次放进入中断的定时器 */
    if (htim == (&htim4)) // 100US
    {
        /* general function start */
        /* 依次放该定时器下不同功能的触发间隔变量 */
        /* 一般类型任务 */
        static uint16_t general_flag = 0;
        /* 以上每个触发间隔变量 ++  */
        general_flag += 1;
        /* 编写触发功能函数 */
        if (general_flag == 10) // 10:1ms
        {
            Button_L.update();
            Button_M.update();
            Button_R.update();
            /* 一定要添加间隔变量复位！！ */
            general_flag = 0;
        }
        /* general function end */

        /* lcd_breath function start */
        /* 依次放该定时器下不同功能的触发间隔变量 */
        /* lcd 呼吸显示任务 */
        static uint16_t lcd_breath_flag = 0;
        /* 以上每个触发间隔变量 ++  */
        lcd_breath_flag += 1;
        /* 编写触发功能函数 */
        if (lcd_breath_flag == 500) // 500:50ms
        {
            bsp_st7735s_runtimet(500);
            /* 一定要添加间隔变量复位！！ */
            lcd_breath_flag = 0;
        }
        /* lcd_breath function end */

        /* foc run function start */
        /* 依次放该定时器下不同功能的触发间隔变量 */
        // static uint16_t foc_run_flag = 0;
        // /* 以上每个触发间隔变量 ++  */
        // foc_run_flag += 1;
        // /* 编写触发功能函数 */
        // if (foc_run_flag == 10)
        // {
        //     app_motor_can_Basic_postback(&motor_1612);

        //     /* 一定要添加间隔变量复位！！ */
        //     foc_run_flag = 0;
        // }

        /* foc run function end */

        /* ***************************************************************************** */

        /* led update function start */
        /* 依次放该定时器下不同功能的触发间隔变量 */
        // static uint16_t led_update_flag = 0;
        // /* 以上每个触发间隔变量 ++  */
        // led_update_flag += 1;
        // /* 编写触发功能函数 */
        // if (led_update_flag == 100)
        // {
        //     BORAD_LED.twinkle_update();
        //     /* 一定要添加间隔变量复位！！ */
        //     led_update_flag = 0;
        // }
        /* led update function end */

        /* ***************************************************************************** */

        /* simpl_usart function start */
        /* 串口周期发送 */
        static uint16_t simpl_usart_flag = 0;
        /* 以上每个触发间隔变量 ++  */
        simpl_usart_flag += 1;
        /* 编写触发功能函数 */
        if (simpl_usart_flag == 5000) // 50000:5s
        {
            // printf("Button State: %4d %4d %4d \r\n", Button_L.OUT_State, Button_M.OUT_State, Button_R.OUT_State);
            /* 一定要添加间隔变量复位！！ */
            simpl_usart_flag = 0;
        }
        /* simpl_usart function end */

        /* ***************************************************************************** */
    }
}
