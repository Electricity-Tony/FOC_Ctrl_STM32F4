/**
 * @file dep.cpp
 * @brief dep
 * @author Tony_Wang
 * @version 1.1
 * @date 2023-7-24
 * @copyright
 * @par 日志:
 *   V1.0 为保证所有库需要依赖的文件都包含，将所有依赖文件独立成dep库
 *
 */

#ifndef __TIMER_HPP__
#define __TIMER_HPP__

/* 包含头文件 ----------------------------------------------------------------*/
#include "main.h"
#include "tim.h"

/* 宏定义 --------------------------------------------------------------------*/

/* 函数声明 -------------------------------------------------------------------*/

void timer_init(void);

/* 耦合功能函数 */
// void task_timer_led_change(void);

#endif