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
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
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
typedef enum cmd_type
{
    start = 0,
    stop,
    set,
    error
} cmd_type_t;
typedef struct main_cmd
{
    cmd_type_t cmd;
    uint32_t speed;
    uint32_t incline;
} main_cmd_t;
/*******************************************************************************
* Private func
*******************************************************************************/
power_com_cmd_t convert_android_to_power_cmd(android_cmd_t androidCmd);
android_cmd_t convert_power_cmd_to_android(power_com_cmd_t powerCmd);
void android_proc();
void keypad_proc();
void clear_data();
/*******************************************************************************
 * Code
 ******************************************************************************/
int main(void)
{
    startRunCmd.command = START_RUN;
    startRunCmd.length  = 0;
    startRunCmd.sequence = 0;
    startRunCmd.type    = MASTER_REQUEST_TYPE;
    startRunCmd.buff[0] = XOR_Calculator((uint8_t*)&startRunCmd, 0, POWER_COM_CMD_HEADER_SIZE);

    stopRunCmd.command = STOP_RUN;
    stopRunCmd.length  = 0;
    stopRunCmd.sequence = 0;
    stopRunCmd.type    = MASTER_REQUEST_TYPE;
    stopRunCmd.buff[0] = XOR_Calculator((uint8_t*)&stopRunCmd, 0, POWER_COM_CMD_HEADER_SIZE);

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
    }
}

void clear_data()
{
    treadmillData.incline = 0;
    treadmillData.speed = 0;
}

void keypad_proc()
{
    static keypad_info_t key;
    /* scan keypad */
    key = KEYPAD_ScanWithCheckHold(HOLD_TIME);
    switch (key.keyName)
    {
        case RUN_KEY:
            ANDROID_SendCmd("start\n", 6);
            POWER_COM_SendCmd(&startRunCmd, startRunCmd.length + 5);
            // default val:
            treadmillData.speed = 20;
            treadmillData.incline = 0;
            run_flag = 1;
            break;
        case STOP_KEY:
            ANDROID_SendCmd("end\n", 4);
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
            android_cmd_t androidCmdResend;
            POWER_COM_SendCmd(&cmdSend, cmdSend.length + 5);
            androidCmdResend = convert_power_cmd_to_android(cmdSend);
            ANDROID_SendCmd((char *)&androidCmdResend, strlen(androidCmdResend.cmd));
            #ifdef DEBUG
            printf("treadmillData speed : %d     Incline: %d\r\n", treadmillData.speed, treadmillData.incline);
            printf("androidCmdResend : %s\r\n", androidCmdResend.cmd);
            #endif
        }
        #ifdef DEBUG
        printf("IsDataChanged : %d\r\n", IsDataChanged);
        #endif
        IsDataChanged = NO;
    }
}

void android_proc()
{
    if(!QUEUE_Empty(&AndroidCommandQueue))
    {
        android_cmd_t androidCmdResend;
        memset(&androidCmdResend, '\0', sizeof(androidCmdResend));
        QUEUE_Get(&AndroidCommandQueue, (uint8_t *)&androidCmd);
        cmdSend = convert_android_to_power_cmd(androidCmd);
        #ifdef DEBUG
            printf("queue get: %s\r\n",androidCmd.cmd);
        #endif
        switch (cmdSend.command)
        {
        case START_RUN:
            run_flag = 1;
            treadmillData.speed = 20;
            treadmillData.incline = 0;
            break;
        case STOP_RUN:
            run_flag = 0;
            treadmillData.speed = 0;
            treadmillData.incline = 0;
            break;
        default:
            break;
        }
        if(cmdSend.command != 0xFF)
        {
            if((run_flag == 1) || (cmdSend.command == STOP_RUN))
            {
                POWER_COM_SendCmd(&cmdSend, cmdSend.length + 5);
                androidCmdResend = convert_power_cmd_to_android(cmdSend);
                ANDROID_SendCmd((char *)&androidCmdResend, strlen(androidCmdResend.cmd));
                #ifdef DEBUG
                printf("send cmd to android: %s\r\n",androidCmdResend.cmd);
                printf("treadmillData speed : %d     Incline: %d\r\n", treadmillData.speed, treadmillData.incline);
                #endif
            }
        }
        else
        {
            ANDROID_SendCmd("error\n", strlen("error\n"));
            #ifdef DEBUG
            printf("runflag = %d\r\n",run_flag);
            printf("cmdSend.command = %d\r\n", cmdSend.command);
            printf("send cmd to android: error");
            #endif
        }
    }
}

power_com_cmd_t convert_android_to_power_cmd(android_cmd_t androidCmd)
{
    power_com_cmd_t cmdRet;
    cmdRet.command = 0xFF;
    char *incPtr = NULL;
    uint32_t tempSpeed = 0;
    if(androidCmd.cmd[0] == 's')
    {
        if(androidCmd.cmd[1] == 't')
        {
            if(androidCmd.cmd[2] == 'a')
            {
                POWER_COM_ConverstDataToCmd(20 ,0);
                cmdRet = startRunCmd;
            }
        }
    }
    else if(androidCmd.cmd[0] == 'e')
    {
        if(androidCmd.cmd[1] == 'n')
        {
            if(androidCmd.cmd[2] == 'd')
            {
                POWER_COM_ConverstDataToCmd(0 ,0);
                cmdRet = stopRunCmd;
            }
        }
    }
    else if(run_flag == 1)
    {
        tempSpeed = (uint32_t) strtoul(androidCmd.cmd, &incPtr, 10);
        treadmillData.speed = 10 + tempSpeed/10;
        #if DEBUG
        printf("speed after convert: %d\r\n", treadmillData.speed);
        #endif
        treadmillData.incline = (uint32_t) strtoul(incPtr + 1 , (char **)NULL, 10);
        if(treadmillData.speed > 150)
            treadmillData.speed = 10;

        if(treadmillData.speed < 10)
            treadmillData.speed = 10;

        if(treadmillData.incline > 12)
            treadmillData.incline = 0;

        cmdRet = POWER_COM_ConverstDataToCmd((uint8_t)treadmillData.speed ,(uint8_t)treadmillData.incline);
    }
    return cmdRet;
}

android_cmd_t convert_power_cmd_to_android(power_com_cmd_t powerCmd)
{
    android_cmd_t cmdRet;
    memset(&cmdRet, '\0', sizeof(cmdRet));
    if(powerCmd.command == START_RUN)
        strcat(cmdRet.cmd,"start\n");
    else if(powerCmd.command == STOP_RUN)
        strcat(cmdRet.cmd,"end\n");
    else
    {
        sprintf(cmdRet.cmd,"%d",treadmillData.speed);
        strcat(cmdRet.cmd,";");
        sprintf(cmdRet.cmd + strlen(cmdRet.cmd),"%d",treadmillData.incline);
        strcat(cmdRet.cmd,"\n");
    }
    return cmdRet;
}
