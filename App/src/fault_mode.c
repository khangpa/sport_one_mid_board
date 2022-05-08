#include "fault_mode.h"
#include "systick.h"
#include "screen.h"
#include "stm32f10x_gpio.h"
#include "safety_checking.h"
#include "power_communicate.h"
#include "xor.h"
#include "board.h"
program_state_t fault_mode(run_mechine_data_t *treadmillData, program_state_t *laststate)
{
    /* send stop command */
    power_com_cmd_t cmdSend;
    cmdSend.command = STOP_RUN;
    cmdSend.length  = 0;
    cmdSend.sequence = 0;
    cmdSend.type    = MASTER_REQUEST_TYPE;
    cmdSend.buff[0] = XOR_Calculator((uint8_t*)&cmdSend, 0, POWER_COM_CMD_HEADER_SIZE);
    POWER_COM_SendCmd(&cmdSend,cmdSend.length + 5);
    reset_run_treadmillData(treadmillData);
    //error screen
    SCREEN_ERROR(1);
    // wait for input safe key
    while(!GPIO_ReadInputDataBit(SAFE_KEY_GPIO, SAFE_KEY_PIN));
    SCREEN_ERROR(0);
    *laststate = FAULT;
    return (START);
}