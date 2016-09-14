#ifndef _SNAKE_H_
#define _SNAKE_H_
#include "hal_lcd.h"
#include <ioCC2530.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#define LED1 P1_0       // P1.0口控制LED1
#define MAX_LENGTH  200

unsigned int LCD_Display_char[8][128]={0};
void Write_Dot(int x,int y,int dot,unsigned int a[][128]);//将一个点写入数组 x，y为坐标，dot为点，a为要写入的数组
void Draw_frame();//外面的框架
void Snake_Init();//初始化一些参数
void Snake_Display();//显示蛇的函数
void Snake_Move(int dir);//蛇移动函数
int Check_Dot(int x,int y,unsigned int a[][128]);//判断一个坐标是否已经写入点了
void InitT3();//定时器3
void Food_Create(void);//创造食物函数
int Snake_Death(void);//判断蛇是否死亡，1死了 0活着
int Snake_EatedFood(void);//1蛇吃到了食物，0没吃到
void Init_Key(void);//按钮初始化
int Snake_Key(void);//按钮
void Snake_StartPage(void);//开始界面
void Snake_EndPage(void);//结束界面

int count=0;
int Snake_Direction=0;
struct Point{  //蛇点的坐标
  int x;
  int y;
};
struct Snake{
  struct Point nodes[MAX_LENGTH]; //长度
  int length; //当前长度
  int dir;  //方向
  int live;//生存状况
}snake;
struct Food{
  struct Point position;//位置
  int exist;//存在与否
}food;
#endif
