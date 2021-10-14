#include <stdint.h>
#include "stm32f10x_usart.h"
#include "uart.h"
#include "board.h"
#include "queue.h"
#include "xor.h"
#include "power_communicate.h"

/*******************************************************************************
* Definition
*******************************************************************************/
#define POWER_CMD_BUFF_MAX_SIZE     3
/*******************************************************************************
* Variable
*******************************************************************************/
QUEUEx_t PowerCommandQueue;
power_com_cmd_t PowerCommandBuff[POWER_CMD_BUFF_MAX_SIZE];

/*******************************************************************************
* Private func
*******************************************************************************/
/*!
 * @brief functions POWER_COM_CallbackHandle.
 *
 */
static void POWER_COM_CallbackHandle()
{
    static uint8_t revByteCount = 0;
    static uint8_t revBuff[POWER_COM_CMD_HEADER_SIZE + POWER_COM_CMD_BUFF_MAX_LENGH + 1];
    uint8_t revByte = 0;
    
    revByte = UART_GetData(POWER_COM_UART);
    revBuff[revByteCount++] = revByte;
    if(revByteCount >= POWER_COM_CMD_HEADER_SIZE)
    {
        if(revByteCount == POWER_COM_CMD_HEADER_SIZE + revBuff[3] + 1)
        {
            if(revBuff[POWER_COM_CMD_HEADER_SIZE + revBuff[3]] ==\
                XOR_Calculator(revBuff, 0, POWER_COM_CMD_HEADER_SIZE + revBuff[3]))
            {
                QUEUE_Push(&PowerCommandQueue, revBuff);
            }
            revByteCount = 0;
        }
    }
}

/*******************************************************************************
* Code
*******************************************************************************/
uint32_t POWER_COM_Init()
{
    UART_CallbackInit(POWER_COM_UART, POWER_COM_CallbackHandle);
    
    UART_Init(POWER_COM_UART,9600,USART_Mode_Tx|USART_Mode_Rx);
    QUEUE_Init(&PowerCommandQueue, (u8*)PowerCommandBuff,\
                POWER_CMD_BUFF_MAX_SIZE, sizeof(power_com_cmd_t));
    USART_Cmd(POWER_COM_UART, ENABLE);
    return 0;
}

uint32_t POWER_COM_SendCmd(power_com_cmd_t *command, uint32_t commandLength)
{
    UART_SendData(POWER_COM_UART,(uint8_t *)command,command->length+5);
    return 0;
}

/*!
 * @brief convert data to the command
 *
 * @param speed     speed of machine
 * @param incline   incline of machine
 * @return command
*/
power_com_cmd_t POWER_COM_ConverstDataToCmd(uint8_t speed, uint8_t incline)
{
    static uint8_t lastSpeed = 0;
    static uint8_t lastIncline = 0;
    power_com_cmd_t cmdReturn;
    if(lastSpeed != speed)
    {
        cmdReturn.command = SET_SPEED_MOTOR;
        cmdReturn.type    = MASTER_REQUEST_TYPE;
        cmdReturn.sequence= 0x00;
        cmdReturn.length  = 0x01;
        cmdReturn.buff[0] = speed;
        cmdReturn.buff[1] = XOR_Calculator((uint8_t*)&cmdReturn,0,5);
        lastSpeed = speed;
    }
    else if(lastIncline != incline)
    {
        cmdReturn.command = SET_INCLINE;
        cmdReturn.type    = MASTER_REQUEST_TYPE;
        cmdReturn.sequence= 0x00;
        cmdReturn.length  = 0x01;
        cmdReturn.buff[0] = incline;
        cmdReturn.buff[1] = XOR_Calculator((uint8_t*)&cmdReturn,0,5);
        lastIncline = incline;
    }
    else
        cmdReturn.command = 0xff;
    return (cmdReturn);
}


uint8_t POWER_COM_GetCmd(power_com_cmd_t *power_command)
{
    if(!QUEUE_Empty(&PowerCommandQueue))
        QUEUE_Get(&PowerCommandQueue, (u8*)&power_command);
    return 0;
}
/*******************************************************************************
* EOF
*******************************************************************************/
