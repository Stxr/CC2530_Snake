#include "snake.h"
void Write_Dot(int x,int y,int dot,unsigned int a[][128]) { //将一个点写入数组 x，y为坐标，dot为点，a为要写入的数组
    if(x<0||x>64||y<0||y>32) return;
    for(int k=x*2; k<=x*2+1; k++) {
        if(dot) {
            switch (y%4) {      //四个为一点
            case 0:
                a[y/4][k]|=0x03;
                break;
            case 1:
                a[y/4][k]|=0x0c;
                break;
            case 2:
                a[y/4][k]|=0x30;
                break;
            case 3:
                a[y/4][k]|=0xc0;
                break;
            }
        } else {
            switch (y%4) {      //四个为一点
            case 0:
                a[y/4][k]&=~0x03;
                break;
            case 1:
                a[y/4][k]&=~0x0c;
                break;
            case 2:
                a[y/4][k]&=~0x30;
                break;
            case 3:
                a[y/4][k]&=~0xc0;
                break;
            }
        }
    }
}
int Check_Dot(int x,int y,unsigned int a[][128]) { //判断一个坐标是否已经写入点了
    if(x<0||x>64||y<0||y>32) return 0;
    return !(a[y/8][2*x]&0);
}
void Draw_frame() {       //外面的框架
    for(int x=0; x<64; x++) {
        Write_Dot(x,0,1,LCD_Display_char);
        Write_Dot(x,31,1,LCD_Display_char);
    }
    for(int y=0; y<32; y++) {
        Write_Dot(0,y,1,LCD_Display_char);
        Write_Dot(63,y,1,LCD_Display_char);
    }
}
void Snake_Init() { //蛇的初始化
    HalLcd_HW_Init(); //初始化LCD
    memset(&snake, 0, sizeof(snake));//结构体置零
    memset(&food, 0,  sizeof(food));
    Draw_frame(); //四周画框
    Init_Key();
    // snake={{30,16},1,3,1};
    snake.nodes[0].x=30;
    snake.nodes[0].y=16;
    snake.nodes[1].x=31;
    snake.nodes[1].y=16;
    snake.nodes[2].x=32;
    snake.nodes[2].y=16;
    snake.length=3;
    snake.live=1;
    Snake_Display();//显示蛇的开始位置
    InitT3();
}
void Snake_Display() {
    if(snake.live) {
        for(int i=0; i<snake.length; i++) {
            if(i==snake.length-1) { //如果吃到了食物就不消去最后一个了
                Write_Dot(snake.nodes[i].x,snake.nodes[i].y,0,LCD_Display_char);
            } else {
                Write_Dot(snake.nodes[i].x,snake.nodes[i].y,1,LCD_Display_char);
            }
        }
    }
}
void Snake_Move(int dir) { //0:up 1:down 2:left 3:right
    for(int i=snake.length-1; i>0; i--) {
        snake.nodes[i]=snake.nodes[i-1];//结构体赋值,蛇身体的信号传递
    }
    switch (dir) {//蛇头移动一格
    case 0:
        snake.nodes[0].y--;
        break; //up
    case 1:
        snake.nodes[0].y++;
        break; //down
    case 2:
        snake.nodes[0].x--;
        break; //left
    case 3:
        snake.nodes[0].x++;
        break; //right
    }
    Snake_Display();//显示蛇的函数
    // snake.nodes[snake.length-1]
}
void InitT3() {
    T3CTL |= 0x08 ;          //开溢出中断
    T3IE  = 1;               //开总中断和T3中断
    T3CTL |= 0xE0;           //128分频,128/16000000*N=0.5S,N=62500
    T3CTL &= ~0x03;          //自动重装 00－>0xff  62500/255=245(次)
    T3CTL |= 0x10;           //启动
    EA = 1;                  //开总中断
}
void Food_Create(void) { //创造食物的函数
 if(!food.exist){
    food.exist=1;
    srand(snake.nodes[2].x+count);//随机种子
    food.position.x=rand()%61+1;//随机数
    food.position.y=rand()%29+1;//随机数
 }
  Write_Dot(food.position.x,food.position.y,1,LCD_Display_char);
}
int Snake_Death(void){
  for(int i=3;i<MAX_LENGTH;i++){
    if((snake.nodes[0].x==snake.nodes[i].x&&snake.nodes[0].y==snake.nodes[i].y)||\
       snake.nodes[0].x==0  ||\
       snake.nodes[0].x==63 ||\
       snake.nodes[0].y==0  ||\
       snake.nodes[0].y==31) {//如果蛇和身体有接触或者撞墙
      snake.live=0;//死了
      return 1;
    }
  }
  return 0;
}
int Snake_EatedFood(void){ //如果吃到了食物
  if(snake.nodes[0].x==food.position.x&&snake.nodes[0].y==food.position.y){
    snake.length++;//长度加一
    food.exist=0;//食物不存在了，要添加新的食物
    return 1;
  }
  return 0;
}
int Snake_Key(void)
{
   int  value;

  // APCFG |= 1 << 6 ; //注意这里是设置ADC输入通道！！
   ADCCON3  = (0x36);            //选择1.25V为参考电压；14位分辨率；对片内温度传感器采样
   ADCCON3 |= 0X80;
   ADCCON1 |= 0x30;              //选择ADC的启动模式为手动
   ADCCON1 |= 0x40;              //启动AD转化
   while(!(ADCCON1 & 0x80));     //等待 AD 转换完成
   value = (int) ADCL ;           //ADCL 寄存器低 2 位无效
   value |= ((int)(ADCH) << 8);

   return (int)sqrt(value);
}
void Init_Key(void)
{
//   DISABLE_ALL_INTERRUPTS();     //关闭所有中断
  //  InitClock();                  //设置系统主时钟为 32M
   P2SEL &=~0x01;
   P2DIR &=~0x01;
   APCFG |=0x40;
   P0DIR &= ~0x40;     //按键接在P0.1口上，设P0.1为输入模式
   //P0INP &= ~0x40;     //打开P0.1上拉电阻
}
void Snake_StartPage(void){
  for(int i=0;i<3;i++){ //第四组
    HalLcd_HW_DisplayChinese(i,0,35);
  }
  for(int i=3;i<6;i++){ //贪吃蛇
    HalLcd_HW_DisplayChinese(i,3,-3*16+35);
  }
  for(int i=6;i<6+6;i++){//按任意键开始
    HalLcd_HW_DisplayChinese(i,6,-6*16+10);
  }
}
void Snake_EndPage(void){
  for(int i=12;i<12+4;i++){//游戏结束
    HalLcd_HW_DisplayChinese(i,3,-12*16+30);
  }
}
#pragma vector = T3_VECTOR //定时器
__interrupt void T3_ISR(void) {
    T3IE = 0;
    IRCON = 0x00;            //清中断标志, 也可由硬件自动完成 ]
    count++;
    if(count==100) {      //245次中断后LED取反，闪烁一轮（约为0.5 秒时间）
        //经过示波器测量确保精确
        count = 0;           //计数清零
        //  LED1 = ~LED1;        //改变LED1的状态
        Snake_Move(Snake_Direction);
        Snake_EatedFood();
    }
    T3IE = 1;
}
