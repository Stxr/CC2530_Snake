#include <ioCC2530.h>
#include "type.h"
#include "hal_lcd.h"
extern unsigned int LCD_Display_char[8][128];
void main(void)
{
  Snake_Init();
  while (1) {
    HalLcd_HW_Display(LCD_Display_char);
  }
}
