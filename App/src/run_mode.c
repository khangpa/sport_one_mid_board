#include "keypad.h"
#include "systick.h"
#include "screen.h"
#include "timer3.h"
#include "xor.h"
#include "power_communicate.h"
#include "run_mode.h"
/*******************************************************************************
* Definition
*******************************************************************************/
#define DEFAULT_RUN_SPEED   10
#define MAX_RUN_SPEED       70
#define FLAG_OFF            0
#define FLAG_ON             1
typedef enum 
{
    COUNT_UP_ALL,
    COUNT_DOWN_TIME,
    COUNT_DOWN_DIS,
    COUNT_DOWN_CALO
} type_of_count;
/*******************************************************************************
 * Variable
 ******************************************************************************/
volatile uint32_t Sec;
type_of_count HowToCountData = COUNT_UP_ALL;
uint32_t ExeRunFlag = FLAG_OFF;
uint8_t Execrise[12][15]= {
                            {0x11,0x21,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11,0x11},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                            };
/*******************************************************************************
*Private func
*******************************************************************************/

/*!
 * @brief 
 *
 * @param 
 * @param 
*/
static void countSec()
{
    if(HowToCountData != COUNT_DOWN_TIME)
    {
        Sec++;
        if(Sec == 0xFFFFFFFF)
        Sec = 0;
    }
    else
    {
        if(Sec != 0);
        Sec--;
    }
    /* time data change */
    IsDataChanged = YES;
}

/*!
 * @brief function count seccond with tone
 *
 * @param none
*/
static void countSecWithTone()
{
    Sec++;
    SCREEN_Tone();
}

/*!
 * @brief waitting screen 3s before start
 *
 * @param 
 * @param 
*/

static void waittingScreen(run_mechine_data_t *mechineDate)
{
    SCREEN_Clear();
    SCREEN_UpdateCalo(mechineDate->calo);
    SCREEN_UpdateDistance(mechineDate->distance);
    SCREEN_UpdateIncline(mechineDate->incline);
    SCREEN_UpdateTime(mechineDate->runTime);
    TIMER3_Init();
    TIMER3_CallbackInit(countSecWithTone);
    TIMER3_Start();
        /* screen countdown 3s */
    while((Sec<4)&&(KEYPAD_ScanKey()!=STOP_KEY))
    {
        NVIC_DisableIRQ(TIM3_IRQn);
        SCREEN_MiddleNumber(4-Sec);
        NVIC_EnableIRQ(TIM3_IRQn);
    }
    TIMER3_STOP();
}


/*!
 * @brief check the last state of the machine to change default and method count data
 *        by changing the ExeRunFlag and HowToCountData variables
 *
 * @param treadmillData
 * @param lastState
*/
static void check_last_state_and_change_run_info(run_mechine_data_t *treadmillData, program_state_t *laststate)
{
    switch (*laststate)
    {
        case START:
            treadmillData->speed = DEFAULT_RUN_SPEED;
            ExeRunFlag = FLAG_OFF;
            HowToCountData = COUNT_UP_ALL;
            break;
        case SET_UP:
            if(treadmillData->runTime != 0)
            {
                Sec = treadmillData->runTime;
                HowToCountData = COUNT_DOWN_TIME;
            }
            else if(treadmillData->distance != 0)
                HowToCountData = COUNT_DOWN_DIS;
            else 
                HowToCountData = COUNT_DOWN_CALO;
            break;
        case EXERCISE_SET:
            Sec = treadmillData->runTime;
            HowToCountData = COUNT_DOWN_TIME;
            ExeRunFlag    = FLAG_ON;
            break;
        default:
            break;
    }
}

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief The run mode
 *
 * @param treadmillData
 * @param laststate 
 * @return State of program
*/
program_state_t run_mode(run_mechine_data_t *treadmillData, program_state_t *laststate)
{
    program_state_t stateReturn;
    keypad_info_t key;
    key.keyName = NO_KEY;
    key.keypad_state = RELEAS_STATE;
    static power_com_cmd_t cmdSend;
    /* Variable to calculate when distances and calorie values change */
    static uint32_t displaySpeed;
    static uint32_t startTickForChangeDistance;
    static uint32_t startTickForChangeCalo;
    static uint32_t changeMoment;
    static uint32_t numberOfChange;
    if(IsThisTheFirstTimeRun == YES)
    {
        cmdSend.command = START_RUN;
        cmdSend.length  = 0;
        cmdSend.sequence = 0;
        cmdSend.type    = MASTER_REQUEST_TYPE;
        cmdSend.buff[0] = XOR_Calculator((uint8_t*)&cmdSend, 0, POWER_COM_CMD_HEADER_SIZE);
        waittingScreen(treadmillData);
        Sec = treadmillData->runTime;
        startTickForChangeCalo = 0;
        /* return immediately when pressing the stop key */
        if(KEYPAD_ScanKey()==STOP_KEY)
        {
            Sec = 0;
            return STOP;
        }
        check_last_state_and_change_run_info(treadmillData,laststate);
        /* Start timer for count time */
        TIMER3_CallbackInit(countSec);
        TIMER3_Start();
        numberOfChange = 0;
        changeMoment = treadmillData->runTime *14/15;
        treadmillData->speed = DEFAULT_RUN_SPEED;
        displaySpeed = DEFAULT_RUN_SPEED;
        /* send start cmd */
        POWER_COM_SendCmd(&cmdSend, cmdSend.length + 5);
        IsThisTheFirstTimeRun = NO;
        startTickForChangeDistance = SYSTICK_GetTick();
    }

    /* update mechindata_runtime */
    treadmillData->runTime = Sec;
    /* update screen and send data to lower layer*/
    if(IsDataChanged == YES)
    {
        /* show screen */
        SCREEN_UpdateTime(treadmillData->runTime);
        SCREEN_UpdateIncline(treadmillData->incline);
        SCREEN_UpdateSpeed(treadmillData->speed);
        /* send data to lower layer */
        if(cmdSend.command != START_RUN)
            cmdSend = POWER_COM_ConverstDataToCmd(treadmillData->speed, treadmillData->incline);
        else
            cmdSend.command = 0xFF;
        if(cmdSend.command != 0xFF)
            POWER_COM_SendCmd(&cmdSend, cmdSend.length + 5);
        IsDataChanged = NO;
    }

    /* scan keypad */
    key = KEYPAD_ScanWithCheckHold(HOLD_TIME);
    switch (key.keyName)
    {
        case STOP_KEY:
            SCREEN_Tone();
            IsDataChanged = YES;
            IsThisTheFirstTimeRun = YES;
            ExeRunFlag = FLAG_OFF;
            HowToCountData = COUNT_UP_ALL;
            TIMER3_STOP();
            Sec = 0;
            stateReturn = STOP;
            break;
        case INCLINE_3_KEY:
            treadmillData->incline =  3;
            SCREEN_Tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case INCLINE_6_KEY:
            treadmillData->incline =  6;
            SCREEN_Tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case INCLINE_9_KEY:
            treadmillData->incline = 9;
            SCREEN_Tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_3_KEY:
            treadmillData->speed = 20;
            SCREEN_Tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_6_KEY:
            treadmillData->speed = 35;
            SCREEN_Tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case SPEED_9_KEY:
            treadmillData->speed = 50;
            SCREEN_Tone();
            IsDataChanged = YES;
            stateReturn = RUN;
            break;
        case PLUS_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            displaySpeed += 1;
            if(displaySpeed % 2 == 0)
                treadmillData->speed += 1;
            if(treadmillData->speed > MAX_RUN_SPEED)
            {
                treadmillData->speed = MAX_RUN_SPEED;
                displaySpeed = MAX_RUN_SPEED;
            }
            else
            {
                IsDataChanged = YES;
                SCREEN_Tone();
            }
            stateReturn = RUN;
            break;
        case MINUS_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            displaySpeed -= 1;
            if(displaySpeed % 2 == 0)
                treadmillData->speed -= 1;
            if(treadmillData->speed < DEFAULT_RUN_SPEED)
            {
                treadmillData->speed = DEFAULT_RUN_SPEED;
                displaySpeed = DEFAULT_RUN_SPEED;
            }
            else
            {
                IsDataChanged = YES;
                SCREEN_Tone();
            }
            stateReturn = RUN;
            break;
        case UP_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            treadmillData->incline += 1;
            if(treadmillData->incline > 12)
                treadmillData->incline = 12;
            else
            {
                IsDataChanged = YES;
                SCREEN_Tone();
            }
            stateReturn = RUN;
            break;
        case DOWN_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            treadmillData->incline -= 1;
            if(treadmillData->incline > 12 )
                treadmillData->incline = 0;
            else
            {
                IsDataChanged = YES;
                SCREEN_Tone();
            }
            stateReturn = RUN;
        default:
            stateReturn = RUN;
            break;
    }

    /*-------------------FOR EXECISE MODE-------------------*/
    /* change speed and data if run in exercise mode */
    if(ExeRunFlag == FLAG_ON)
    {
        if(Sec == changeMoment)
        {
            changeMoment = Sec*14/15;
            treadmillData->speed   += (Execrise[treadmillData->runEx - 1][numberOfChange] >> 4) & 0x0F;
            treadmillData->incline += (Execrise[treadmillData->runEx - 1][numberOfChange]) & 0x0F;
            if( ((uint32_t)(treadmillData->speed)) >= 150)
                treadmillData->speed = 150;
            if(treadmillData->incline > 12)
                treadmillData->incline = 12;
            numberOfChange ++;
            IsDataChanged = YES;
        }
    }
    /*--------------------FOR SETUP MODE----------------------*/

    /* cacutlate time to change calo and distance and change data*/
    if(SYSTICK_GetTick() > startTickForChangeDistance)
        if((SYSTICK_GetTick() - startTickForChangeDistance) > (360000/treadmillData->speed))
        {
            if(HowToCountData == COUNT_DOWN_DIS)
                treadmillData->distance -= 1;
            else
                treadmillData->distance += 1;
            startTickForChangeDistance = SYSTICK_GetTick();
            SCREEN_UpdateDistance(treadmillData->distance);
        }
    else
        if((startTickForChangeDistance - SYSTICK_GetTick()) < (360000/treadmillData->speed))
        {
            if(HowToCountData == COUNT_DOWN_DIS)
                treadmillData->distance -= 1;
            else
                treadmillData->distance += 1;
            startTickForChangeDistance = 0;
            startTickForChangeDistance = SYSTICK_GetTick();
            SCREEN_UpdateDistance(treadmillData->distance);
        }
    if(SYSTICK_GetTick() > startTickForChangeCalo)
        //must replace by the calorie change time formula
        if((SYSTICK_GetTick() - startTickForChangeCalo) > (360000/treadmillData->speed))
        {
            if(HowToCountData == COUNT_DOWN_CALO)
                treadmillData->calo -= 1;
            else
                treadmillData->calo += 1;
            startTickForChangeCalo = SYSTICK_GetTick();
            SCREEN_UpdateCalo(treadmillData->calo);
        }
    else
        //must replace by the calorie change time formula
        if((SYSTICK_GetTick() - startTickForChangeCalo) > (360000/treadmillData->speed))
        {
            if(HowToCountData == COUNT_DOWN_CALO)
                treadmillData->calo -= 1;
            else
                treadmillData->calo += 1;
            startTickForChangeCalo = SYSTICK_GetTick();
            SCREEN_UpdateCalo(treadmillData->calo);
        }

    /* Stop if time or distance or calories expire */
    if(HowToCountData != COUNT_UP_ALL)
    {
        if((!treadmillData->distance) || (!treadmillData->calo) || (!treadmillData->runTime))
        {
            /* stop mode */
            IsDataChanged           = YES;
            IsThisTheFirstTimeRun   = YES;
            HowToCountData          = COUNT_UP_ALL;
            ExeRunFlag              = FLAG_OFF;
            /* stop timer */
            TIMER3_STOP();
            stateReturn = STOP;
        }
    }
    *laststate = RUN;
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
