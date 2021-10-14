#include "keypad.h"
#include "systick.h"
#include "screen.h"
#include "setup_mode.h"
/*******************************************************************************
 * Variables
 ******************************************************************************/
/* mode setup  */
uint8_t ModeState;
/* count for blink */
uint32_t CountForBlink;
/*******************************************************************************
 * Definition
 ******************************************************************************/
#define MAX_MODE    (2)
#define BLINK_FREQ  (50000)
/*******************************************************************************
 * Private func
 ******************************************************************************/
/*!
 * @brief Increase the value according to the mode
 *        runtime, distance, calo <=> mode: 0 1 2
 *
 * @param treadmillData
 * @return None 
 */
static void increase_val(run_mechine_data_t *treadmillData);


/*!
 * @brief Decrease the value according to the mode
 *        runtime, distance, calo <=> mode: 0 1 2
 * @param 
 * @param 
 */
static void decrease_val(run_mechine_data_t *treadmillData);

/*!
 * @brief change data to default
 *
 * @param 
 * @param 
*/
void reset_data_to_default(run_mechine_data_t *treadmillData, uint8_t ModeState);
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief The setup mode
 *
 * @param treadmillData
 * @param laststate 
 * @return State of program
*/
program_state_t setup_mode(run_mechine_data_t *treadmillData, program_state_t *laststate)
{
    program_state_t stateReturn;
    char key = NO_KEY;
    /* fist time run */
    if(IsThisTheFirstTimeRun == YES)
    {
        while(KEYPAD_ScanKey()==SETUP_KEY);
        IsThisTheFirstTimeRun = NO;
        treadmillData->runTime = 900;/* default value 15 min */
    }
    /* Show screen */
    if(IsDataChanged == YES)
    {
        SCREEN_UpdateTime(treadmillData->runTime);
        SCREEN_UpdateDistance(treadmillData->distance);
        SCREEN_UpdateCalo(treadmillData->calo);
        IsDataChanged = NO;
    }
    /* use a variable Count blink led */
    CountForBlink++;
    if(CountForBlink == BLINK_FREQ)
    {
        if (ModeState==0)
        {
            SCREEN_ClearSection(0,1);
            SCREEN_ClearSection(8,3);
        }
        else if (ModeState == 1)
            SCREEN_ClearSection(14,3);
        else if(ModeState == 2)
            SCREEN_ClearSection(20,3);
    }
    else if(CountForBlink == 2*BLINK_FREQ)
    {
        IsDataChanged = YES;
        CountForBlink=0;
    }

    /* Scan key */
    key = KEYPAD_ScanKey();
    switch(key)
    {
        case SETUP_KEY:
            SCREEN_Tone();
            DF_Play(ModeState + 8);
            while(KEYPAD_ScanKey()==SETUP_KEY);
            ModeState += 1;
            /* clear data */
            reset_data_to_default(treadmillData, ModeState);
            IsDataChanged = YES;
            stateReturn = SET_UP;
            break;
        case PLUS_KEY:
            SCREEN_Tone();
            increase_val(treadmillData);
            IsDataChanged = YES;
            stateReturn = SET_UP;
            break;
        case MINUS_KEY:
            SCREEN_Tone();
            decrease_val(treadmillData);
            IsDataChanged = YES;
            stateReturn = SET_UP;
            break;
        case UP_KEY:
            SCREEN_Tone();
            increase_val(treadmillData);
            IsDataChanged = YES;
            stateReturn = SET_UP;
            break;
        case DOWN_KEY:
            SCREEN_Tone();
            decrease_val(treadmillData);
            IsDataChanged = YES;
            stateReturn = SET_UP;
            break;
        case RUN_KEY:
            SCREEN_Tone();
            IsDataChanged = YES;
            IsThisTheFirstTimeRun = YES;
            stateReturn = RUN;
            break;
        case STOP_KEY:
            SCREEN_Tone();
            IsDataChanged = YES;
            IsThisTheFirstTimeRun = YES;
            stateReturn = START;
            break;
        default:
            stateReturn = SET_UP;
    }
    if(ModeState > MAX_MODE)
    {
        ModeState = 0;
        IsDataChanged = YES;
        IsThisTheFirstTimeRun = YES;
        stateReturn = START;
        SYSTICK_Delay_ms(30);
    }
    
    *laststate = SET_UP;
    return (stateReturn);
}


static void increase_val(run_mechine_data_t *treadmillData)
{
    if(ModeState == 0)
    {
        treadmillData->runTime += 60;
        if(treadmillData->runTime > MAX_RUN_TIME)
            treadmillData->runTime = 60;
    }
    else if(ModeState == 1)
    {
        treadmillData->distance += 10;
        if(treadmillData->distance > MAX_DISTANCE)
            treadmillData->distance = 10;
    }
    else if(ModeState == 2)
    {
        treadmillData->calo += 10;
        if(treadmillData->calo > MAX_CALO)
            treadmillData->calo = 10;
    }
}

static void decrease_val(run_mechine_data_t *treadmillData)
{
    if(ModeState == 0)
    {
        if(treadmillData->runTime > 300)
            treadmillData->runTime -= 60;
        else
            treadmillData->runTime = MAX_RUN_TIME;
    }
    else if(ModeState == 1)
    {
        treadmillData->distance -= 10;
        if(treadmillData->distance < 1)
            treadmillData->distance = MAX_DISTANCE;
    }
    else if(ModeState == 2)
    {
        
        treadmillData->calo -= 10;
        if(treadmillData->calo < 10)
            treadmillData->calo = MAX_CALO;
    }
}

void reset_data_to_default(run_mechine_data_t *treadmillData, uint8_t ModeState)
{
    switch (ModeState)
    {
        case 1:
            treadmillData->runTime = 0;
            treadmillData->distance = 100;
            break;
        case 2:
            treadmillData->distance = 0;
            treadmillData->calo = 500;
            break;
        default:
            break;
    }
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
