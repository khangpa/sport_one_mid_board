#include "stm32f10x.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"
#include "timer3.h"
TIMER3_CallbackHandle TIMER3_CallbackFunc;
/*******************************************************************************
* Code
*******************************************************************************/

void TIMER3_Init()
{
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);
    TIM_TimeBaseInitTypeDef timerBaseInit;
    timerBaseInit.TIM_CounterMode = TIM_CounterMode_Down;
    timerBaseInit.TIM_ClockDivision = TIM_CKD_DIV4;
    timerBaseInit.TIM_Prescaler = SystemCoreClock/10000 - 1;   /* Ftimer = 10KHz */
    timerBaseInit.TIM_Period = 10000 - 1;                      /* 1s */
    TIM_TimeBaseInit(TIM3, &timerBaseInit);
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
}

void TIMER3_CallbackInit(TIMER3_CallbackHandle CallbackFunc)
{
    TIMER3_CallbackFunc = CallbackFunc;
}

void TIMER3_Start()
{
    TIM_Cmd(TIM3,ENABLE);
    NVIC_EnableIRQ(TIM3_IRQn);
}

void TIMER3_STOP()
{
    TIM_Cmd(TIM3,DISABLE);
    NVIC_DisableIRQ(TIM3_IRQn);
}

void TIM3_IRQHandler()
{
    TIM_ClearFlag(TIM3,TIM_FLAG_Update);
    TIMER3_CallbackFunc();
}