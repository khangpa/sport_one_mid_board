#include "stm32f10x.h"
#include "stdint.h"
#include "stm32f10x_sdio.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_gpio.h"
#include "systick.h"
#include "board.h"
#include "keypad.h"
/*******************************************************************************
 * Definition
 ******************************************************************************/
#define NOT_PRESS   (0)
#define PRESS       (1)
#define NO_ROW      (9)

/* Read write macro */
#define KEYPAD_READ_ROW(x)          (GPIO_ReadInputDataBit(KEYPAD_GPIO_ROW, x))
#define KEYPAD_SET_COL(x)           (GPIO_SetBits(KEYPAD_GPIO_COL,x))
#define KEYPAD_CLR_COL(x)           (GPIO_ResetBits(KEYPAD_GPIO_COL,x))
/*******************************************************************************
 * Variable
 ******************************************************************************/
keypad_state key_laststate = RELEAS_STATE;
const char Keypad_Button_Values[4][5] =  {  
                                            {'1', '2', '3', 'X', 'Y'},
                                            {'4', '5', '6', '7', 'Z'},
                                            {'8', '9', 'A', 'B', 'C'},
                                            {'W', 'T', 'D', 'E', 'F'}
                                         };
/*******************************************************************************
 * Private func
 ******************************************************************************/
/*!
 * @brief Set high all col
 * @param none
 * @retval none
 */
static void KEYPAD_SetHighAllCol()
{
    KEYPAD_SET_COL(KEYPAD_COL_1);
    KEYPAD_SET_COL(KEYPAD_COL_2);
    KEYPAD_SET_COL(KEYPAD_COL_3);
    KEYPAD_SET_COL(KEYPAD_COL_4);
    KEYPAD_SET_COL(KEYPAD_COL_5);
}

/*!
 * @brief Set low all col
 *
 * @param none
 */
static void KEYPAD_SetLowAllCol()
{
    KEYPAD_CLR_COL(KEYPAD_COL_1);
    KEYPAD_CLR_COL(KEYPAD_COL_2);
    KEYPAD_CLR_COL(KEYPAD_COL_3);
    KEYPAD_CLR_COL(KEYPAD_COL_4);
    KEYPAD_CLR_COL(KEYPAD_COL_5);
}
/*!
 * @brief Set 1 high low and another low
 *
 * @param col_index     index of col
 * @return  none 
 */
static void KEYPAD_SetColX(col_index index)
{
    KEYPAD_SetLowAllCol();
    switch (index)
    {
    case (Col_1):
        KEYPAD_SET_COL(KEYPAD_COL_1);
        break;
    case Col_2:
        KEYPAD_SET_COL(KEYPAD_COL_2);
        break;
    case Col_3:
        KEYPAD_SET_COL(KEYPAD_COL_3);
        break;
    case Col_4:
        KEYPAD_SET_COL(KEYPAD_COL_4);
        break;
    case Col_5:
        KEYPAD_SET_COL(KEYPAD_COL_5);
    default:
        break;
    }
}

/*!
 * @brief 
 *
 * @return return PRESS: key pressed
 *         return NOT_PRESS: key not press
 */
static uint32_t KEYPAD_IsKeyPressed()
{
    uint8_t keyStatus = NOT_PRESS;
    KEYPAD_SetHighAllCol();
    if( KEYPAD_READ_ROW(KEYPAD_ROW_1)||
        KEYPAD_READ_ROW(KEYPAD_ROW_2)||
        KEYPAD_READ_ROW(KEYPAD_ROW_3)||
        KEYPAD_READ_ROW(KEYPAD_ROW_4) )
        keyStatus = PRESS;
    return keyStatus;
}

/*!
 * @brief 
 *
 * @param 
 * @param 
 */
static uint8_t KEYPAD_RowCheck()
{
    uint32_t retVal;
    if(KEYPAD_READ_ROW(KEYPAD_ROW_1))
        retVal = Row_1;
    else if(KEYPAD_READ_ROW(KEYPAD_ROW_2))
        retVal = Row_2;
    else if(KEYPAD_READ_ROW(KEYPAD_ROW_3))
        retVal = Row_3;
    else if(KEYPAD_READ_ROW(KEYPAD_ROW_4))
        retVal = Row_4;
    else
        retVal = NO_ROW;
    return (retVal);
}

/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Init GPIO pin for keyPad
 *
 * @param None
 */
void KEYPAD_Init()
{
    GPIO_InitTypeDef GPIO_InitStruce;
    /* GPIOA, GPIOB Periph clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
    /* Col init */
    GPIO_InitStruce.GPIO_Pin = KEYPAD_COL_1;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEYPAD_GPIO_COL,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_COL_2;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEYPAD_GPIO_COL,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_COL_3;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEYPAD_GPIO_COL,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_COL_4;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEYPAD_GPIO_COL,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_COL_5;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(KEYPAD_GPIO_COL,&GPIO_InitStruce);

    /* ROW Init*/
    GPIO_InitStruce.GPIO_Pin = KEYPAD_ROW_1;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEYPAD_GPIO_ROW,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_ROW_2;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEYPAD_GPIO_ROW,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_ROW_3;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEYPAD_GPIO_ROW,&GPIO_InitStruce);

    GPIO_InitStruce.GPIO_Pin = KEYPAD_ROW_4;
    GPIO_InitStruce.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStruce.GPIO_Mode = GPIO_Mode_IPD;
    GPIO_Init(KEYPAD_GPIO_ROW,&GPIO_InitStruce);
}

/*!
 * @brief Scan keypad
 *
 * @param None
 * @return 
 */
char KEYPAD_ScanKey()
{
    char key = NO_KEY;
    uint32_t rowVal;
    if(KEYPAD_IsKeyPressed())
    {
        /* Debounce time */
        SYSTICK_Delay_ms(20);
        if(KEYPAD_IsKeyPressed())
        {
            KEYPAD_SetColX(Col_1);
            rowVal = KEYPAD_RowCheck();
            if(rowVal != NO_ROW)
                key = Keypad_Button_Values[rowVal][Col_1];
            else
            {
                KEYPAD_SetColX(Col_2);
                rowVal = KEYPAD_RowCheck();
                if(rowVal != NO_ROW)
                    key = Keypad_Button_Values[rowVal][Col_2];
                else
                {
                    KEYPAD_SetColX(Col_3);
                    rowVal = KEYPAD_RowCheck();
                    if(rowVal != NO_ROW)
                        key = Keypad_Button_Values[rowVal][Col_3];
                    else
                    {
                        KEYPAD_SetColX(Col_4);
                        rowVal = KEYPAD_RowCheck();
                        if(rowVal != NO_ROW)
                            key = Keypad_Button_Values[rowVal][Col_4];
                        else
                        {
                            KEYPAD_SetHighAllCol();
                            rowVal = KEYPAD_RowCheck();
                            if(rowVal != NO_ROW)
                                key = Keypad_Button_Values[rowVal][Col_5];
                        }
                    }
                }
            }
        }
    }
    return (key);
}

keypad_info_t KEYPAD_ScanWithCheckHold(uint32_t holdKeytimeOut)
{
    keypad_info_t keyInfo;
    char keyTemp;
    uint32_t t0 = SYSTICK_GetTick();
    uint32_t t1 = SYSTICK_GetTick();
    keyInfo.keyName = KEYPAD_ScanKey();
    switch (key_laststate)
    {
        case HOLD_STATE:
            /* code */
            keyInfo.keyName = KEYPAD_ScanKey();
            if(keyInfo.keyName != NO_KEY)
                keyInfo.keypad_state = HOLD_STATE;
            else
                keyInfo.keypad_state = RELEAS_STATE;
            break;
        
        default:
            if(keyInfo.keyName != NO_KEY)
            {
                keyTemp = keyInfo.keyName;
                while(t1 - t0 < holdKeytimeOut)
                {
                    keyInfo.keyName = KEYPAD_ScanKey();
                    t1 = SYSTICK_GetTick();
                    if(keyInfo.keyName == NO_KEY)
                        break;
                }
                if(t1 - t0 < holdKeytimeOut)
                    keyInfo.keypad_state = PRESS_STATE;
                else
                    keyInfo.keypad_state = HOLD_STATE;
                keyInfo.keyName = keyTemp;
            }
            else
                keyInfo.keypad_state = RELEAS_STATE;
            break;
    }
    key_laststate = keyInfo.keypad_state;
    return (keyInfo);
}
/*******************************************************************************
 * EOF
 ******************************************************************************/
