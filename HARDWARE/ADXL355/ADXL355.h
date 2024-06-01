#ifndef __ADXL355_H
#define __ADXL355_H

#include "stm32f10x.h"
#include "iic.h"
#include "delay.h"

#define ADXL355_ADDR 0x1D // ADXL355 I2Cµÿ÷∑

#define ADXL355_DEVID_AD 0x00
#define ADXL355_DEVID_MST 0x01
#define ADXL355_PARTID 0x02
#define ADXL355_REVID 0x03
#define ADXL355_STATUS 0x04
#define ADXL355_FIFO_ENTRIES 0x05
#define ADXL355_TEMP2 0x06
#define ADXL355_TEMP1 0x07
#define ADXL355_XDATA3 0x08
#define ADXL355_XDATA2 0x09
#define ADXL355_XDATA1 0x0A
#define ADXL355_YDATA3 0x0B
#define ADXL355_YDATA2 0x0C
#define ADXL355_YDATA1 0x0D
#define ADXL355_ZDATA3 0x0E
#define ADXL355_ZDATA2 0x0F
#define ADXL355_ZDATA1 0x10
#define ADXL355_RANGE 0x2C
#define ADXL355_POWER_CTL 0x2D
#define ADXL355_RESET 0x2F
 
void ADXL355_WriteRegister(uint8_t reg, uint8_t value);
uint8_t ADXL355_ReadRegister(uint8_t reg);
void ADXL355_Init(void);
void ADXL355_SetRange(uint8_t range);
int32_t ADXL355_ReadAxis(uint8_t reg);
void ADXL355_ReadAccel(int32_t *x, int32_t *y, int32_t *z);

#endif
