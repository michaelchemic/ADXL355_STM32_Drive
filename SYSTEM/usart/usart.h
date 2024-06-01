#ifndef __USART_H
#define __USART_H

#include "stdio.h"	
#include "sys.h" 

extern uint8_t USAR1_RxData;

extern uint8_t USAR_RxFlag;
extern uint8_t Receive_Flag;

void uart1_init(u32 Baudrate);

void USAR_SendByte(uint8_t Byte);
void USAR_SendByte_One(uint8_t Byte);
void USAR_SendArray(uint8_t *Array, uint16_t Length);
void USAR_SendString(char *String);
uint32_t USAR_Pow(uint32_t X, uint32_t Y);
void USAR_SendNumber(uint32_t Number, uint8_t Length);
uint8_t USAR_GetRxFlag(void);

uint8_t USAR3_GetRxData(void);

void USART1_IRQHandler(void);

void USART_Send_Byte(USART_TypeDef* USARTx, uint16_t Data);
void USART_Send_String(USART_TypeDef* USARTx, char *str);


	
#endif
