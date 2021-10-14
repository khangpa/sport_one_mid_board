#ifndef __COMMON_H__
#define __COMMON_H__
#include "stdint.h"
/*******************************************************************************
 * Definition
 ******************************************************************************/
/* program states */
typedef enum
{
    START,
    RUN,
    EXERCISE_SET,
    SET_UP,
    STOP,
    FAULT,
    IDLE
} program_state_t;

/* program data for display and executes*/
typedef struct
{
    uint32_t speed;
    uint32_t distance;
    uint32_t calo;
    uint32_t runTime;
    uint8_t runEx;
    uint8_t incline;
} run_mechine_data_t;

/* define default value and maximum value of data */
#define MAX_SPEED           (14)
#define MAX_RUN_TIME        (5940)
#define MAX_INCLINE         (15)
#define MAX_CALO            (9999)
#define MAX_DISTANCE        (9999)
#define MAX_RUN_EX          (12)

#define MIN_SPEED           (15)
#define MIN_RUN_TIME        (300)
#define MIN_INCLINE         (9)
#define MIN_CALO            (300)
#define MIN_DISTANCE        (200)
#define MIN_RUN_EX          (12)

#define DEFAULT_SPEED       (0)
#define DEFAULT_RUN_TIME    (0)
#define DEFAULT_INCLINE     (0)
#define DEFAULT_CALO        (0)
#define DEFAULT_DISTANCE    (0)
#define DEFAULT_RUN_EX      (1)

#define YES     0
#define NO      1
/*******************************************************************************
 * Variable
 ******************************************************************************/
extern volatile uint8_t IsDataChanged;
extern uint32_t IsThisTheFirstTimeRun;
/*******************************************************************************
 * API
 ******************************************************************************/
void reset_run_treadmillData(run_mechine_data_t *treadmillData);
#endif /* common.h */