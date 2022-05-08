#include "systick.h"
/*******************************************************************************
 * Variable
 ******************************************************************************/
static volatile uint32_t tick;

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief init systick
 *
 * @param 
 * @param 
 */
void SYSTICK_Init(void)
{
    SysTick_Config(SystemCoreClock/1000);
}

void SysTick_Handler(void)
{
    tick++;
}
/* FUNCTION ********************************************************************
 *
 * Function Name : SYSTICK_GetTick
 * Description   : Tick get function
 *
 * END ************************************************************************/
uint32_t SYSTICK_GetTick(void)
{
    return tick;
}
/* FUNCTION ********************************************************************
 *
 * Function Name : SYSTICK_Delay_ms
 * Description   : Delay function
 *
 * END ************************************************************************/
void SYSTICK_Delay_ms(uint32_t time_ms)
{
    static uint32_t t0, t1;
    t0 = SYSTICK_GetTick();
    t1 = SYSTICK_GetTick();
    if(t0 < (MAXTICK - time_ms))
    {
        while(t1 - t0 < time_ms)
        {
            t1 = SYSTICK_GetTick();
        }
    }
    else
    {
        if(t1 > time_ms)
        {
            while(t1 > time_ms)
            {
                t1 = SYSTICK_GetTick();
            }
            while(MAXTICK - t0 + t1 < time_ms)
            {
                t1 = SYSTICK_GetTick();
            }
        }
        else
        {
            while(MAXTICK - t0 + t1 < time_ms)
            {
                t1 = SYSTICK_GetTick();
            }
        }
    }
    return;
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
