#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "adxl355.h"
#include "IIC.h"
 
 int main(void)
 {	
    int32_t x, y, z;
    SystemInit();
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // �����ж����ȼ�����2
	delay_init();	    //��ʱ������ʼ��	  
	LED_Init();		  	//��ʼ����LED���ӵ�Ӳ���ӿ�
    IIC_Init();
    ADXL355_Init();
     uart1_init(115200);
     //USART_Send_String(USART1, "usart1 test ok!!");
     //delay_ms(5000);
	while(1)
	{
         ADXL355_ReadAccel(&x, &y, &z);
      printf("%ld, %ld, %ld\n", x, y, z);
//printf("a:%d  b:%d  c:%d\r\n",a,b,c);
        //delay_ms(500);
       // LED_Blink();
	}
 }
