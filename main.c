/////////////////By 3DSage on YouTube https://www.youtube.com/3dsage////////////

typedef unsigned short u16;
typedef unsigned long u32;
#define MODE_4 0x4
#define BG2_ENABLE       0x400
#define REG_DISPCNT      *(u32*)0x4000000
#define SetMode(mode)    REG_DISPCNT = (mode)
u16* videoBuffer;
u16* FrontBuffer       = (u16*)0x6000000;
u16* BackBuffer        = (u16*)0x600A000;
#define VideoBuffer      (u16*)0x6000000
#define BACKBUFFER             0x10
u16* paletteMem        = (u16*)0x5000000;
#define KEYA             1
#define KEYB             2
#define KEYRIGHT         16
#define KEYLEFT          32
#define KEYUP            64
#define KEYDOWN          128
#define KEYS             (*(volatile u16*)0x4000130)
#define KEY_DOWN(k)      (!((KEYS)&k))
volatile u16* ScanlineCounter = (volatile u16*)0x4000006;
#define REG_DM3SAD       *(u32*)0x40000D4
#define REG_DM3DAD       *(u32*)0x40000D8
#define REG_DM3CNT       *(u32*)0x40000DC
int mx=60,my=80;                                                                //mouse position
int texture=0;                                                                  //what mouse texture to draw
int selected=4;                                                                 //button selected
#include "background.h"
#include "mouse.h"


void swapBuffers()			                                                    //flips between the back/front buffer
{
 while(*ScanlineCounter<160){}                                                  //waits for the drawer to get to end before flip
 if(REG_DISPCNT & BACKBUFFER)                                                   //backbuffer is the current buffer, switch to the fontbuffer
 { 
  REG_DISPCNT&= ~BACKBUFFER;                                                    //flip active buffer to front buffer by clearing backbuffer bit
  videoBuffer = BackBuffer;                                                     //now we point our drawing buffer to the back buffer
 }
 else                                                                           //front buffer is active so switch it to backbuffer
 { 
  REG_DISPCNT |= BACKBUFFER;                                                    //flip active buffer to backbuffer by setting back buffer bit
  videoBuffer = FrontBuffer;                                                    //now we point our drawing buffer to the front buffer
 }
}


void PlotPixel(int x,int y, unsigned short int c)                               //Mode 4 is 240(120)x160 by 8bit	
{
 videoBuffer[y*120+x]=c;
}


void DrawBackground()								                            //draws the background
{int i,x,y;
 REG_DM3SAD = (unsigned long)BData;
 REG_DM3DAD = (unsigned long)videoBuffer;
 REG_DM3CNT = 0x80000000 | 120*160;
 for(i=0;i<8;i++){ videoBuffer[i]=BData[8];}                                    //cover up background color pixels
}


void DrawButtons()	                                                            //draws the colored buttons
{int i,x,y,xo=0,yo=0;
 for(i=0;i<4;i++)
 {
  for(y=yo+130;y<yo+30+130;y++){ for(x=xo;x<xo+15;x++){PlotPixel(x,y,BData[i]);}}
  if(selected==i){ for(y=yo+130+2;y<yo+28+130;y++){ for(x=xo+1;x<xo+14;x++){PlotPixel(x,y,BData[6]);}}}
  yo-=40; xo+=20;
 }
}


void DrawMouse()	                                                            //draws the mouse
{int x,y;
 for(y=0;y<20;y++){ for(x=0;x<10;x++){ if(MData[y*20+x]!=0){ PlotPixel(x+mx,y+my,MData[y*20+x+texture]);}}}
}


void picking()                                                                  //check if mouse pixel equals palette color
{int i;
 for(i=0;i<4;i++)
 { 
  if(videoBuffer[my*120+mx-2]==BData[i]){ PlotPixel(0,0,BData[i]); selected=i; i=4;}
 }
}


void Keys()
{
 texture=0; 
 if(KEY_DOWN(KEYLEFT       )){ mx-=1;}                                          //left
 if(KEY_DOWN(KEYRIGHT      )){ mx+=1;}                                          //right
 if(KEY_DOWN(KEYS&KEYUP    )){ my-=2;}                                          //up
 if(KEY_DOWN(KEYS&KEYDOWN  )){ my+=2;}                                          //down
 if(KEY_DOWN(KEYS&KEYA)){ picking(); texture=10;}                               //down
 if(KEY_DOWN(KEYS&KEYB)){ picking(); texture=10;}                               //down
}


int main(void)
{int i;
 SetMode(MODE_4|BG2_ENABLE);                                                    //set mode 4
 for(i=0;i<256;i++){ paletteMem[i]=BPalette[i];}                                //load color palette
 
 while(1)                                                                       //Main loop
 {
  DrawBackground();                                        
  DrawButtons();
  DrawMouse();
  Keys();
  swapBuffers(); 			
 }
 return 0;
}

