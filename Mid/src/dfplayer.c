#include "uart.h"
#include "systick.h"
#include "stdint.h"
#include "board.h"
#include "dfplayer.h"
#include "softuart.h"

#define Source      0x02  // TF CARD

#define Start_Byte 0x7E
#define End_Byte   0xEF
#define Version    0xFF
#define Cmd_Len    0x06
#define Feedback   0x00    //If need for Feedback: 0x01,  No Feedback: 0

void Send_cmd (uint8_t cmd, uint8_t Parameter1, uint8_t Parameter2)
{
	uint16_t Checksum = Version + Cmd_Len + cmd + Feedback + Parameter1 + Parameter2;
	Checksum = 0-Checksum;

	uint8_t CmdSequence[10] = { Start_Byte, Version, Cmd_Len, cmd, Feedback, Parameter1, Parameter2, (Checksum>>8)&0x00ff, (Checksum&0x00ff), End_Byte};

	Softuart_SendData(CmdSequence,10);
}

void DF_PlayFromStart(void)
{
  	Send_cmd(0x03,0x00,0x01);
  	SYSTICK_Delay_ms(200);
}

void DF_Play(uint32_t fileName)
{
 	Send_cmd(0x03,0x00,fileName);
  	SYSTICK_Delay_ms(200);
}

void DF_Init (uint8_t volume)
{
	// UART_Init(DF_UART,9600, USART_Mode_Tx);
    // Softuart_SendData(DF_UART,ENABLE);
	Softuart_Init(9600);
	Send_cmd(0x3F, 0x00, Source);
	SYSTICK_Delay_ms(200);
	Send_cmd(0x06, 0x00, volume);
	SYSTICK_Delay_ms(500);
}

void DF_DeInit()
{
    // Softuart_SendData(DF_UART,DISABLE);
}
void DF_Next (void)
{
    Send_cmd(0x01, 0x00, 0x00);
    SYSTICK_Delay_ms(200);
}

void DF_Pause (void)
{
	Send_cmd(0x0E, 0, 0);
	SYSTICK_Delay_ms(200);
}

void DF_Previous (void)
{
	Send_cmd(0x02, 0, 0);
	SYSTICK_Delay_ms(200);
}

void DF_Playback (void)
{
	Send_cmd(0x0D, 0, 0);
	SYSTICK_Delay_ms(200);
}













