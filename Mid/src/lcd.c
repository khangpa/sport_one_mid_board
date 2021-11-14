#include <stdint.h>
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"   
#include "stm32f10x_gpio.h" 
#include "lcd.h" 
#include "keypad.h"
#define HT1621_GPIO         GPIOB
#define HT1621_CS           GPIO_Pin_11
#define HT1621_WR           GPIO_Pin_10
#define HT1621_DAT          GPIO_Pin_1
#define ENABLE_CLOCK_HT1621_GPIO  (RCC->APB2ENR|=(1<<3))
   
#define  DAT   1    //����   
#define  COM   0    //����    
   
   
//һ��HT1621 4COM�ĳ��ñ���,��λ��COM0,��4bit��SEG0,��4bit��SEG1   
        //�ڰ���ʾλ
//  _   
// |_|   ��Ե�8����ʾλ 1--f 2--e 3--d 4--c 5--g 6--b 7--a    
// |_|     
//     
//         
//unsigned char NUM[]={   
//  0x60,0x60,0x60,0xe0,0x60,0xe0,0x60,     // 0x60 -- 1   
//  0x3e,0xbe,0xbe,0x3e,0xbe,0xbe,0xbe,     // 0x3e -- 2   
//  0x7a,0x7a,0xfa,0xfa,0xfa,0xfa,0xfa,     // 0x7a -- 3   
//  0xe3,0x63,0x63,0xe3,0xe3,0xe3,0x63,     // 0x63 -- 4   
//  0xdb,0x5b,0xdb,0xdb,0xdb,0x5b,0xdb,     // 0x5b -- 5   
//  0xdf,0xdf,0xdf,0xdf,0xdf,0x5f,0xdf,     // 0x5f -- 6   
//  0x70,0x70,0x70,0xf0,0x70,0xf0,0xf0,     // 0x70 -- 7   
//  0xff,0xff,0xff,0xff,0xff,0xff,0xff,     // 0x7f -- 8   
//  0xfb,0x7b,0xfb,0xfb,0xfb,0xfb,0xfb,     // 0x7b -- 9   
//  0xfd,0xfd,0xfd,0xfd,0x7d,0xfd,0xfd,     // 0x7d -- 0   
//};   
   
/*  
* fq  
* LCD GPIO ��ʼ��  
* ���:void  
* ����:void  
* �õ� HT1621_GPIO 4,5,6,7 �ֱ��Ӧlcd CS RD(��ʱ����) WR DAT  
*/   
void LCD_GpioInit(void)   
{
    ENABLE_CLOCK_HT1621_GPIO;
    
    GPIO_InitTypeDef GPIO_InitStruct_LCD; 
   
    GPIO_InitStruct_LCD.GPIO_Pin    = HT1621_CS|HT1621_WR|HT1621_DAT;   
    GPIO_InitStruct_LCD.GPIO_Mode   = GPIO_Mode_Out_OD;   
    GPIO_InitStruct_LCD.GPIO_Speed  = GPIO_Speed_50MHz;   
    //GPIO_InitStruct_LCD.GPIO_PuPd   = GPIO_PuPd_UP;   
    //GPIO_InitStruct_LCD.GPIO_OType  = GPIO_OType_PP;   
    GPIO_Init(HT1621_GPIO,&GPIO_InitStruct_LCD);  
}   
   
/*  
* fq  
* LCD ģʽд��  
* ���:MODE :COM(����ģʽ) DAT(����ģʽ)      
* ����:void  
*  
*/   
   
void LCD_WriteMode(unsigned char MODE) //д��ģʽ,����or����   
{   
   GPIO_ResetBits(HT1621_GPIO,HT1621_WR);                                //  RW = 0;   
       
    GPIO_SetBits(HT1621_GPIO,HT1621_DAT);                                 //  DA = 1;   
    GPIO_SetBits(HT1621_GPIO,HT1621_WR);                                  //  RW = 1;   
       
       
    GPIO_ResetBits(HT1621_GPIO,HT1621_WR);                                //  RW = 0;   
       
    GPIO_ResetBits(HT1621_GPIO,HT1621_DAT);                               //  DA = 0;   
    GPIO_SetBits(HT1621_GPIO,HT1621_WR);                                  //  RW = 1;   
       
       
    GPIO_ResetBits(HT1621_GPIO,HT1621_WR);                                //  RW = 0;   
       
   
    if(0==MODE)   
    {   
        GPIO_ResetBits(HT1621_GPIO,HT1621_DAT);                           //  DA = 0;   
    }   
    else   
    {   
        GPIO_SetBits(HT1621_GPIO,HT1621_DAT);                             //  DA = 1;   
    }   
       
    GPIO_SetBits(HT1621_GPIO,HT1621_WR);                                  //  RW = 1;   
       
}
   
/*  
* fq  
* LCD ����д�뺯��  
* ���:cbyte ,����������  
* ����:void  
*  
*/   
   
void LCD_WriteCommand(unsigned char Cbyte)                             //д�������9bit,���1bitû��   
{   
   unsigned char i;   
    for(i=0;i<8;i++)   
    {   
        GPIO_ResetBits(HT1621_GPIO,HT1621_WR);                            //  RW = 0;   
           
   
        if((Cbyte>>(7-i)) & 0x01)   
        {   
            GPIO_SetBits(HT1621_GPIO,HT1621_DAT);                         //  DA = 1;   
        }   
        else   
        {   
            GPIO_ResetBits(HT1621_GPIO,HT1621_DAT);                       //  DA = 0;   
        }   
           
        GPIO_SetBits(HT1621_GPIO,HT1621_WR);                              //  RW = 1;   
           
    }   
    GPIO_ResetBits(HT1621_GPIO,HT1621_WR);                                //  RW = 0;   
       
    GPIO_ResetBits(HT1621_GPIO,HT1621_DAT);                               //  DA = 0; ��9bit,������,���Ǳ���д��   
    GPIO_SetBits(HT1621_GPIO,HT1621_WR);                                  //  RW = 1;   
    
}   
   
/*  
* fq  
* LCD ��ַд�뺯��  
* ���:cbyte ,��ַ  
* ����:void  
*  
*/   
void LCD_WriteAddress(unsigned char Abyte)                             //д��ַ6bit   
{   
   unsigned char i;   
    Abyte = Abyte<<2;   
    for(i=0;i<6;i++)   
    {   
        GPIO_ResetBits(HT1621_GPIO,HT1621_WR);                            //  RW = 0;   
           
   
        if((Abyte>>(7-i)) & 0x01)   
        {   
            GPIO_SetBits(HT1621_GPIO,HT1621_DAT);                         //  DA = 1;   
        }   
        else   
        {   
            GPIO_ResetBits(HT1621_GPIO,HT1621_DAT);                       //  DA = 0;   
        }   
           
        GPIO_SetBits(HT1621_GPIO,HT1621_WR);                              //  RW = 1;   
           
    }   
}   
/*  
* fq  
* LCD ����д�뺯��  
* ���:Dbyte ,����  
* ����:void  
*  
*/   
void LCD_WriteData(unsigned char Dbyte)    //д����   
{   
    int i;   
    for(i=0;i<8;i++)   
    {   
        GPIO_ResetBits(HT1621_GPIO,HT1621_WR);                            //  RW = 0;   
           
   
        if((Dbyte>>(7-i)) & 0x01)   
        {   
            GPIO_SetBits(HT1621_GPIO,HT1621_DAT);                         //  DA = 1;   
        }   
        else   
        {   
            GPIO_ResetBits(HT1621_GPIO,HT1621_DAT);                       //  DA = 0;   
        }   
           
        GPIO_SetBits(HT1621_GPIO,HT1621_WR);                              //  RW = 1;   
           
    }   
    
}   
/*  
* fq  
* LCD ģ���ڲ�ר����ʱ����  
* ���:ms  
* ����:void  
*  
*/   
void delay(uint32_t MS)                                         //��ʱ   
{   
    unsigned char us,usn;   
    while(MS!=0)   
    {    
        usn = 2;   
        while(usn!=0)   
        {   
            us=0xf2;   
            while (us!=0){us--;};   
            usn--;   
        }   
        MS--;   
    }   
}   


//void delay(uint32_t MS,char key)                                         //��ʱ   
//{   
//    uint32_t mS=MS*484;
//    while(key == NO_KEY && (mS))   
//    {    
//        mS--;   
//    }   
//}
/*  
* fq  
* LCD ��ʼ������lcd��������ʼ������  
* ���:void  
* ����:void  
*  
*/   
void LCD_Init( void )                                               //��ʼ��   
{   
    //GPIO_SetBits(HT1621_GPIO,HT1621_CS);
    GPIO_ResetBits(HT1621_GPIO,HT1621_CS);                                //CS = 0;   
    LCD_WriteMode(COM);        //����ģʽ   
    LCD_WriteCommand(0x01);    //Enable System   
    LCD_WriteCommand(0x03);    //Enable Bias   
    LCD_WriteCommand(0x04);    //Disable Timer   
    LCD_WriteCommand(0x05);    //Disable WDT  
    LCD_WriteCommand(0x09);
    LCD_WriteCommand(0x08);    //Tone OFF   
    LCD_WriteCommand(0x18);    //on-chip RC��   
    LCD_WriteCommand(0x29);    //1/4Duty 1/3Bias   
    LCD_WriteCommand(0x80);    //Disable IRQ   
    LCD_WriteCommand(0x40);    //Tone Frequency 4kHZ   
    LCD_WriteCommand(0xE3);    //Normal Mode
    GPIO_SetBits(HT1621_GPIO,HT1621_CS);                                  //CS = 1;   
}   
/*  
* fq  
* LCD ������ʾ����  
* ���:��ʾ�������飬n ��ʾ��������  
* ����:void  
*  
*/   
void full_ram(unsigned char *puts,unsigned char n)   
{   
    unsigned char i;   
       
    GPIO_ResetBits(HT1621_GPIO,HT1621_CS);                                // CS = 0;   
    LCD_WriteMode(DAT);   
    LCD_WriteAddress(0);   
    for(i=0;i<15;i++)   
    {   
        LCD_WriteData(puts[15*n+i]);
    }      
    GPIO_SetBits(HT1621_GPIO,HT1621_CS);                                  //CS = 1;   
}   
/*  
* fq  
* LCD ����д�뺯��  
* ���:puts ����ʾ���ַ���,����Ϊ7���������������򳬳�������Ч��������������룬��: "12234567"  
* ����:void  
*  
*/   
   

void lcd_show_data(unsigned char *puts)   
{   
 unsigned char i ;   
       
    GPIO_ResetBits(HT1621_GPIO,HT1621_CS);                                // CS = 0;   
    LCD_WriteMode(DAT);   
    LCD_WriteAddress(0);   
       
    for(i=0;i<1;i++)   
    {   
        LCD_WriteData(puts[i]);
    }   
    GPIO_SetBits(HT1621_GPIO,HT1621_CS);                                  //CS = 1;   
}
/*  
* fq  
* LCD ����ʵ����ʱ����  
* ���:times  
* ����:void  
*  
*/   
void lcd_delay(int times)   
{   
    int i;   
    for(i=0;i<20*times;i++) ;   
}   
   
/*  
* fq  
* LCD ��������  
* ���:times  
* ����:void  
*  
*/   
   
void lcd_clr(void)   
{   
   unsigned char i;   
       
    GPIO_ResetBits(HT1621_GPIO,HT1621_CS);                              // CS = 0;   
    LCD_WriteMode(DAT);   
    LCD_WriteAddress(0);   
       
    for(i=0;i<16;i++)   
    {   
        LCD_WriteData(0x00);   
    }      
    GPIO_SetBits(HT1621_GPIO,HT1621_CS);                                  //CS = 1;   
}   
   
/*  
* fq  
* ����ܱʶκ���  
* ���:void  
* ����:void  
*  
*/   
/* #if 1  
    unsigned char HT1621[]={   
       
        0x81,0x01,0x01,0x01,0x01,0x01,0x01,   
        0x02,0x02,0x02,0x02,0x82,0x02,0x02,   
        0x04,0x84,0x04,0x04,0x04,0x04,0x04,   
        0x08,0x08,0x88,0x08,0x08,0x08,0x08,   
        0x10,0x10,0x10,0x10,0x10,0x10,0x90,   
        0x20,0x20,0x20,0x20,0x20,0xa0,0x20,   
        0x40,0x40,0x40,0xc0,0x40,0x40,0x40,   
        };   
       
void test_line(void)   
{   
    int i ;   
    for(i=0;i<7;i++)   
    {   
        full_ram(HT1621,i); 
    }   
    GPIO_ResetBits(HT1621_GPIO,HT1621_CS);                                // CS = 0;   
   
    LCD_WriteMode(COM);   
    GPIO_SetBits(HT1621_GPIO,HT1621_CS);                                  //CS = 1;   
}      
#endif   
    */
/*  
* fq  
* LCD ����д�뺯��������ӿ�  
* ���:data ����ʾ���ַ���,����Ϊ7���������������򳬳�������Ч��������������룬��: "12234567"  
* ����:void  
*  
*/ 
void lcd_show_data1(unsigned char *puts,uint8_t address)
{   
    unsigned char i ;   
       
    GPIO_ResetBits(HT1621_GPIO,HT1621_CS);                                // CS = 0;   
    LCD_WriteMode(DAT);   
    LCD_WriteAddress(address);   
       
    for(i=0;i<SIZE_BUFF;i++)   
    {   
        LCD_WriteData(puts[i]);    
    }   
    GPIO_SetBits(HT1621_GPIO,HT1621_CS);                                  //CS = 1;   
}

void lcd_send_data(unsigned char *data,uint8_t address)   
{   
    char i = 0 ;   
    unsigned char show_buf[SIZE_BUFF]={'\0'};   
     
    while(data[i]!='\0')                                             //ȡǰ��λ��ʾ��   
    {   
        switch(data[i])   
        {   
            case '0':                    
                if(address==0||address==2||address==4||address==6)
                {
                    show_buf[i] = 0x7d|DOT_SEG_1;
                }
                else
                {
                    show_buf[i] = 0xd7;                   
                }                
                break;   
            case '1':
                if(address==0||address==2||address==4||address==6)
                {
                    show_buf[i] = 0x60|DOT_SEG_1;        
                }
                else
                {
                    show_buf[i] = 0x06; 
                }              
                break;   
            case '2':
                if(address==0||address==2||address==4||address==6)
                {
                    show_buf[i] = 0x3e|DOT_SEG_1; 
                }
                else
                {
                    show_buf[i] = 0xe3; 
                }                
                break;   
            case '3':
                if(address==0||address==2||address==4||address==6)
                {
                    show_buf[i] = 0x7a|DOT_SEG_1;
                }
                else
                {
                    show_buf[i] = 0xa7; 
                }                 
                break;   
            case '4':
                if(address==0||address==2||address==4||address==6)
                {
                    show_buf[i] = 0x63|DOT_SEG_1;
                }
                else
                {
                    show_buf[i] = 0x36; 
                }               
                break;   
            case '5':
                if(address==0||address==2||address==4||address==6)
                {
                    show_buf[i] = 0x5b|DOT_SEG_1;
                }
                else
                {
                    show_buf[i] = 0xb5; 
                }              
                break;   
            case '6':
                if(address==0||address==2||address==4||address==6)
                {
                    show_buf[i] = 0x5f|DOT_SEG_1;
                }
                else
                {
                    show_buf[i] = 0xf5; 
                }               
                break;   
            case '7':
                if(address==0||address==2||address==4||address==6)
                {
                    show_buf[i] = 0x70|DOT_SEG_1;
                }
                else
                {
                    show_buf[i] = 0x07; 
                }                  
                break;   
            case '8':
                if(address==0||address==2||address==4||address==6)
                {
                    show_buf[i] = 0x7f|DOT_SEG_1;
                }
                else
                {
                    show_buf[i] = 0xf7; 
                }                 
                break;   
            case '9':
                if(address==0||address==2||address==4||address==6)
                {
                    show_buf[i] = 0x7b|DOT_SEG_1;
                }
                else
                {
                    show_buf[i] = 0xb7; 
                }                 
                break;
            case 'P':
                if(address==6)
                {
                    show_buf[i] = 0x37;
                }
                break;
            case 'E':
                if(address==6)
                {
                    show_buf[i] = 0x1F;

                }
                break;
            default:
                show_buf[i] = 0xff;
                break;
        }
        if(address==8||address==12||address==22||address==14        
           ||address==24)
        {
            show_buf[i] |= 0x08;
        }
        i++;       
    }
    lcd_show_data1(show_buf,address);
}

void lcd_send_data1(unsigned char* data,uint8_t address)
{
    char i = 0 ;   
    unsigned char show_buf[SIZE_BUFF]={'\0'};   
     
    while(data[i]!='\0')                                             //ȡǰ��λ��ʾ��   
    {   
        switch(data[i])   
        {   
            case '0':                    
                show_buf[i] = 0xd7;                
                break;   
            case '1':
                show_buf[i] = 0x06;             
                break;   
            case '2':
                show_buf[i] = 0xe3;                
                break;   
            case '3':
                show_buf[i] = 0xa7;                
                break;   
            case '4':
                show_buf[i] = 0x36;               
                break;   
            case '5':
                show_buf[i] = 0xb5;              
                break;   
            case '6':
                show_buf[i] = 0xf5;              
                break;   
            case '7':
                show_buf[i] = 0x07;                 
                break;   
            case '8':
                show_buf[i] = 0xf7;                
                break;   
            case '9':
                show_buf[i] = 0xb7;                
                break;
            case 'P':
                show_buf[i] = 0x37;
                break;
            default:
                show_buf[i] = 0xff;
                break;
        }
        if(address==16)
        {
            show_buf[i] |= 0x08;
        }
        i++;       
    }
    lcd_show_data1(show_buf,address);
}

void lcd_clr_section(uint8_t address, uint8_t size)
{   
    unsigned char i,j;   
       
    GPIO_ResetBits(HT1621_GPIO,HT1621_CS);                                // CS = 0;   
    LCD_WriteMode(DAT);   
    LCD_WriteAddress(address);   
    j=address;
    for(i=0;i<size;i++)   
    {      
        if(j==0)
        {
            LCD_WriteData(0x80);
        }
        else if(j==12 || j==24)
        {
            LCD_WriteData(0x08);
        }
        else
        {
            LCD_WriteData(0x00);
        }
        j+=2;          
    }      
    GPIO_SetBits(HT1621_GPIO,HT1621_CS);                                  //CS = 1;   
}

void LCD_Tone_On()
{
    GPIO_ResetBits(HT1621_GPIO,HT1621_CS);                               //CS = 0;
    LCD_WriteMode(COM);
    LCD_WriteCommand(0x09);                                             //ON
    GPIO_SetBits(HT1621_GPIO,HT1621_CS);                                //CS = 1;
}
void LCD_Tone_Off()
{
    GPIO_ResetBits(HT1621_GPIO,HT1621_CS);
    LCD_WriteMode(COM);
    LCD_WriteCommand(0x08);                                             //OFF
    GPIO_SetBits(HT1621_GPIO,HT1621_CS);                                //CS = 1;
}
/***************************************************end file*************************************************/   