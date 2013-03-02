#include "hd44780_driver.h"

const char Decode2Rus[255-192+1]= {
	0x41,0xA0,0x42,0xA1,0xE0,0x45,0xA3,0xA4,
	0xA5,0xA6,0x4B,0xA7,0x4D,0x48,0x4F,0xA8,
	0x50,0x43,0x54,0xA9,0xAA,0x58,0xE1,0xAB,
	0xAC,0xE2,0xAD,0xAE,0xAD,0xAF,0xB0,0xB1,
	0x61,0xB2,0xB3,0xB4,0xE3,0x65,0xB6,0xB7,
	0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0x6F,0xBE,
	0x70,0x63,0xBF,0x79,0xE4,0x78,0xE5,0xC0,
	0xC1,0xE6,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7
};

void hd44780_delay(void) {
	volatile uint32_t tmpvar;
	for (tmpvar=4000;tmpvar!=0;tmpvar--);
}

void hd44780_init() {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	volatile uint32_t tmpvar;

	LCD_PORT->CRH |= LCD_PORT_CRH_S;
	LCD_PORT->CRL |= LCD_PORT_CRL_S;
	LCD_PORT->CRH &= ~(LCD_PORT_CRH_C);
	LCD_PORT->CRL &= ~(LCD_PORT_CRL_C);

	/* Set 4-bit Mode */
	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC | LCD_CD_BC | LCD_EN_BC);
	LCD_PORT->BSRR=(LCD_DB5_BS);

	LCD_PORT->BSRR=LCD_EN_BS;
	for (tmpvar=20000;tmpvar!=0;tmpvar--);
	LCD_PORT->BSRR=LCD_EN_BC;
	for (tmpvar=20000;tmpvar!=0;tmpvar--);

	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC | LCD_CD_BC | LCD_EN_BC);
	LCD_PORT->BSRR=(LCD_DB5_BS);

	LCD_PORT->BSRR=LCD_EN_BS;
	for (tmpvar=20000;tmpvar!=0;tmpvar--);;
	LCD_PORT->BSRR=LCD_EN_BC;
	for (tmpvar=20000;tmpvar!=0;tmpvar--);

	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC | LCD_CD_BC | LCD_EN_BC);
	LCD_PORT->BSRR=(LCD_DB7_BS);

	LCD_PORT->BSRR=LCD_EN_BS;
	for (tmpvar=20000;tmpvar!=0;tmpvar--);
	LCD_PORT->BSRR=LCD_EN_BC;
	for (tmpvar=20000;tmpvar!=0;tmpvar--);


	hd44780_set_state(LCD_ENABLE,CURSOR_DISABLE,NO_BLINK);
	hd44780_clear();
	hd44780_send(0x06,COMMAND);
}

void hd44780_set_user_char(uint8_t char_num, uint8_t * char_data) {
	uint8_t i;
	hd44780_send(((1<<6) | (char_num * 8) ), COMMAND);
	for (i=0;i<=7;i++) {
		hd44780_send(char_data[i],DATA);
	}
	hd44780_send((1<<7), COMMAND);
}

void hd44780_set_xy(uint8_t x, uint8_t y)  {
	if (y==0) {
		hd44780_send( ((1<<7) | (x)),COMMAND);
	} else {
		hd44780_send( ((3<<6) | (x)),COMMAND);
	}
}


void hd44780_out(char * txt) {
	while(*txt) {
		hd44780_send(*txt,DATA);
		txt++;
	}
}

void hd44780_clear(void) {
	hd44780_send(0x01,COMMAND);
}

void hd44780_set_state(lcd_state state, cursor_state cur_state, cursor_mode cur_mode)  {
	if (state==LCD_DISABLE)  {
		hd44780_send(0x08,COMMAND);
	} else {
		if (cur_state==CURSOR_DISABLE) {
			if (cur_mode==NO_BLINK)  {
				hd44780_send(0x0C,COMMAND);
			} else {
				hd44780_send(0x0D,COMMAND);
			}
		} else  {
			if (cur_mode==NO_BLINK)  {
				hd44780_send(0x0E,COMMAND);
			} else {
				hd44780_send(0x0F,COMMAND);
			}
		}
	}
}

void hd44780_send(uint8_t byte, dat_or_comm dc)  {

	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC | LCD_CD_BC | LCD_EN_BC);

	if (dc) {
		LCD_PORT->BSRR=LCD_CD_BS;
		if (byte > 0x7F) {
			byte = Decode2Rus[byte-192];
		}
	}

	if (byte & 0x10) {
		LCD_PORT->BSRR=LCD_DB4_BS;
	}
	if (byte & 0x20) {
		LCD_PORT->BSRR=LCD_DB5_BS;
	}
	if (byte & 0x40) {
		LCD_PORT->BSRR=LCD_DB6_BS;
	}
	if (byte & 0x80) {
		LCD_PORT->BSRR=LCD_DB7_BS;
	}

	LCD_PORT->BSRR=LCD_EN_BS;
	hd44780_delay();
	LCD_PORT->BSRR=LCD_EN_BC;
	hd44780_delay();


	LCD_PORT->BSRR=(LCD_DB7_BC | LCD_DB6_BC | LCD_DB5_BC | LCD_DB4_BC );

	if (byte & 0x01) {
		LCD_PORT->BSRR=LCD_DB4_BS;
	}
	if (byte & 0x02) {
		LCD_PORT->BSRR=LCD_DB5_BS;
	}
	if (byte & 0x04) {
		LCD_PORT->BSRR=LCD_DB6_BS;
	}
	if (byte & 0x08) {
		LCD_PORT->BSRR=LCD_DB7_BS;
	}

	LCD_PORT->BSRR=LCD_EN_BS;
	hd44780_delay();
	LCD_PORT->BSRR=LCD_EN_BC;
	hd44780_delay();


}
