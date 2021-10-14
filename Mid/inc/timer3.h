#ifndef __TIMER3_H__
#define __TIMER3_H__
/*******************************************************************************
* Definition
*******************************************************************************/
typedef void (*TIMER3_CallbackHandle)(void);

void TIMER3_CallbackInit(TIMER3_CallbackHandle TIMER3_CallbackFunc);
void TIMER3_Init();
void TIMER3_STOP();
void TIMER3_Start();
#endif  /* __TIMER3_H__ */
/*******************************************************************************
* EOF
*******************************************************************************/
