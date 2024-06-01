#include "usart.h"
#include "stdio.h"
uint8_t USAR1_RxData;

uint8_t USAR_RxFlag;
uint8_t Receive_Flag;

// 加入以下代码,支持printf函数,而不需要选择use MicroLIB
#if 1
#pragma import(__use_no_semihosting)
// 标准库需要的支持函数
struct __FILE {
    int handle;
};

FILE __stdout;
// 定义_sys_exit()以避免使用半主机模式
void _sys_exit(int x)
{
    x = x;
}
// 重定义fputc函数
int fputc(int ch, FILE *f)
{
    while ((USART1->SR & 0X40) == 0)
        ; // 循环发送,直到发送完毕
    USART1->DR = (u8)ch;
    return ch;
}
#endif
// 串口1配置部分
void uart1_init(u32 Baudrate)
{
	  GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
	  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); // 开启GPIO时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; // 复用推挽输出
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;      // 开启GPIOA9
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IPU; // 上拉输入
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10;   // 开启GPIOA10
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    
    USART_InitStructure.USART_BaudRate            = Baudrate;                       // 设置波特率为Baudrate
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; // 硬件流控制-不使用流控
    USART_InitStructure.USART_Mode                = USART_Mode_Tx | USART_Mode_Rx;  // 设置为发送+接收模式
    USART_InitStructure.USART_Parity              = USART_Parity_No;                // 校验位：No无校验(√)  Odd奇校验  Even偶校验
    USART_InitStructure.USART_StopBits            = USART_StopBits_1;               // 停止位：1位停止位
    USART_InitStructure.USART_WordLength          = USART_WordLength_8b;            // 字长：8位(因为此程序中无校验位)
    USART_Init(USART1, &USART_InitStructure);

    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); // 开启RXNE标志位到NVIC的输出

    USART_Cmd(USART1, ENABLE); // 使能串口
		
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
	
}


/*----------------------以下为发送(Txd)有关函数-----------------------------*/

void USAR_SendByte(uint8_t Byte) // 从Tx引脚发送一个字节数据-(只支持16进制数据如0x41='A')
{
    USART_SendData(USART3, Byte);
    while (USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET)
        ; // 等待数据传输结束
}

void USAR_SendByte_One(uint8_t Byte) // 从Tx引脚发送一个字节数据-(只支持16进制数据如0x41='A')
{
    USART_SendData(USART1, Byte);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
        ; // 等待数据传输结束
}

void USAR_SendByte_Two(uint8_t Byte) // 从Tx引脚发送一个字节数据-(只支持16进制数据如0x41='A')
{
    USART_SendData(USART2, Byte);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
        ; // 等待数据传输结束
}

////////////////////////////////////
void USAR_SendArray(uint8_t *Array, uint16_t Length) // 从Tx引脚发送提前定义的数组的数据 USAR_SendArray(Array,5)
{
    uint16_t i;
    for (i = 0; i < Length; i++) {
        USAR_SendByte(Array[i]);
    }
}

void USAR_SendString(char *String) // 从Tx引脚发送字符串
{
    uint8_t i;
    for (i = 0; String[i] != '\0'; i++) {
        USAR_SendByte(String[i]);
    }
}
////////////////////////////////////////////////////////////////////////////////
uint32_t USAR_Pow(uint32_t X, uint32_t Y) // 从Tx引脚发送一个数字(辅助)
{
    uint32_t Result = 1;
    while (Y--) {
        Result *= X;
    }
    return Result;
}

void USAR_SendNumber(uint32_t Number, uint8_t Length) // 从Tx引脚发送一个数字
{
    uint8_t i;
    for (i = 0; i < Length; i++) {
        USAR_SendByte(Number / USAR_Pow(10, Length - i - 1) % 10 + '0');
    }
}

/*----------------------以下为接收(Rxd)有关函数-----------------------------*/

uint8_t USAR_GetRxFlag(void) // 得到接收数据的标志位
{
    if (USAR_RxFlag == 1) {
        USAR_RxFlag = 0;
        return 1;
    }
    return 0;
}

uint8_t USAR1_GetRxData(void) // 得到接收到的数据的值
{
    return USAR1_RxData;
}


//当前只用串口发送数据到上位机
//void USART1_IRQHandler(void)
//{
//    if (USART_GetITStatus(USART1, USART_IT_RXNE) == SET) // 如果串口接收到数据
//    {
//        USAR1_RxData = USART_ReceiveData(USART1); // 接收数据赋予全局变量USAR_RxData
//        USAR_RxFlag  = 1;
//        USART_ClearITPendingBit(USART1, USART_IT_RXNE);

//        if (USAR1_RxData == 0xAA) {
//            rx_index              = 0;            // 重新开始接收数据
//            rx_buffer[rx_index++] = USAR1_RxData; // 存储帧头
//        } else if (rx_index != 0) {
//            rx_buffer[rx_index++] = USAR1_RxData; // 存储数据

//            if (USAR1_RxData == 0x55) { // 若接收到帧尾

//                if (rx_index == 5) { // 检查数据包格式是否正确
//                    // 解析数据

//                     Motor_Function();

//                    // 处理完成后可以清除接收缓冲区
//                    rx_index = 0;
//                } else {
//                    // 数据包格式不正确，重新开始接收数据
//                    rx_index = 0;
//                }
//            }
//            // 防止缓冲区溢出
//            else if (rx_index >= RX_BUFFER_SIZE) {
//                rx_index = 0; // 重新开始接收数据
//            }
//        }
//    }
//}



//////////////////////////////////////////////////////////////////////////
/**
 * 功能：串口写字节函数
 * 参数1：USARTx ：串口号
 * 参数2：Data   ：需写入的字节
 * 返回值：None
 */
void USART_Send_Byte(USART_TypeDef *USARTx, uint16_t Data)
{
    USART_SendData(USARTx, Data);
    while (USART_GetFlagStatus(USARTx, USART_FLAG_TXE) == RESET)
        ;
}
/**
 * 功能：串口写字符串函数
 * 参数1：USARTx ：串口号
 * 参数2：str    ：需写入的字符串
 * 返回值：None
 */
void USART_Send_String(USART_TypeDef *USARTx, char *str)
{
    uint16_t i = 0;
    do {
        USART_Send_Byte(USARTx, *(str + i));
        i++;
    } while (*(str + i) != '\0');

    while (USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
}

