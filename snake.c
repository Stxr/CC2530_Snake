#include "snake.h"
int count=0;
void Write_Dot(int x,int y,int dot,unsigned int a[][128]){ //将一个点写入数组 x，y为坐标，dot为点，a为要写入的数组
  if(x<0||x>64||y<0||y>32) return;
    for(int k=x*2;k<=x*2+1;k++){
      if(dot){
        switch (y%4) {      //四个为一点
          case 0: a[y/4][k]|=0x03;break;
          case 1: a[y/4][k]|=0x0c;break;
          case 2: a[y/4][k]|=0x30;break;
          case 3: a[y/4][k]|=0xc0;break;
        }
      }else{
        switch (y%4) {      //四个为一点
          case 0: a[y/4][k]&=~0x03;break;
          case 1: a[y/4][k]&=~0x0c;break;
          case 2: a[y/4][k]&=~0x30;break;
          case 3: a[y/4][k]&=~0xc0;break;
        }
      }
    }
}
int Check_Dot(int x,int y,unsigned int a[][128]){ //判断一个坐标是否已经写入点了
  if(x<0||x>64||y<0||y>32) return 0;
    return !(a[y/8][2*x]&0);
}
void Draw_frame(){        //外面的框架
  for(int x=0;x<64;x++){
    Write_Dot(x,0,1,LCD_Display_char);
    Write_Dot(x,31,1,LCD_Display_char);
  }
  for(int y=0;y<32;y++){
    Write_Dot(0,y,1,LCD_Display_char);
    Write_Dot(63,y,1,LCD_Display_char);
  }
}
void Snake_Init(){ //蛇的初始化
  HalLcd_HW_Init(); //初始化LCD
  memset(&snake, 0, sizeof(snake));//结构体置零
  Draw_frame(); //四周画框
  // snake={{30,16},1,3,1};
  snake.nodes[0].x=30;
  snake.nodes[0].y=16;
  snake.nodes[1].x=31;
  snake.nodes[1].y=16;
  snake.nodes[2].x=32;
  snake.nodes[2].y=16;
  snake.length=3;
  snake.live=1;

  // P1DIR |= 0x01;           //P1.0定义为输出

  Snake_Display();//显示蛇的开始位置
  InitT3();
}
void Snake_Display(){
  if(snake.live){
    for(int i=0;i<snake.length;i++){
      if(i==snake.length-1){
        Write_Dot(snake.nodes[i].x,snake.nodes[i].y,0,LCD_Display_char);
      }else{
        Write_Dot(snake.nodes[i].x,snake.nodes[i].y,1,LCD_Display_char);
      }
    }
  }
}
void Snake_Move(int dir){//0:up 1:down 2:left 3:right
  for(int i=snake.length-1;i>0;i--){
    snake.nodes[i]=snake.nodes[i-1];//结构体赋值,蛇身体的信号传递
  }
  switch (dir) {//蛇头移动一格
    case 0:snake.nodes[0].y--;break; //up
    case 1:snake.nodes[0].y++;break; //down
    case 2:snake.nodes[0].x--;break; //left
    case 3:snake.nodes[0].x++;break; //right
  }
  // snake.nodes[snake.length-1]
}
void InitT3()
{
  T3CTL |= 0x08 ;          //开溢出中断
  T3IE = 1;                //开总中断和T3中断
  T3CTL |= 0xE0;           //128分频,128/16000000*N=0.5S,N=62500
  T3CTL &= ~0x03;          //自动重装 00－>0xff  62500/255=245(次)
  T3CTL |= 0x10;           //启动
  EA = 1;                  //开总中断
}
#pragma vector = T3_VECTOR
__interrupt void T3_ISR(void)
{
    T3IE = 0;
    IRCON = 0x00;            //清中断标志, 也可由硬件自动完成 ]
    count++;
    if(count==245)        //245次中断后LED取反，闪烁一轮（约为0.5 秒时间）
    {                        //经过示波器测量确保精确
       count = 0;           //计数清零
      //  LED1 = ~LED1;        //改变LED1的状态
       Snake_Move(0);
    }
    T3IE = 1;
}