#include <ioCC2530.h>
#include "type.h"
#include "hal_lcd.h"
extern unsigned int LCD_Display_char[8][128];
extern int Snake_Direction;
int main(void)
{
   HalLcd_HW_Init();
  Snake_StartPage();
  while(Snake_Key()>140);
  Snake_Init();
  while (1) {
    Food_Create();//创造食物,放在主函数里是为了产生随机种子
    if(Snake_Key()<60){
      Snake_Direction=2;//up
    }else if(Snake_Key()>60&&Snake_Key()<120){
      Snake_Direction=3;//down
    }else if(Snake_Key()>120&&Snake_Key()<135){
      Snake_Direction=1;//left
    }else if(Snake_Key()>135&&Snake_Key()<140){
      Snake_Direction=0;//right
    }
    if(Snake_Death()){
      T3IE = 0;
      HalLcd_HW_Init();
      Snake_EndPage();
      break;
    }
    HalLcd_HW_Display(LCD_Display_char);
  }
}
