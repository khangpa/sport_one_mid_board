#include "common.h"
/*******************************************************************************
 * Variable
 ******************************************************************************/
volatile uint8_t IsDataChanged = YES;
uint32_t IsThisTheFirstTimeRun = YES;
/*******************************************************************************
 * Code
 ******************************************************************************/
void reset_run_treadmillData(run_mechine_data_t *treadmillData)
{
    treadmillData->calo       = 0;
    treadmillData->distance   = 0;
    treadmillData->incline    = 0;
    treadmillData->runEx      = DEFAULT_RUN_EX;
    treadmillData->runTime    = 0;
}