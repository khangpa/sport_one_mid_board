#include "stop_mode.h"
#include "screen.h"
#include "xor.h"
#include"power_communicate.h"
#include "systick.h"
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief The stop mode
 *
 * @param treadmillData
 * @param laststate         program_state_t 
 *                          {
 *                              START,
                                RUN,
                                EXERCISE_SET,
                                SET_UP,
                                STOP,
                                IDLE
                            }
 * @return New state of program
*/
program_state_t stop_mode(run_mechine_data_t *treadmillData, program_state_t *laststate)
{
    /* send stop command */
    power_com_cmd_t cmdSend;
    cmdSend.command = STOP_RUN;
    cmdSend.length  = 0;
    cmdSend.sequence = 0;
    cmdSend.type    = MASTER_REQUEST_TYPE;
    cmdSend.buff[0] = XOR_Calculator((uint8_t*)&cmdSend, 0, POWER_COM_CMD_HEADER_SIZE);
    POWER_COM_SendCmd(&cmdSend,cmdSend.length + 5);
    while(treadmillData->speed)
    {
        /* get speed value */
        treadmillData->speed -= 1;
        SCREEN_UpdateSpeed(treadmillData->speed);
    }
    reset_run_treadmillData(treadmillData);
    *laststate = STOP;
    return (START);
}

/*******************************************************************************
* EOF
*******************************************************************************/
