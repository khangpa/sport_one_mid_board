#include "softuart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_rcc.h"

static uint32_t tickcount = 0;

void Softuart_Handtick(void)
{
    tickcount++;
    if(tickcount == 0xFFFFFFFF)
      tickcount =0;
}

uint32_t Softuart_Gettick(void)
{
    return tickcount;
}

void Softuart_Init()
{
    /* GPIOA Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    /* GPIOA Config */
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);
    /* TIM2 Clock enabale */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    /* TIM2 Config */
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_TimeBaseStructure.TIM_Period = 2499;
    TIM_TimeBaseStructure.TIM_Prescaler = 1;
    TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    /* TIM IT enable */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    NVIC_EnableIRQ(TIM2_IRQn);
    TIM_Cmd(TIM2, ENABLE);
}

void Softuart_SendByte(uint8_t byte)
{
    //GPIO_ResetBits(GPIOB, GPIO_Pin_10);
    GPIO_SetBits(GPIOB, GPIO_Pin_10);
    uint8_t buff[8];
    for(uint8_t i = 0; i < 8; i++)
    {
        if(byte & 0x08 == 0x08)
        {
            buff[i] = 1;
        }else
        {
            buff[i] = 0;
        }
        byte >>= 1;
    }
    tickcount = 0;
    while(Softuart_Gettick() != 1);
    GPIO_ResetBits(GPIOB, GPIO_Pin_10);
    //GPIO_SetBits(GPIOB, GPIO_Pin_10);
    for(uint8_t i = 0; i < 8; i++)
    {
        while (Softuart_Gettick() == i+1);
        if(buff[i] == 1)
        {
            //GPIO_ResetBits(GPIOB, GPIO_Pin_10);
            GPIO_SetBits(GPIOB, GPIO_Pin_10);
        }else
        {
            GPIO_ResetBits(GPIOB, GPIO_Pin_10);
            //GPIO_SetBits(GPIOB, GPIO_Pin_10);
        }
    }
    while (Softuart_Gettick() == 9);
    //GPIO_ResetBits(GPIOB, GPIO_Pin_10);
    GPIO_SetBits(GPIOB, GPIO_Pin_10);
}

void Softuart_send_frame(uint8_t startbyte, uint8_t addressbyte,
                                    uint8_t databyte, uint8_t stopbyte)
{
    Softuart_SendByte(startbyte);
    Softuart_SendByte(addressbyte);
    Softuart_SendByte(databyte);
    Softuart_SendByte(stopbyte);
}

void Softuart_SendData(uint8_t* buff, uint8_t length)
{
    for(uint8_t i = 0; i < length; i++)
    {
        Softuart_SendByte(buff[i]);
    }
}

void TIM2_IRQHandler()
{
    TIM_ClearFlag(TIM2,TIM_FLAG_Update);
    Softuart_Handtick();
}