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
/*******************************************************************************
 * Definition
 ******************************************************************************/
/*******************************************************************************
* Private func
*******************************************************************************/
/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    keypad_info_t key;
    power_com_cmd_t startRunCmd;
    power_com_cmd_t stopRunCmd;
    power_com_cmd_t cmdSend;
    android_cmd_t android_cmd;
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

    android_cmd.speed = 0;
    android_cmd.incline = 0;
    treadmillData.runEx = 1;
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
        key = KEYPAD_ScanWithCheckHold(500);
        if(key.keyName == START_RUN)
            POWER_COM_SendCmd(&startRunCmd, cmdSend.length + 5);
        if(key.keyName == STOP_RUN)
            POWER_COM_SendCmd(&stopRunCmd, cmdSend.length + 5);
        if(!QUEUE_Empty(&AndroidCommandQueue))
        {
            QUEUE_Get(&AndroidCommandQueue, (uint8_t *)&android_cmd);
            cmdSend = POWER_COM_ConverstDataToCmd(android_cmd.speed , android_cmd.incline);
            if(cmdSend.command != 0xFF)
                POWER_COM_SendCmd(&cmdSend, cmdSend.length + 5);
        }
    }
    
}
