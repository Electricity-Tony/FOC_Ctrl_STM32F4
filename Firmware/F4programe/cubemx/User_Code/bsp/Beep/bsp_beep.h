#ifndef __BSP_Beep_H
#define __BSP_Beep_H

#include "stm32f4xx.h"

#ifdef __cplusplus
extern "C"
{
#endif // __cplusplus


//硬件相关宏定义，根据Cube使用的硬件外设在这里更改即可
#define BSP_BEEP_TIM							htim5
#define BSP_BEEP_CHANNEL					TIM_CHANNEL_1
typedef struct  song
{
	unsigned short int a; //代表音符e
	unsigned char p;      //代表音符节拍
}bsp_beep_Song;
//歌曲结构体 记得加结束符 格式为{对应音，该音是什么音符}

void bsp_beep_Music_Handle(void);//将这个函数放在定时器更新中断里面 建议0.5s为一次周期 可根据歌曲特性改变周期
void bsp_beep_Music_Play(bsp_beep_Song* head);
void bsp_beep_Music_Init(void);
void bsp_beep_Music_Stop(void); //音乐停止

extern bsp_beep_Song warning[];
extern bsp_beep_Song warning1_L[];
extern bsp_beep_Song warning2_L[];
extern bsp_beep_Song warning3_L[];
extern bsp_beep_Song warning4_L[];
extern bsp_beep_Song warning5_L[];
extern bsp_beep_Song warning6_L[];
extern bsp_beep_Song warning7_L[];
extern bsp_beep_Song warning8_L[];
extern bsp_beep_Song warning1_M[];
extern bsp_beep_Song warning2_M[];
extern bsp_beep_Song warning3_M[];
extern bsp_beep_Song warning4_M[];
extern bsp_beep_Song warning5_M[];
extern bsp_beep_Song warning6_M[];
extern bsp_beep_Song warning7_M[];
extern bsp_beep_Song warning8_M[];
extern bsp_beep_Song warning1_H[];
extern bsp_beep_Song warning2_H[];
extern bsp_beep_Song warning3_H[];
extern bsp_beep_Song warning4_H[];
extern bsp_beep_Song warning5_H[];
extern bsp_beep_Song warning6_H[];
extern bsp_beep_Song warning7_H[];
extern bsp_beep_Song warning8_H[];
extern bsp_beep_Song warningx[];
extern bsp_beep_Song jile[];
extern bsp_beep_Song Castle_in_the_Sky[];
extern bsp_beep_Song ItsOK[];
extern bsp_beep_Song Little_star[];
extern bsp_beep_Song INTEL[];
extern bsp_beep_Song Song_of_Joy[];
extern bsp_beep_Song Kontora[];
extern bsp_beep_Song News[];


#ifdef __cplusplus
}
#endif // __cplusplus


#endif

