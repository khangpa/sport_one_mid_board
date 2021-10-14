#ifndef __UART_H__
#define __UART_H__
#include "stm32f10x.h"
/*******************************************************************************
* Definitio
*******************************************************************************/
typedef void (*UART_HandleTypeDef)(void);
/*******************************************************************************
* 
*******************************************************************************/

void UART_Init(USART_TypeDef* USARTx, u32 baudrate, u16 mode);
void UART_CallbackInit(USART_TypeDef* USARTx, UART_HandleTypeDef pHandle);
void UART_SendData(USART_TypeDef* USARTx, u8* buff, u8 len);
uint8_t UART_GetData(USART_TypeDef* USARTx);
#endif /* __UART_H__ */
/*******************************************************************************
* EOF
*******************************************************************************/
