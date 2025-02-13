/**
 * @file bsp_foc.cpp
 * @brief foc板级支持包
 * @author Tony_Wang
 * @version 2.0 Beta
 * @date 2023-9-18
 * @copyright
 * @par 日志:
 *   V1.0 		基本构建，完成： 1.开环速度控制 2.基于编码器的位置闭环控制
 * 	 V1.1 		重写运行逻辑，改为模式运行
 *	 V2.0 Beta  新增电流采样和电流闭环，但是没有调通
 */

#ifndef __BSP_FOC_HPP
#define __BSP_FOC_HPP
#include "main.h"
#include "PID_cpp/bsp_pid.hpp"
#include "app_encoder/app_encoder.hpp"
#include "dep.hpp"
#include "filter/bsp_filter.hpp"
#include "adc_cpp/bsp_adc.hpp"

/* pwm 勾选 fastmode */
/* gpio 速度设置为最大 */

/* pwm 使能的枚举类型 */
typedef enum
{
	PWM_DISABLE = 0,
	PWM_ENABLE = 1
} PWM_STATE;

/* pwmio 类 定义 */
class pwmio
{
public:
	TIM_HandleTypeDef *htim; // 使用的时钟
	uint32_t TIM_CHANNEL;	 // 使用的时钟通道计数值

	PWM_STATE __TIM_EX_FLAG = PWM_DISABLE; // 互补输出使能，默认为关闭

	// 成员函数
	pwmio(void){};
	pwmio(TIM_HandleTypeDef *htim, uint32_t TIM_CHANNEL, PWM_STATE __TIM_EX_FLAG = PWM_DISABLE);
	void set_ccr(uint16_t ccr);		 // 设置 pwmio 的 比较值
	void set_state(PWM_STATE state); // pwm 输出控制函数

protected:
	uint16_t ccr;	 // 通道的比较值
	PWM_STATE state; // pwm输出状态
};

/* 用来设定 foc 正转的旋转方向 */
typedef enum
{
	FORWARD = 1,
	REVERSE = -1
} DIR_STATE;

/* ****************** 电流采样传感器构造函数 ************************** */
class current_sensor
{
public:
	/* 相电流结构体声明 */
	struct phase
	{
		float data;		// 节点上存储的元素
		int8_t channel; // 链接 bsp_ADC_DMA 的数据序号
		float offset;	// 补偿值
		// float ratio = 1; // 电流转换系数
		phase(void){};
		phase(float x) : data(x) {} // 节点的构造函数
		phase(float x, int8_t channel) : data(x), channel(channel) {}
	};

	bsp_ADC_DMA *adc_dma; // 连接的adc采样类

	phase phase_u, phase_v, phase_w; // 三个相


	// 成员函数
	current_sensor(void){};
	current_sensor(bsp_ADC_DMA *adc_dma);
	void set_phase(phase *_phase, int8_t channel);
	void calibration(uint16_t times);								 // 偏差校准函数
	void init(int8_t channel_u, int8_t channel_v, int8_t channel_w); // 初始化函数
	// ~current();
	void update(void); // 数据更新

private:
};

typedef enum
{
	openloop = 0, // 无编码器开环运行
	speedMode,	  // 速度运行
	angleMode,	  // 角度运行
	currentMode,  // 电流(力矩)运行
	calMode,	  // 校准模式，闭环不允许运行
} foc_run_mode;

/**************************			foc 构造函数				*************************/

/* foc 使能的枚举类型 */
typedef enum
{
	FOC_DISABLE = 0,
	FOC_ENABLE = 1
} FOC_STATE;

class foc
{
public:
	/* Svpwm 功能结构体 */
	struct Svpwm_Struct
	{
		float uAlpha;	// 阿尔法轴目标电压 	(最大值为母线电压 * sqrt(3) / 3)
		float uBeta;	// 贝塔轴目标电压   	(最大值为母线电压 * sqrt(3) / 3)
		float u1;		// 用于扇区判断
		float u2;		// 用于扇区判断
		float u3;		// 用于扇区判断
		float t0;		// 0矢量作用时长
		float t1;		// 1矢量作用时长
		float t2;		// 2矢量作用时长
		float t3;		// 3矢量作用时长
		float t4;		// 4矢量作用时长
		float t5;		// 5矢量作用时长
		float t6;		// 6矢量作用时长
		float t7;		// 7矢量作用时长
		float ta;		// a相作用时长
		float tb;		// b相作用时长
		float tc;		// c相作用时长
		float ts;		// SVPWM周期
		float K;		// SVPWM 系数K =  sqrt(3) * Ts / Udc
		float udc;		// 母线电压
		uint8_t sector; // 扇区索引
	};
	Svpwm_Struct Svpwm_Mod; // 调用的Svpwm结构体

	pwmio *pwm_u, *pwm_v, *pwm_w; // 三个pwm控制电机三相

	/* EN 使能IO */
	GPIO_TypeDef *EN_GPIO_Port; // 电机使能端口
	uint16_t EN_GPIO_Pin;		// 电机使能引脚

	/* 电机硬相关配置参数 */
	DIR_STATE dir;						 // 正转的旋转方向
	uint8_t pole_pairs;					 // 极对数
	float shaft_angle;					 // 机械角度，单位 rad
	float degree;						 // 机械角度，单位 度,仅方便调试
	float electrical_angle;				 // 电角度
	FOC_STATE motor_state = FOC_DISABLE; // 电机使能情况

	/* 软配置参数 */
	/* 原始电角度偏差值 */
	float zero_electrical_angle = 0.0f; // 原始电角度偏差值
	/* 电压限制 */
	float voltage_limit;		// 输出限制电压
	float voltage_power_supply; // 电源电压
	/* PID */
	pid *_PID_OUT; // 外环位置环
	pid *_PID_IN;  // 内环速度环
	pid *_PID_IQ;  // 电流q环
	pid *_PID_ID;  // 电流d环
	/* 编码器 */
	encoder *_encoder = nullptr;	 // 使用的编码器
	DIR_STATE encoder_dir = FORWARD; // 霍尔传感器方向
	/* 电流软传感器 */
	current_sensor *_current_sensor = nullptr; // 使用的电流采样
	/* 滤波器配置 */
	LowPassFilter *_SPEED_LowPassFilter = new LowPassFilter(0.01f); // 速度低通滤波，默认配置一个低通滤波器，时间常数为 10ms
	LowPassFilter *_Iq_LowPassFilter = new LowPassFilter(0.001f);	// q电流低通滤波，默认配置一个低通滤波器，时间常数为 10ms
	LowPassFilter *_Id_LowPassFilter = new LowPassFilter(0.001f);	// d电流低通滤波，默认配置一个低通滤波器，时间常数为 10ms

	/* 运行中间参数 */
	/* 目标运行值 */
	float target_speed;	  // 目标速度
	float target_degree;  // 目标角度
	float target_current; // 目标电流
	/* 运行状态 */
	foc_run_mode run_mode = speedMode; // 当前运行状态
	/* 运行参数 */
	float speed; // 当前旋转速度,单位 rpm

	/* 以下变量是控制参数，区分 I_ */
	/* 输入控制参数 */
	float U_q, U_d;
	/* 帕克逆变换后的中间量 */
	float U_alpha;
	float U_beta;
	/* 克拉克逆变换后的中间量 */
	float Uu, Uv, Uw;

	/* 以下变量是采样参数，区分 U_ */
	/* 采样实际参数 */
	float I_q, I_d;
	/* 帕克变换后的中间量 */
	float I_alpha;
	float I_beta;

	// 成员函数
	foc(void){};
	foc(pwmio *pwm_u, pwmio *pwm_v, pwmio *pwm_w, int pole_pairs, DIR_STATE dir = FORWARD, GPIO_TypeDef *EN_GPIO_Port = nullptr, uint16_t EN_GPIO_Pin = 0);

	/* foc 使能与失能 */
	void enable(void);
	void disable(void);

	/* 初始化功能函数 */
	float init_ZeroElectricalAngle(uint16_t delaytime); // 自动检测初始化电角度偏差值函数
	void init(void);									// foc 初始化函数

	/* 配置设置函数 */
	void set_voltage_limit(float voltage_limit, float voltage_power_supply); // 电压限制设置函数
	float set_ZeroElectricalAngle(float zero_electrical_angle);				 // 原始电角度设定
	void set_SPEED_LowPassFilter(LowPassFilter *_SPEED_LowPassFilter);		 // 配置 速度低通 滤波器
	void set_Iq_LowPassFilter(LowPassFilter *_Iq_LowPassFilter);			 // 配置 q电流低通 滤波器
	void set_Id_LowPassFilter(LowPassFilter *_Id_LowPassFilter);			 // 配置 d电流低通 滤波器

	void set_PID_OUT(pid *_PID_OUT);						  // 连接 PID 位置环
	void set_PID_IN(pid *_PID_IN);							  // 连接 PID 速度环
	void set_PID_IQ(pid *_PID_IQ);							  // 连接 PID 电流Q环
	void set_PID_ID(pid *_PID_ID);							  // 连接 PID 电D流环
	void set_encoder(encoder *_encoder);					  // 连接 编码器
	void set_current_sensor(current_sensor *_current_sensor); // 连接 电流软传感器

	/* 运行目标设置函数 */
	void set_speed(float _target_speed);					// 设置目标速度
	void set_speed(float _target_speed, foc_run_mode mode); // 带模式的目标速度
	void set_degree(float _target_speed);					// 设置目标角度
	void set_current(float _target_current);				// 设置目标电流

	/* 数据转换函数 */
	float shaftAngle_2_electricalAngle(void);					 // 电角度转换函数
	float shaftAngle_2_electricalAngle(float shaft_angle_putin); // 电角度转换函数
	float _normalizeAngle(float angle);							 // 角度标准化为[0,2PI]

	/* 运行计算函数 */
	void Clark_Park_Inverse(float I_q, float I_d, float angle_el);			// 输入I_q，I_d，和电角度，通过克拉克与帕克逆变换
	void Clark_Park(float Uu_in, float Uv_in, float Uw_in, float angle_el); // 电流计算 克拉克和帕克变换
	void SpwmCtrl(void);													// Spwm控制方式
	void SvpwmCtrl(void);													// Svpwm方式实现
	void run_UVW(void);														// 根据最后电压运行函数
	void run_UVW(float Uu_in, float Uv_in, float Uw_in);					// 根据最后电压运行函数
	void run_UVW_T(void);													// foc 時間输出函数(ccr计数值)
	void run_UVW_T(uint16_t Tu, uint16_t Tv, uint16_t Tw);					// foc 時間输出函数(ccr计数值)

	/* 采样功能函数 */
	void encoder_update(void); // 编码器采样更新函数
	/* 周期运行功能函数 */
	void run(void); // foc自动运行函数
	void current_run(void);
	void speed_run(void);
	void angle_run(void);

	// 保护成员函数
protected:
	uint16_t _tim_autoreload; // 当前时钟的重装载值
};

extern foc motor;
extern pwmio pwm_u;
extern pid motor_pid_out;
extern pid motor_pid_in;
extern pid motor_pid_IQ;
extern pid motor_pid_ID;
extern current_sensor foc_current_sensor;

#endif