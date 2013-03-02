#include "lcd_nokia5110.h"
#include "font.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"

	//SetupLCD();
	//LCD_print_num(0,15,0);

void LcdWrite(unsigned int dataOrCommand, unsigned int value) {
	if (dataOrCommand)
		GPIO_SetBits(GPIOA, LCD_PIN_COMMAND);
	else
		GPIO_ResetBits(GPIOA, LCD_PIN_COMMAND);

	GPIO_ResetBits(GPIOA, LCD_PIN_ENABLE); // enable LCD

	int i;
	for (i = 0; i < 8; i++) {
		GPIO_ResetBits(GPIOA, LCD_PIN_CLOCK);
		if (value & 0x80)
			GPIO_SetBits(GPIOA, LCD_PIN_DATA);
		else
			GPIO_ResetBits(GPIOA, LCD_PIN_DATA);
		value <<= 1;
		GPIO_SetBits(GPIOA, LCD_PIN_CLOCK); // LCD latches data on the rising clock edge
	}

	GPIO_SetBits(GPIOA, LCD_PIN_ENABLE); // disable LCD
}

void delay_ms(unsigned long nCount) {
	uint32_t delay = nCount * 2516; // approximate loops per ms at 24 MHz, Debug config
	for (; delay != 0; delay--)
		;
}

void LCD_set_XY(unsigned char x, unsigned char y) {
	LcdWrite(LCD_CMD, 0x40 + y);
	LcdWrite(LCD_CMD, 0x80 + x);
}

void LCD_print_num(unsigned char num, unsigned char x, unsigned char y) {
	int horizontal = 0;
	int vertical = 0;
	for (vertical = 0; vertical < 3; vertical++) {
		LCD_set_XY(x, y + vertical);
		for (horizontal = 0; horizontal < 16; horizontal++) {
			LcdWrite(LCD_DATA, num_font[num][vertical][horizontal]);
		}
	}
}

void LCD_clear(void){
	for (int i=0; i<84*4; i++)
		LcdWrite(LCD_DATA, 0x00);
}

void SetupLCD(void) {
	GPIO_InitTypeDef GPIO_InitStructure;
	// Initialize PA8-PA12 as outputs
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Pin = LCD_PIN_ENABLE | LCD_PIN_RESET
			| LCD_PIN_COMMAND | LCD_PIN_DATA | LCD_PIN_CLOCK;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	// Reset LCD
	GPIO_SetBits(GPIOA, LCD_PIN_RESET);
	delay_ms(10);
	GPIO_ResetBits(GPIOA, LCD_PIN_RESET);
	delay_ms(10);
	GPIO_SetBits(GPIOA, LCD_PIN_RESET);
	// Initialize LCD state
	LcdWrite(LCD_CMD, 0x21); // LCD Extended Commands.
	LcdWrite(LCD_CMD, 0xc6); // Set LCD Vop (Contrast).
	LcdWrite(LCD_CMD, 0x06); // Set Temp coefficent.
	LcdWrite(LCD_CMD, 0x13); // LCD bias mode
	LcdWrite(LCD_CMD, 0x20);
	LcdWrite(LCD_CMD, 0x0C); // LCD in normal mode.
}
