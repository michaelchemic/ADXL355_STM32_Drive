#include "led.h"
#include "delay.h"
void LED_Init(void)
{
 
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);	 
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;				
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 
 GPIO_Init(GPIOC, &GPIO_InitStructure);					 
 GPIO_SetBits(GPIOC,GPIO_Pin_13);						

}
 void LED_Blink(void)
 {
		LED0=0;
		delay_ms(100);	 //—” ±300ms
		LED0=1;
		delay_ms(100);	//—” ±300ms
 }
 
 
