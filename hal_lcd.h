#ifndef _HAL_LCD_H
#define _HAL_LCD_H
#include "type.h"
void HalLcd_HW_Init(void);
void HalLcd_HW_WriteLine(unsigned int line,const char *text,unsigned char point);
uint8 getStrlen(char *p);
void HalLcd_HW_Control(uint8 cmd);
void HalLcd_HW_Display(unsigned int a[][128]);//显示全屏数组
void set_ddram_line_col(uint8 line,uint8 col);//设置行列
void HalLcd_HW_Write(uint8 data);//写数据
#endif
