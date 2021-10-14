#include "keypad.h"
#include "common.h"
#include "dfplayer.h"
#include "screen.h"
#include "systick.h"
#include "start_mode.h"
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief The start mode
 *
 * @param treadmillData
 * @param laststate 
 * @return State of program
*/
program_state_t start_mode(run_mechine_data_t *treadmillData, program_state_t *laststate)
{
    uint32_t t0 = 0;
    uint32_t t1 = 0;
    uint8_t turnOffFlag = 0;
    static program_state_t stateReturn;
    char key = NO_KEY;
    /* start display */
    if(IsDataChanged == YES)
    {
        /* update screen */
        reset_run_treadmillData(treadmillData);
        /* update screen */
        SCREEN_UpdateSpeed(treadmillData->speed);            /* Speed */
        SCREEN_UpdateCalo(treadmillData->calo);              /* Calo */
        SCREEN_UpdateDistance(treadmillData->distance);      /* Distance */
        SCREEN_UpdateIncline(treadmillData->incline);        /* Incline */
        SCREEN_UpdateTime(treadmillData->runTime);           /* Run time */
        SYSTICK_Delay_ms(1000);
        IsDataChanged = NO;
    }
    SYSTICK_Delay_ms(20);
    key = KEYPAD_ScanKey();
    if(key == EXE_KEY)
    {
        SCREEN_Tone();
        IsDataChanged = YES;
        stateReturn = EXERCISE_SET;
    }
    else if(key == RUN_KEY)
    {
        SCREEN_Tone();
        IsDataChanged = YES;
        stateReturn = RUN;
    }
    else if(key == SETUP_KEY)
    {
        SCREEN_Tone();
        IsDataChanged = YES;
        stateReturn = SET_UP;
    
    }
    else if(key == STOP_KEY)
    {
        t0 = SYSTICK_GetTick();
        t1 = SYSTICK_GetTick();
        while((t1 - t0) < 30000)
        {
            t1 = SYSTICK_GetTick();
            key = KEYPAD_ScanKey();
            if(key != STOP_KEY)
            {
                turnOffFlag = 1;
                break;
            }            
        }
        if(turnOffFlag == 0)
            DF_Pause();
        stateReturn = START;
    }
    else
        stateReturn = START;
    
    *laststate = START;
    return (stateReturn);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
