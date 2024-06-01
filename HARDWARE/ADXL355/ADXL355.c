#include "ADXL355.h"
#include "iic.h"
#include "usart.h"
#include "delay.h"
//// Initialize the ADXL355
//void ADXL355_Init(void) {
//     ADXL355_WriteRegister(ADXL355_RESET, 0x52);  // 复位设备
//	delay_ms(100);
//    ADXL355_WriteRegister(ADXL355_POWER_CTL, 0x00); // Put the device into standby mode
//	ADXL355_WriteRegister(ADXL355_RANGE_2G,0x01);
//	
//}

void ADXL355_WriteRegister(uint8_t reg, uint8_t value) {
    IIC_Start();
    IIC_Send_Byte((ADXL355_ADDR << 1) | 0); // 发送从机地址和写命令
    IIC_Wait_Ack();
    IIC_Send_Byte(reg); // 发送寄存器地址
    IIC_Wait_Ack();
    IIC_Send_Byte(value); // 发送数据
    IIC_Wait_Ack();
    IIC_Stop();
}

uint8_t ADXL355_ReadRegister(uint8_t reg) {
    uint8_t value;
    IIC_Start();
    IIC_Send_Byte((ADXL355_ADDR << 1) | 0); // 发送从机地址和写命令
    IIC_Wait_Ack();
    IIC_Send_Byte(reg); // 发送寄存器地址
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((ADXL355_ADDR << 1) | 1); // 发送从机地址和读命令
    IIC_Wait_Ack();
    value = IIC_Read_Byte(0); // 读取数据，发送nACK
    IIC_Stop();
    return value;
}

void ADXL355_Init(void) {
    ADXL355_WriteRegister(ADXL355_RESET, 0x52); // 复位设备
    delay_ms(100);

    ADXL355_WriteRegister(ADXL355_POWER_CTL, 0x00); // 进入测量模式
    ADXL355_WriteRegister(ADXL355_RANGE,0x01); // 设置量程为±2g
}

//void ADXL355_SetRange(uint8_t range) {
//    uint8_t value = ADXL355_ReadRegister(ADXL355_RANGE);
//    value &= ~0x03; // 清除原有的量程设置
//    value |= range; // 设置新的量程
//    ADXL355_WriteRegister(ADXL355_RANGE, value);
//}

int32_t ADXL355_ReadAxis(uint8_t reg) {
	  int i;
    uint8_t rawData[3];
    int32_t value = 0;

    IIC_Start();
    IIC_Send_Byte((ADXL355_ADDR << 1) | 0); // 发送从机地址和写命令
    IIC_Wait_Ack();
    IIC_Send_Byte(reg); // 发送寄存器地址
    IIC_Wait_Ack();
    IIC_Start();
    IIC_Send_Byte((ADXL355_ADDR << 1) | 1); // 发送从机地址和读命令
    IIC_Wait_Ack();

    for ( i = 0; i < 3; i++) {
        rawData[i] = IIC_Read_Byte(i < 2); // 读取数据，前两次发送ACK，最后一次发送nACK
    }

    IIC_Stop();

    value = (int32_t)rawData[0] << 16;
    value |= (int32_t)rawData[1] << 8;
    value |= rawData[2];
    value >>= 4; // 数据是20位的，需要右移以便正确对齐

    return value;
}

void ADXL355_ReadAccel(int32_t *x, int32_t *y, int32_t *z) {
    *x = ADXL355_ReadAxis(ADXL355_XDATA3);
    *y = ADXL355_ReadAxis(ADXL355_YDATA3);
    *z = ADXL355_ReadAxis(ADXL355_ZDATA3);
}


