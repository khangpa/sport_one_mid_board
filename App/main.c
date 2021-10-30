/*
 * Copyright (c) 2021
 * Vitech, JSC.
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: Khangpa
 *
 * Last Changed By:  $Author: Khangpa $
 * Revision:         $Revision: xxx $
 * Last Changed:     $Date: xxx $
 */
#include "safety_checking.h"
#include "keypad.h"
#include "stdint.h"
#include "common.h"
#include "systick.h"
#include "start_mode.h"
#include "setup_mode.h"
#include "exercise_mode.h"
#include "run_mode.h"
#include "board.h"
#include "stop_mode.h"
#include "stm32f10x_gpio.h"
#include "screen.h"
#include "uart.h"
#include "timer3.h"
#include "power_communicate.h"
#include "stm32f10x_usart.h"
#include "dfplayer.h"

/*******************************************************************************
* Variable
*******************************************************************************/
run_mechine_data_t treadmillData;
volatile uint8_t  WarningFlag = 0;
volatile int sec = 3;
/*******************************************************************************
 * Definition
 ******************************************************************************/
/*******************************************************************************
* Private func
*******************************************************************************/
void seccount()
{
    sec--;
    WarningFlag = 1;
}
/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    SYSTICK_Init();
    SCREEN_Init();         
    KEYPAD_Init();
    SCREEN_Clear();
    SCREEN_MiddleNumber(sec);
    TIMER3_Init();
    TIMER3_CallbackInit(seccount);
    TIMER3_Start();
    POWER_COM_Init();
    DF_Init(5);
    program_state_t laststate = START;
    program_state_t state = START;
    treadmillData.runEx = 1;
    treadmillData.speed = 0;
    treadmillData.runTime = 0;
    treadmillData.distance = 0;
    treadmillData.incline = 0;
    while(1)
    {
        switch (state)
        {
            case START:
                state = start_mode(&treadmillData,&laststate);
                break;
            case RUN:
                state = run_mode(&treadmillData,&laststate);
                break;
            case STOP:
                state = stop_mode(&treadmillData,&laststate);
                break;
            case EXERCISE_SET:
                state = exercise_mode(&treadmillData,&laststate);
                break;
            case SET_UP:
                state = setup_mode(&treadmillData,&laststate);
                break;
            default:
                break;
        }
    }
}
