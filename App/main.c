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
#include "keypad.h"
#include "stdint.h"
#include "common.h"
#include "systick.h"
#include "board.h"
#include "uart.h"
#include "power_communicate.h"
#include "android.h"
#include "queue.h"
#include "xor.h"
/*******************************************************************************
* Variable
*******************************************************************************/
extern QUEUEx_t AndroidCommandQueue;
run_mechine_data_t treadmillData;
power_com_cmd_t startRunCmd;
power_com_cmd_t stopRunCmd;
power_com_cmd_t cmdSend;
android_cmd_t androidCmd;
uint32_t run_flag = 0;
/*******************************************************************************
 * Definition
 ******************************************************************************/
/*******************************************************************************
* Private func
*******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
void clear_data()
{
    treadmillData.incline = 0;
    treadmillData.speed = 0;
    androidCmd.speed = 0;
    androidCmd.incline = 0;
}
void keypad_proc()
{
    static keypad_info_t key;
    /* scan keypad */
    key = KEYPAD_ScanWithCheckHold(HOLD_TIME);
    switch (key.keyName)
    {
        case RUN_KEY:
            androidCmd.speed = 0xff;
            ANDROID_SendCmd(&androidCmd, sizeof(androidCmd));
            POWER_COM_SendCmd(&startRunCmd, startRunCmd.length + 5);
            // default val:
            androidCmd.speed = 20;
            treadmillData.speed = 20;
            androidCmd.incline = 0;
            treadmillData.incline = 0;
            run_flag = 1;
            break;
        case STOP_KEY:
            androidCmd.speed = 0xfe;
            ANDROID_SendCmd(&androidCmd, sizeof(androidCmd));
            POWER_COM_SendCmd(&stopRunCmd, stopRunCmd.length + 5);
            clear_data();
            run_flag = 0;
            break;
        case INCLINE_3_KEY:
            treadmillData.incline =  3;
            IsDataChanged = YES;
            break;
        case INCLINE_6_KEY:
            treadmillData.incline =  6;
            IsDataChanged = YES;
            break;
        case INCLINE_9_KEY:
            treadmillData.incline = 9;
            IsDataChanged = YES;
            break;
        case SPEED_3_KEY:
            treadmillData.speed = 30;
            IsDataChanged = YES;
            break;
        case SPEED_6_KEY:
            treadmillData.speed = 60;
            IsDataChanged = YES;
            break;
        case SPEED_9_KEY:
            treadmillData.speed = 90;
            IsDataChanged = YES;
            break;
        case PLUS_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            treadmillData.speed += 1;
            if(treadmillData.speed > 150)
                treadmillData.speed = 150;
            else
                IsDataChanged = YES;
            break;
        case MINUS_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            treadmillData.speed -= 1;
            if(treadmillData.speed < 10)
                treadmillData.speed = 10;
            else
                IsDataChanged = YES;
            break;
        case UP_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            treadmillData.incline += 1;
            if(treadmillData.incline > 12)
                treadmillData.incline = 12;
            else
                IsDataChanged = YES;
            break;
        case DOWN_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            treadmillData.incline -= 1;
            if(treadmillData.incline > 12 )
                treadmillData.incline = 0;
            else
                IsDataChanged = YES;
            break;
        default:
            IsDataChanged = NO;
            break;
    }
    if((IsDataChanged == YES) && run_flag)
    {
        cmdSend = POWER_COM_ConverstDataToCmd(treadmillData.speed ,treadmillData.incline);
        if(cmdSend.command != 0xFF)
        {
            POWER_COM_SendCmd(&cmdSend, cmdSend.length + 5);
            androidCmd.speed = treadmillData.speed;
            androidCmd.incline = treadmillData.incline;
            ANDROID_SendCmd(&androidCmd, sizeof(androidCmd));
        }
        IsDataChanged = NO;
    }
}

void android_proc()
{
    if(!QUEUE_Empty(&AndroidCommandQueue))
    {
        QUEUE_Get(&AndroidCommandQueue, (uint8_t *)&androidCmd);
        if(androidCmd.speed == 0xFF)
        {
            POWER_COM_SendCmd(&startRunCmd, startRunCmd.length + 5);
            androidCmd.speed = 20;
            androidCmd.incline = 0;
            treadmillData.speed = 20;
            treadmillData.incline = 0;
            run_flag = 1;
        }
        else if(androidCmd.speed == 0xFE)
        {
            POWER_COM_SendCmd(&stopRunCmd, startRunCmd.length + 5);
            clear_data();
            run_flag = 0;
        }
        else if(run_flag && (androidCmd.speed != 0xFF))
        {
            if(androidCmd.speed > 150)
                androidCmd.speed = 150;
            if(androidCmd.speed < 10)
                androidCmd.speed = 10;
            if(androidCmd.incline > 12)
                androidCmd.speed = 12;
            treadmillData.speed = androidCmd.speed;
            treadmillData.incline = androidCmd.incline;
            cmdSend = POWER_COM_ConverstDataToCmd(androidCmd.speed ,androidCmd.incline);
            if(cmdSend.command != 0xFF)
                POWER_COM_SendCmd(&cmdSend, cmdSend.length + 5);
        }
        /* resend raw message was received to android board */
        ANDROID_SendCmd(&androidCmd, sizeof androidCmd);
    }
}

int main(void)
{
    startRunCmd.command = START_RUN;
    startRunCmd.length  = 0;
    startRunCmd.sequence = 0;
    startRunCmd.type    = MASTER_REQUEST_TYPE;
    startRunCmd.buff[0] = XOR_Calculator((uint8_t*)&cmdSend, 0, POWER_COM_CMD_HEADER_SIZE);

    stopRunCmd.command = STOP_RUN;
    stopRunCmd.length  = 0;
    stopRunCmd.sequence = 0;
    stopRunCmd.type    = MASTER_REQUEST_TYPE;
    stopRunCmd.buff[0] = XOR_Calculator((uint8_t*)&cmdSend, 0, POWER_COM_CMD_HEADER_SIZE);

    androidCmd.speed = 0;
    androidCmd.incline = 0;
    treadmillData.runEx = 0;
    treadmillData.speed = 0;
    treadmillData.runTime = 0;
    treadmillData.distance = 0;
    treadmillData.incline = 0;
    SYSTICK_Init();
    KEYPAD_Init();
    POWER_COM_Init();
    ANDROID_Init();
    while(1)
    {
        keypad_proc();
        android_proc();
        //POWER_COM_SendCmd(&startRunCmd, startRunCmd.length +5);
        //ANDROID_SendCmd(&androidCmd,2);
    }
}

