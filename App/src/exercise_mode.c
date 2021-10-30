#include "keypad.h"
#include "screen.h"
#include "systick.h"
#include "dfplayer.h"
#include "exercise_mode.h"

/*******************************************************************************
 * Variable
 ******************************************************************************/
static uint8_t  ResetVal;
static uint32_t CountForBlink;

/*******************************************************************************
* Definition
*******************************************************************************/

#define BLINK_FREQ      (50000U)

/*******************************************************************************
* Private func
*******************************************************************************/
/*!
 * @brief function blink time led
 *
 * @param None
 * @return None
*/
static void blinkTimeLed();

/*!
 * @brief function reset all exercise info: TIME, EXE
 *
 * @param treadmillData  
*/
static void reset_ex_value(run_mechine_data_t *treadmillData);
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief The exercise mode
 *
 * @param treadmillData
 * @param laststate 
 * @return State of program
*/
program_state_t exercise_mode(run_mechine_data_t *treadmillData, program_state_t *laststate)
{
    static program_state_t stateReturn;
    keypad_info_t key;
    /* reset time, incline for new exercise*/
    if(ResetVal == YES)
    {
        reset_ex_value(treadmillData);
        ResetVal = NO;
    }

    /* update screen */
    if(IsDataChanged == YES)
    {
        SCREEN_UpdateEx(treadmillData->runEx);               /* exercise */
        SYSTICK_Delay_ms(50);
        SCREEN_UpdateTime(treadmillData->runTime);           /* Run time */
        IsDataChanged = NO;
    }

    /* check first time run */
    if(IsThisTheFirstTimeRun == YES)
    {
        while(KEYPAD_ScanKey()==EXE_KEY);
        IsThisTheFirstTimeRun = NO;
    }

    blinkTimeLed();
    
    /* Scan key */
    key = KEYPAD_ScanWithCheckHold(500);
    /* Executive */
    switch (key.keyName)
    {
        case EXE_KEY:
            if(treadmillData->runEx >11 || treadmillData->runEx < 3)
                DF_Play(treadmillData->runEx + 2);
            while(
                  KEYPAD_ScanKey()==EXE_KEY);
            treadmillData->runEx += 1;
            if(treadmillData->runEx > MAX_RUN_EX)
                treadmillData->runEx = DEFAULT_RUN_EX;
            SCREEN_Tone();
            ResetVal = YES;
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case PLUS_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            treadmillData->runTime += 60;
            if(treadmillData->runTime > MAX_RUN_TIME)
                treadmillData->runTime = MIN_RUN_TIME;
            SCREEN_Tone();
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case MINUS_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            treadmillData->runTime -= 60;
            if(treadmillData->runTime < MIN_RUN_TIME)
                treadmillData->runTime = MAX_RUN_TIME;
            SCREEN_Tone();
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case UP_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            treadmillData->runTime += 60;
            if(treadmillData->runTime > MAX_RUN_TIME)
                treadmillData->runTime = MIN_RUN_TIME;
            SCREEN_Tone();
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case DOWN_KEY:
            if(key.keypad_state == HOLD_STATE)
                SYSTICK_Delay_ms(DELAY_IF_HOLD_STATE);
            treadmillData->runTime -= 60;
            if(treadmillData->runTime < MIN_RUN_TIME)
                treadmillData->runTime = MAX_RUN_TIME;
            SCREEN_Tone();
            IsDataChanged = YES;
            stateReturn = EXERCISE_SET;
            break;
        case STOP_KEY:
            SCREEN_Tone();
            reset_run_treadmillData(treadmillData);
            IsDataChanged = YES;
            IsThisTheFirstTimeRun = YES;
            ResetVal      = YES;
            stateReturn = START;
            break;
        case RUN_KEY:
            IsDataChanged = YES;
            IsThisTheFirstTimeRun = YES;
            ResetVal      = YES;
            SCREEN_Tone();
            stateReturn = RUN;
            break;
        default:
            stateReturn = EXERCISE_SET;
            break;
    }
    *laststate = EXERCISE_SET;
    return (stateReturn);
}

static void blinkTimeLed()
{
    CountForBlink++;
    if(CountForBlink == BLINK_FREQ)
    {
        SCREEN_ClearSection(0,1);
        SCREEN_ClearSection(8,3);
    }
    else if(CountForBlink == 2*BLINK_FREQ)
    {
        IsDataChanged = YES;
        CountForBlink=0;
    }
}

static void reset_ex_value(run_mechine_data_t *treadmillData)
{
    /* time: 30 min */
    treadmillData->runTime = 1800;
    switch(treadmillData->runEx)
    {
        case 7:
            treadmillData->incline = 2;
            break;
        case 10:
            treadmillData->incline = 9;
            break;
        case 11:
            treadmillData->incline = 9;
            break;
        case 12:
            treadmillData->incline = 2;
            break;
        default:
            treadmillData->incline = 0;
            break;
    }
    SCREEN_UpdateIncline(treadmillData->incline);
    ResetVal = NO;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
