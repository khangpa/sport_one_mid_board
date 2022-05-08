#ifndef __SOFTUART_H__
#define __SOFTUART_H__
#include <stdint.h>
void Softuart_Init();
void Softuart_SendByte(uint8_t byte);
void Softuart_send_frame(uint8_t startbyte, uint8_t addressbyte,
                                    uint8_t databyte, uint8_t stopbyte);
void Softuart_SendData(uint8_t* buff, uint8_t length);
#endif /* __SOFTUART_H__ */