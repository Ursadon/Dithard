#ifndef __LCD_H
#define __LCD_H

#define LCD_PIN_ENABLE      GPIO_Pin_1
#define LCD_PIN_RESET       GPIO_Pin_0
#define LCD_PIN_COMMAND     GPIO_Pin_2
#define LCD_PIN_DATA        GPIO_Pin_3
#define LCD_PIN_CLOCK       GPIO_Pin_4

#define LCD_CMD             0
#define LCD_DATA            1

void LcdWrite(unsigned int dataOrCommand, unsigned int value);
void LCD_print_num(unsigned char num, unsigned char x, unsigned char y);
void LCD_clear(void);
void delay_ms(unsigned long nCount);
void SetupLCD(void);

#endif
