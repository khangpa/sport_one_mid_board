/*
 * Copyright (c) May 2021
 * VitechSolutions
 * All Rights Reserved
 *
 *
 * Description:
 *
 * Author: KhangPA
 */

/*******************************************************************************
* Include
*******************************************************************************/
#include "stm32f10x.h"
#include "stdio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "board.h"
#include "screen.h"
#include "stm32f10x_exti.h"
#include "safety_checking.h"
/*******************************************************************************
* Definitions
*******************************************************************************/

/*******************************************************************************
* Private Func
*******************************************************************************/

/*******************************************************************************
* Variable
*******************************************************************************/
extern program_state_t laststate;
extern program_state_t state;
/*******************************************************************************
* Code
*******************************************************************************/
void SAFETY_CHECK_Init()
{
    GPIO_InitTypeDef GPIO_InitStruce;
    EXTI_InitTypeDef EXTI_InitStruce;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    /* GPIO safe key in pin */
    GPIO_InitStruce.GPIO_Mode  = GPIO_Mode_IPD;
    GPIO_InitStruce.GPIO_Pin   = SAFE_KEY_PIN;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(SAFE_KEY_GPIO,&GPIO_InitStruce);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource10);
    /* EXTI line */
    EXTI_InitStruce.EXTI_Line = EXTI_Line4;
    EXTI_InitStruce.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStruce.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStruce.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStruce);
    NVIC_EnableIRQ(EXTI15_10_IRQn);
}

void EXTI4_IRQHandler()
{
    EXTI_ClearFlag(EXTI_Line4);
    if(!GPIO_ReadInputDataBit(SAFE_KEY_GPIO, SAFE_KEY_PIN))
    {
        laststate = state;
        state = FAULT;
    }
}
