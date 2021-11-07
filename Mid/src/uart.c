#include "stdio.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "board.h"
#include "uart.h"
/*******************************************************************************
* Definition
*******************************************************************************/
UART_HandleTypeDef    UART3_CallbackFunc = NULL;
UART_HandleTypeDef    UART2_CallbackFunc = NULL;
/*******************************************************************************
* Code
*******************************************************************************/
void UART_PinInit(USART_TypeDef* USARTx)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    if(USARTx == USART1)
    {
        GPIO_PinRemapConfig(GPIO_Remap_USART1,ENABLE);
        /*Config USART1 Rx as input floating */
        GPIO_InitStructure.GPIO_Pin = POWER_COM_RxPIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(POWER_COM_GPIO, &GPIO_InitStructure);
        /*Config USART1 Tx as alternate function pp*/
        GPIO_InitStructure.GPIO_Pin = POWER_COM_TxPIN;
        GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(POWER_COM_GPIO, &GPIO_InitStructure);
    }
    else if(USARTx == USART2)
    {
        GPIO_InitStructure.GPIO_Pin = ANDROID_TxPIN;
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
        GPIO_Init(ANDROID_GPIO, &GPIO_InitStructure);
    }
}

void UART_ClockInit(USART_TypeDef* USARTx)
{
    if(USARTx == USART1)
    {
        /*Enable GPIO clock*/
        RCC_APB2PeriphClockCmd(POWER_COM_GPIO_CLK, ENABLE);
        /*Enable AFIO clock*/
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        /*Enable UART clock*/
        RCC_APB2PeriphClockCmd(POWER_COM_CLK, ENABLE);
    }
    else if(USARTx == USART2)
    {
        /*Enable GPIO clock*/
        RCC_APB2PeriphClockCmd(ANDROID_GPIO_CLK, ENABLE);
        /*Enable AFIO clock*/
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
        /*Enable UART clock*/
        RCC_APB1PeriphClockCmd(ANDROID_CLK, ENABLE);
    }
}


void UART_Init(USART_TypeDef* USARTx, u32 baudrate, u16 mode)
{
    USART_InitTypeDef USART_InitStructure;
    UART_ClockInit(USARTx);
    UART_PinInit(USARTx);
    
    /*UARTx Config*/
    USART_InitStructure.USART_BaudRate = baudrate;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = mode;
    
    /*USART Config Init*/
    USART_Init(USARTx, &USART_InitStructure);

    /* Enable USARTx Receive interrupts */
    USART_ITConfig(USARTx, USART_IT_RXNE, ENABLE);

    /* NVIC Config */
    NVIC_EnableIRQ(ANDROID_IRQn);
    /*Enable UARTx*/
    USART_Cmd(USARTx, ENABLE);
}


void UART_CallbackInit(USART_TypeDef* USARTx, UART_HandleTypeDef pHandle)
{
    if(USARTx == POWER_COM_UART)
        UART3_CallbackFunc = pHandle;
    if(USARTx == ANDROID_UART)
        UART2_CallbackFunc = pHandle;
}


void UART_SendData(USART_TypeDef* USARTx, uint8_t* buff, uint8_t len)
{
    for(uint8_t i = 0; i < len; i++)
    {
        
        while(USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET);
        USART_SendData(USARTx, buff[i]);
    }
}

uint8_t UART_GetData(USART_TypeDef* USARTx)
{
    uint8_t datarx;
    //while(USART_GetFlagStatus(USARTx, USART_FLAG_RXNE) == RESET);
    datarx = (uint8_t)USART_ReceiveData(USARTx);
    return datarx;
}