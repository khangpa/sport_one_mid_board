//lcd.h 
 
#ifndef	_LCD_H_ 
#define _LCD_H_ 
#define SIZE_BUFF   (1U)
#define DOT_SEG_1   (0x80)
#define DOT_SEG_2   (0x08)

#define ADD_SPEED_0  (2U)
#define ADD_SPEED_1  (4U)
#define ADD_SPEED_2  (6U)

#define ADD_TIME_SECOND_0  (12U)
#define ADD_TIME_SECOND_1  (10U)
#define ADD_TIME_MINUTE_0  (8U)
#define ADD_TIME_MINUTE_1  (0U)

#define ADD_DIS_0  (14U)
#define ADD_DIS_1  (16U)
#define ADD_DIS_2  (18U)

#define ADD_CAL_0  (20U)
#define ADD_CAL_1  (22U)
#define ADD_CAL_2  (24U)

#define ADD_INCL_0  (28U)
#define ADD_INCL_1  (30U)

void LCD_GpioInit(void);					// lcd gpio��ʼ�� 
void LCD_Init(void);
void test_line(void);// lcd ģ���ʼ��	 
void lcd_show_data(unsigned char *puts);// lcd ��ʾ��������ϣ����ʾ��12345678�����ɴ��롰12345678����data�ĳ̶Ȳ���С��8 
void lcd_clr(void);	
void delay(uint32_t MS);// lcd �������� 
void lcd_test(void);						// lcd ���Ժ��� 
void lcd_clr_section(uint8_t address, uint8_t size);
void lcd_show_data1(unsigned char *puts,uint8_t address)  ;
void lcd_send_data(unsigned char *data,uint8_t address) ;
void lcd_send_data1(unsigned char *data,uint8_t address) ;
void LCD_Tone_On();
void LCD_Tone_Off();
#endif