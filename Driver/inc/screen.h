#ifndef _SCREEN_H_
#define _SCREEN_H_

#include "common.h"

/*******************************************************************************
 * Definitions
 ******************************************************************************/
typedef enum 
{
    RUN_TIME = 0,
    SET_UP_MODE
}State_Update_Data;
/*******************************************************************************
 * API
 ******************************************************************************/
void clear_Dot(unsigned char *data,uint8_t address);
/* void clear_SET_Time(uint32_t time,uint8_t address);
void display_Set(unsigned char *data,uint8_t address); */
void SCREEN_Init();
void SCREEN_Clear();
void SCREEN_ClearSection(uint8_t address, uint8_t size);
void SCREEN_MiddleNumber(int number);
void SCREEN_UpdateTime(uint32_t time);
void SCREEN_UpdateDistance(uint32_t dis);
void SCREEN_UpdateCalo(uint32_t cal);
void SCREEN_UpdateIncline(uint8_t incl);
void SCREEN_UpdateSpeed(uint32_t speed);
void SCREEN_UpdateEx(uint8_t runEx);
void SCREEN_Tone();
void SCREEN_ERROR(uint32_t error);
void mainScreen();
#endif /*_SCREEN_H_*/