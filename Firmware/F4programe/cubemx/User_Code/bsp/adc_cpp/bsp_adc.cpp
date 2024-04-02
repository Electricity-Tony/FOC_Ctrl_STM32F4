/**
 * @file bsp_adc.cpp
 * @brief adc支持包
 * @author Tony_Wang
 * @version 2.0
 * @date 2022-8-31
 * @copyright
 * @par 日志:
 *   V1.0 基本Cube配置DMA配置
 *	 V1.1 增加测不准修改的配置文件
 *
 *	 V2.0 修改为cpp文件，区分普通adc与dmaadc
 */

#include "bsp_adc.hpp"
#include "adc.h"

uint16_t bsp_adc_dma1_buffer[7];
bsp_ADC_DMA bsp_adc_dma1 = bsp_ADC_DMA(&hadc1, 7, bsp_adc_dma1_buffer);

/* *************************************************************** */

////******************************************* bsp_ADC 普通adc采样*************************************************************************////
/**
 * @brief  bsp_ADC ADC声明
 * @param [in]   *hadc	adc编号
 * @param [in]	NUMs_channel	一共使用的adc通道
 *
 */

WEAK bsp_ADC::bsp_ADC(ADC_HandleTypeDef *hadc, uint8_t NUMs_channel)
{
	this->hadc = hadc;
	this->NUMs_channel = NUMs_channel;
}

/**
 * @brief    init 初始化使能相关功能
 * @details  主要使能校准
 * @param
 * @return
 */
void bsp_ADC::init(void)
{
	// HAL_ADCEx_Calibration_Start(hadc); //F4不需要这个
}

/**
 * @brief    update 数据更新
 * @details  所有通道进行一次ADC采样转换
 * @param
 * @return
 */
void bsp_ADC::update(void)
{
	for (uint8_t i = 0; i < NUMs_channel; i++)
	{
		HAL_ADC_Start(hadc);
		HAL_ADC_PollForConversion(hadc, 50);
		data_list[i] = HAL_ADC_GetValue(hadc);
	}
	HAL_ADC_Stop(hadc);
}

////******************************************* bsp_ADC_DMA 使用DMA的ADC采样 *************************************************************************////
/**
 * @brief  bsp_ADC_DMA 串口绘图类声明
 * @param [in]   *hadc	adc编号
 * @param [in]	NUMs_channel	一共使用的adc通道
 * @param [in]	*data_DMA_buffer	连接的DMA暂存数组
 *
 */

WEAK bsp_ADC_DMA::bsp_ADC_DMA(ADC_HandleTypeDef *hadc, uint8_t NUMs_channel, uint16_t *data_DMA_buffer)
{
	this->hadc = hadc;
	this->NUMs_channel = NUMs_channel;
	this->data_DMA_buffer = data_DMA_buffer;
}

/**
 * @brief    init 初始化使能相关功能
 * @details  主要使能校准 开启DMA功能
 * @param
 * @return
 */
void bsp_ADC_DMA::init(void)
{
	// HAL_ADCEx_Calibration_Start(hadc);
	HAL_ADC_Start_DMA(hadc, (uint32_t *)data_DMA_buffer, NUMs_channel);
}

/**
 * @brief    软件使用 adc 获取一次数据
 * @details  主要用在单次 软件触发adc采样上
 * @param
 * @return
 */
void bsp_ADC_DMA::getonce(void)
{
	HAL_ADC_Start_DMA(hadc, (uint32_t *)data_DMA_buffer, NUMs_channel);
}

/**
 * @brief    update 数据更新
 * @details  所有通道读取一次DMA数据
 * @param
 * @return
 */
void bsp_ADC_DMA::update(void)
{
	for (uint8_t i = 0; i < NUMs_channel; i++)
	{
		data_list[i] = data_DMA_buffer[i];
	}
}
