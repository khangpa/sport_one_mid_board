#ifndef __BOARD__
#define __BOARD__

/* ----------keypad---------- */
/* keypad gpio  */
#define KEYPAD_GPIO_COL       GPIOB
#define KEYPAD_GPIO_ROW       GPIOA
/* keypad pin */
#define KEYPAD_COL_1      GPIO_Pin_11
#define KEYPAD_COL_2      GPIO_Pin_10
#define KEYPAD_COL_3      GPIO_Pin_2
#define KEYPAD_COL_4      GPIO_Pin_1
#define KEYPAD_COL_5      GPIO_Pin_0
#define KEYPAD_ROW_1      GPIO_Pin_4
#define KEYPAD_ROW_2      GPIO_Pin_5
#define KEYPAD_ROW_3      GPIO_Pin_6
#define KEYPAD_ROW_4      GPIO_Pin_7
/* Power_communicate */
#define POWER_COM_UART              USART1
#define POWER_COM_IRQn              USART1_IRQn
#define POWER_COM_GPIO              GPIOA
#define POWER_COM_RxPIN             GPIO_Pin_10
#define POWER_COM_TxPIN             GPIO_Pin_9
#define POWER_COM_GPIO_CLK          RCC_APB2Periph_GPIOA
#define POWER_COM_CLK               RCC_APB2Periph_USART1

/* Safe key  */
#define SAFE_KEY_GPIO               GPIOB
#define SAFE_KEY_PIN_OUT            GPIO_Pin_11
#define SAFE_KEY_PIN_IN             GPIO_Pin_10
#define SAFE_KEY_TIMER              TIM2
#define SAFE_KEY_TIMER_CLK          RCC_APB1Periph_TIM2
#define SAFE_KEY_GPIO_CLK           RCC_APB2Periph_GPIOB

/* DF */

#define DF_UART                     USART2
#define DF_UART_Baudrate            9600
#define DF_IRQn                     USART2_IRQn
#define DF_GPIO                     GPIOA
#define DF_RxPIN                    GPIO_Pin_3
#define DF_TxPIN                    GPIO_Pin_2
#define DF_GPIO_CLK                 RCC_APB2Periph_GPIOA
#define DF_CLK                      RCC_APB1Periph_USART2

/* Normal Clock */
#define CLOCK_TIM            TIM3
#endif /* __BOARD__ */
/*******************************************************************************
* EOF
*******************************************************************************/
