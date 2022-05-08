#include "uart.h"
#include "systick.h"
#include "stdint.h"
#include "board.h"
#include "queue.h"
#include "android.h"
#include <string.h>
#include <stdio.h>
#include "stm32f10x_usart.h"

#define ANDROID_BUFF_SIZE     10
QUEUEx_t AndroidCommandQueue;
android_cmd_t AndroidCommandBuff[ANDROID_BUFF_SIZE];

static void clear_buff(char *buff, uint32_t buffSize)
{
    for(int i = 0; i < buffSize; i++)
        buff[i] = '\0';
}

static void Android_CallbackHandle()
{
    static uint8_t revByteCount = 0;
    static uint8_t revBuff[ANDROID_CMD_MAX_SIZE];
    char revByte = 0;
    revByte = UART_GetData(ANDROID_UART);
    #if 0
    printf("recive byte: %c\r\n", revByte);
    #endif
    revBuff[revByteCount++] = revByte;
    if((revByteCount >= ANDROID_CMD_MAX_SIZE) || revByte == '\n')
    {
        QUEUE_Push(&AndroidCommandQueue, revBuff);
        #if DEBUG
        printf("queue push: %s\r\n", revBuff);
        #endif
        //clear buffer
        memset(revBuff, '\0', ANDROID_CMD_MAX_SIZE);
        revByteCount = 0;
    }
}

uint32_t ANDROID_Init()
{
	UART_CallbackInit(ANDROID_UART, Android_CallbackHandle);
	QUEUE_Init(&AndroidCommandQueue, (u8*)AndroidCommandBuff,\
        ANDROID_BUFF_SIZE, sizeof(android_cmd_t));
	UART_Init(ANDROID_UART,9600, USART_Mode_Tx|USART_Mode_Rx);
    USART_Cmd(ANDROID_UART, ENABLE);
    return 0;
}

void ANDROID_DeInit()
{
    USART_Cmd(ANDROID_UART,DISABLE);
}

uint32_t ANDROID_SendCmd(char *cmd, uint32_t cmdSize)
{
    UART_SendData(ANDROID_UART,(uint8_t *)cmd,cmdSize);
    return 0;
}