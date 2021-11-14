#include "uart.h"
#include "systick.h"
#include "stdint.h"
#include "board.h"
#include "queue.h"
#include "android.h"
#include "stm32f10x_usart.h"
#define ANDROID_CMD_BUFF_MAX_SIZE     3
QUEUEx_t AndroidCommandQueue;
android_cmd_t AndroidCommandBuff[ANDROID_CMD_BUFF_MAX_SIZE];

static void Android_CallbackHandle()
{
    static uint8_t revByteCount = 0;
    static uint8_t revBuff[3];
    uint8_t revByte = 0;
    
    revByte = UART_GetData(ANDROID_UART);
    revBuff[revByteCount++] = revByte;
    if(revByteCount >= 2)
    {
        QUEUE_Push(&AndroidCommandQueue, revBuff);
        revByteCount = 0;
    }
}

uint32_t ANDROID_Init()
{
	UART_CallbackInit(ANDROID_UART, Android_CallbackHandle);
	QUEUE_Init(&AndroidCommandQueue, (u8*)AndroidCommandBuff,\
                ANDROID_CMD_BUFF_MAX_SIZE, sizeof(android_cmd_t));
	UART_Init(ANDROID_UART,9600, USART_Mode_Tx|USART_Mode_Rx);
    USART_Cmd(POWER_COM_UART, ENABLE);
    return 0;
}

void ANDROID_DeInit()
{
    USART_Cmd(ANDROID_UART,DISABLE);
}

uint32_t ANDROID_SendCmd(android_cmd_t *cmd, uint32_t cmdSize)
{
    UART_SendData(ANDROID_UART,(uint8_t *)cmd,cmdSize);
    return 0;
}