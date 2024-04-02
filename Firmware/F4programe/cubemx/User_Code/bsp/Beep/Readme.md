# RM2020电控 Board Support Package

## 综述

> 本BSP库根据个人调试和对RM2019代码工程阅读来构建,难免有疏漏之处,目前虽无运行上的BUG,但希望在使用本BSP库过程中出现任何BUG及时提出,以便修正

* @外设相关：<font color=Red>Beep</font> 

  @版本：<font color=Red>1.0</font> 

  @维护：<font color=Red>Evan-GH</font> 

  @代码规范：<font color=Red>RM电控代码规范</font>

  | 版本                               |                             功能                             |
  | :--------------------------------- | :----------------------------------------------------------: |
  | <font color=DeepSkyBlue>1.0</font> | <font color=DeepSkyBlue>根据代码规范修改上届代码库中的蜂鸣器库，整合乐谱</font> |
  
* 对外接口函数

```C
void bsp_beep_Music_Handle(void);//将这个函数放在定时器更新里面 周期根据歌曲特性来自行改变
void bsp_beep_Music_Play(bsp_beep_Song* head);	//开始演奏某个歌曲
void bsp_beep_Music_Stop(void); //音乐停止
```

> 当你需要演奏某个歌曲的时候，调用演奏函数，需要停止的时候调用停止函数，Handle函数放在某个周期函数里面更新。

* 对外结构体和变量

```C
typedef struct  song
{
	unsigned short int a; //代表音符
	unsigned char p;      //代表音符节拍
}bsp_beep_Song;
//歌曲结构体 记得加结束符 格式为{对应音，该音是什么音符}

//对外变量为具体乐谱
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
extern bsp_beep_Song jile[];		//极乐净土
extern bsp_beep_Song Castle_in_the_Sky[]; //天空之城
extern bsp_beep_Song ItsOK[];
extern bsp_beep_Song Little_star[]; //小星星
extern bsp_beep_Song INTEL[];		//英特尔经典音效
extern bsp_beep_Song Song_of_Joy[];
extern bsp_beep_Song Kontora[];		
extern bsp_beep_Song News[];		//新闻音效
extern bsp_beep_Song VOCALOID_1[]; 	//末世歌者
```

> 对外变量主要包含的是乐谱存储数组

## 使用方法

* 1.加载本bsp库到工程中

```C
#include "bsp_beep.h"
```

* 2.使用**void bsp_beep_Music_Play(bsp_beep_Song* head)** 函数演奏你想要演奏的音乐，比如极乐净土：

```C
bsp_beep_Music_Play(jile);
```

* 3.在某个周期执行的函数中放置更新函数：

```C
 while (1)
  {
    /* USER CODE END WHILE */
		bsp_beep_Music_Handle();	//音乐更新句柄
		HAL_Delay(50);						//延时，根据音乐效果自己更改
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
```

> 对于音乐要求比较高的可以自行修改音乐持续时间，音调，音乐间隔时间来获得更好的体验，这里不再赘述。