#ifndef __KEY_PAD_H__
#define __KEY_PAD_H__
#include "stdint.h"
/*******************************************************************************
 * Definition
 ******************************************************************************/
/* define key name */
#define INCLINE_3_KEY       '3'
#define INCLINE_6_KEY       '2'
#define INCLINE_9_KEY       '1'
#define PLUS_KEY            '4'
#define MINUS_KEY           '5'
#define EXE_KEY             '6'
#define RUN_KEY             '7'
#define STOP_KEY            '8'
#define SETUP_KEY           '9'
#define UP_KEY              'A'
#define DOWN_KEY            'B'
#define SPEED_3_KEY         'C'
#define SPEED_6_KEY         'D'
#define SPEED_9_KEY         'E'
#define NO_KEY            (0xff)

/* key state */
typedef enum
{
    pressState,
    holdState,
    releasState
} keypad_state;
/* keypad return */
typedef struct keypad_info
{
    keypad_state keypad_state;
    char keyName;
} keypad_info_t;


/* col and row index enum */
typedef enum
{
    Col_1 = 0,
    Col_2,
    Col_3,
    Col_4
} col_index;

typedef enum
{
    Row_1 = 0,
    Row_2,
    Row_3,
    Row_4
} row_index;


/*******************************************************************************
 * API
 ******************************************************************************/
void KEYPAD_Init();
char KEYPAD_ScanKey();
keypad_info_t KEYPAD_ScanWithCheckHold(uint32_t holdKeytimeOut);
#endif  /* KEY_PAD */