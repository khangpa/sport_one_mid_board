#include <stdint.h>
#include "systick.h"
#include "lcd.h"
#include "screen.h"


/*******************************************************************************
 * Private Function
 ******************************************************************************/

static void Screen_convertData(uint32_t dataIn, unsigned char* dataOut)
{
    int i;
    uint32_t ch[3];
    ch[0]=dataIn/100 ;
    ch[1]=(dataIn-ch[0]*100)/10;
    ch[2]=(dataIn-ch[0]*100-ch[1]*10);
    for(i=0;i<3;i++)
    {
        dataOut[i]=ch[i]+'0';
    }
}

static void Screen_convertData1(uint32_t dataIn, unsigned char* dataOut)
{
    int i;
    uint32_t ch[3];
    ch[0]=dataIn/1000 ;
    ch[1]=(dataIn-ch[0]*1000)/100;
    ch[2]=(dataIn-ch[0]*1000-ch[1]*100)/10;
    for(i=0;i<3;i++)
    {
        dataOut[i]=ch[i]+'0';
    }
}

static void timeScreenSecond(uint32_t second)
{
    unsigned char factorSecond[2]={'\0','\0'};
    factorSecond[0]=second%10+'0';
    factorSecond[1]=second/10+'0';
    lcd_send_data(&factorSecond[1],ADD_TIME_SECOND_1);
    lcd_send_data(&factorSecond[0],ADD_TIME_SECOND_0);
}


static void timeScreenMinute(uint32_t minute)
{
    unsigned char factorMinute[2]={'\0','\0'};
    factorMinute[0]=minute%10+'0';
    factorMinute[1]=minute/10+'0';
    lcd_send_data(&factorMinute[1],ADD_TIME_MINUTE_1);
    lcd_send_data(&factorMinute[0],ADD_TIME_MINUTE_0);
}

void clear_Dot(unsigned char *data,uint8_t address)
{
    unsigned char buffData[10] = {0xd7,0x06,0xe3,0xa7,0x36,0xb5,0xf5,0x07,0xf7,0xb7};
    unsigned char buffDigit[10]={'0','1','2','3','4','5','6','7','8','9'};
    for (int i = 0; i < 10; i++)
    {
        if(*data==buffDigit[i])
        {
            lcd_show_data1(&buffData[i],address);
        }
    }
}
/*
void clear_SET_Time(uint32_t time,uint8_t address)
{
    unsigned char factorSecond[2]={'\0','\0'};
    uint32_t minute=time/60;
    uint32_t second= time-minute*60;
    factorSecond[0]=second % 10+'0';
    factorSecond[1]=second / 10+'0';
    clear_Dot(&factorSecond[1],address);
}

static void display_Set(unsigned char *data,uint8_t address)
{
    unsigned char buffData[10] = {0xdf,0x0e,0xeb,0xaf,0x3e,0xbd,0xfd,0x0f,0xff,0xbf};
    unsigned char buffDigit[10]={'0','1','2','3','4','5','6','7','8','9'};
    unsigned char nullCh=0x08;
    for (int i = 0; i < 10; i++)
    {
        if(*data==buffDigit[i])
        {
            if (address==26)
            {
                lcd_show_data1(&nullCh,address);
            }
            else
            {
                lcd_show_data1(&buffData[i],address);
            }      
        }
    }
} */
/*******************************************************************************
* CODE
*******************************************************************************/

void SCREEN_Init()
{
    LCD_GpioInit();
    LCD_Init();
}

void SCREEN_Clear()
{
    lcd_clr();
}

void SCREEN_ClearSection(uint8_t address, uint8_t size)
{
    lcd_clr_section(address,size);
}

void SCREEN_UpdateTime(uint32_t time)
{
    uint32_t minute=time/60;
    uint32_t second= time-minute*60;
    timeScreenSecond(second);
    timeScreenMinute(minute);
}


void SCREEN_UpdateDistance(uint32_t dis)
{    
    unsigned char dataConvert[3];
    
    if ((uint32_t)dis < (uint32_t)1000)
    {
        Screen_convertData(dis,dataConvert);
        for(int i = 0; i < 3 ; i++)
        {
            lcd_send_data(&dataConvert[i],ADD_DIS_0+2*i);      
        }
    }
    else
    {
        Screen_convertData1(dis,dataConvert);
        for(int i = 0; i < 3 ; i++)
        {
            lcd_send_data1(&dataConvert[i],ADD_DIS_0+2*i);      
        }
    }     
}

void SCREEN_UpdateCalo(uint32_t cal)
{
    unsigned char dataConvert[3];
    
    if ((uint32_t)cal < (uint32_t)1000)
    {
        Screen_convertData(cal,dataConvert);
        for(int i = 0; i < 3 ; i++)
        {
            lcd_send_data(&dataConvert[i],ADD_CAL_0+2*i);      
        }
    }
    else
    {
        Screen_convertData1(cal,dataConvert);
        for(int i = 0; i < 3 ; i++)
        {
            lcd_send_data1(&dataConvert[i],ADD_CAL_0+2*i);      
        }
    }
}

void SCREEN_UpdateIncline(uint8_t incl)
{
    unsigned char factorIncline[2]={'\0','\0'};
    factorIncline[0]=incl%10+'0';
    factorIncline[1]=incl/10+'0';
    lcd_send_data(&factorIncline[1],ADD_INCL_0);
    lcd_send_data(&factorIncline[0],ADD_INCL_1);
}

void SCREEN_UpdateSpeed(uint32_t speed)
{
    unsigned char dataConvert[3];
    Screen_convertData(speed,dataConvert);
    for(int i = 0; i<3 ; i++)
    {
        lcd_send_data(&dataConvert[i],ADD_SPEED_2-2*i);
    }
}

void SCREEN_UpdateEx(uint8_t runEx)
{
    unsigned char ch;
    uint8_t ones = runEx % 10;
    uint8_t tens = runEx /10;
    lcd_send_data("P",6);
    if(tens==0)
    {
        ch=0x7d;
    }
    else
    {
        ch=0x60;
    }
    lcd_show_data1(&ch,4);
    ch = ones +'0';
    lcd_send_data(&ch,2);
}


void SCREEN_MiddleNumber(int number)
{   
    unsigned char ch;
    switch(number)
        {
            case 3:
                ch = 0x7a;
                break;
            case 2:
                ch = 0x3e;
                break;
            case 1:
                ch = 0x60;
                break;
            default:
                break;
        }
        lcd_show_data1(&ch,4);
}

void SCREEN_Tone()
{
    LCD_Tone_On();
    delay(80);
   // SYSTICK_Delay_ms(50);
    LCD_Tone_Off();
}

void mainScreen()
{
    int i=0;
    for(i = 0; i < 31; i+=2)
    {
        if (i!=0 && i!=20 && i!=26 && i!=28)
        {
            lcd_send_data("0",i);
        }
    }
}

void SCREEN_ERROR(uint32_t error)
{
    if(error == 1)
    {
        lcd_send_data("E",6);
        lcd_send_data("5",4);
        lcd_send_data("0",2);
        LCD_Tone_On();
    }
    else
        LCD_Tone_Off();
}

/*******************************************************************************
* EOF
*******************************************************************************/
