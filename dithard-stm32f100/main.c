#include <stm32f10x.h>
#include <stm32f10x_rcc.h>
#include <stm32f10x_gpio.h>
#include <stm32f10x_flash.h>
#include <stm32f10x_usart.h>
#include <stm32f10x_spi.h>
#include <string.h> /* memset */
#include "hd44780\hd44780_driver.h"
#include "FreeRTOS.h"
#include "task.h"
#include "nrf24l01_registers.h"

//Chip Enable Activates RX or TX mode
#define CE_H   GPIO_SetBits(GPIOC, GPIO_Pin_5)
#define CE_L   GPIO_ResetBits(GPIOC, GPIO_Pin_5)

//SPI Chip Select
#define CSN_H  GPIO_SetBits(GPIOC, GPIO_Pin_4)
#define CSN_L  GPIO_ResetBits(GPIOC, GPIO_Pin_4)

uint8_t SPI1_send(uint8_t data){

	SPI1->DR = data; // write data to be transmitted to the SPI data register
	while( !(SPI1->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI1->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI1->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	return SPI1->DR; // return received data from SPI data register
}

uint8_t nrf_read_reg(uint8_t reg){
	CSN_L;
	reg = SPI1_send(reg);
	reg = SPI1_send(NOP);
	CSN_H;
	return reg; // return received data from SPI data register
}
void nrf_write_reg(uint8_t reg, uint8_t data){
	CSN_L;
	reg = SPI1_send(reg | W_REGISTER);
	reg = SPI1_send(data);
	CSN_H;
}
void nrf_send_byte(uint8_t byte){
	CSN_L;
	SPI1_send(W_TX_PAYLOAD);
	SPI1_send(byte);
	CE_L;
	vTaskDelay(1);
	CE_H;
	CSN_H;
}
void vSPI(void *pvParameters) {
	// Configure BLUE Botton (B1 User - PA0) on STM32 Discovery board - Input Floatting
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);


	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	CSN_L;
	CE_L;

	uint8_t SPI_Data = 0xFF, last_state = 0;

	/*------ ENABLE all the clocks and the SPI2-Interface ------*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	/*-------- Configuring SCK, MISO, MOSI --------*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF_PP;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStruct);
//
//	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_12;
//	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF;
//	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
//	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
//	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
//	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*------ SPI init structure ------*/
	SPI_I2S_DeInit(SPI1);
	SPI_InitTypeDef SPI_InitTypeDefStruct;
	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_8;
	SPI_InitTypeDefStruct.SPI_Direction		= SPI_Direction_2Lines_FullDuplex;
	SPI_InitTypeDefStruct.SPI_Mode			= SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_DataSize		= SPI_DataSize_8b;
	SPI_InitTypeDefStruct.SPI_NSS			= SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI_InitTypeDefStruct.SPI_FirstBit		= SPI_FirstBit_MSB;
	SPI_InitTypeDefStruct.SPI_CPOL			= SPI_CPOL_Low;
	SPI_InitTypeDefStruct.SPI_CPHA			= SPI_CPHA_1Edge;
	SPI_Init(SPI1, &SPI_InitTypeDefStruct);
	SPI_Cmd(SPI1, ENABLE);

	nrf_write_reg(CONFIG, PWR_UP | EN_CRC);
	nrf_write_reg(SETUP_RETR, 0x00);

	for (;;) {
		nrf_write_reg(CONFIG, PWR_UP | EN_CRC);
		SPI_Data = nrf_read_reg(CONFIG);
		SPI_Data = nrf_read_reg(EN_AA);
		SPI_Data = nrf_read_reg(EN_RXADDR);
		SPI_Data = nrf_read_reg(SETUP_AW);
		SPI_Data = nrf_read_reg(SETUP_RETR);
		SPI_Data = nrf_read_reg(RF_SETUP);
		SPI_Data = nrf_read_reg(STATUS);
		SPI_Data = nrf_read_reg(OBSERVE_TX);
		if (GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) && last_state == 0) {
			last_state = 1;
			nrf_send_byte(0xAB);
			GPIO_SetBits(GPIOC, GPIO_Pin_9);
		}
		if (!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_0) && (last_state == 1)) {
			last_state = 0;
			nrf_send_byte(0xCC);
			GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		}

//		SPI_Data = SPI_I2S_ReceiveData(SPI2); //Clear RXNE bit
//		SPI_I2S_SendData(SPI2, SPI_Data);
//		while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)){
//			taskYIELD();
//		};

		//return  SPI_I2S_ReceiveData(SPI2);

	}
}
// WAKE test string "0xC0, 0xC9, 0x02, 0x08, 0x73, 0x65, 0x6E, 0x64, 0x64, 0x61, 0x74, 0x61, 0xF0"
enum wake_packet {
	header = 0, address, command, num_bytes, data, crc
};
const unsigned char FEND = 0xC0, // Frame END
		FESC = 0xDB, // Frame ESCape
		TFEND = 0xDC, // Transposed Frame END
		TFESC = 0xDD; // Transposed Frame ESCape

const unsigned char crc8Table[256] = { 0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6,
		0x97, 0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E, 0x43, 0x72, 0x21,
		0x10, 0x87, 0xB6, 0xE5, 0xD4, 0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C,
		0x6D, 0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11, 0x3F, 0x0E, 0x5D,
		0x6C, 0xFB, 0xCA, 0x99, 0xA8, 0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63,
		0x52, 0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB, 0x3D, 0x0C, 0x5F,
		0x6E, 0xF9, 0xC8, 0x9B, 0xAA, 0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22,
		0x13, 0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9, 0xC7, 0xF6, 0xA5,
		0x94, 0x03, 0x32, 0x61, 0x50, 0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D,
		0x2C, 0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95, 0xF8, 0xC9, 0x9A,
		0xAB, 0x3C, 0x0D, 0x5E, 0x6F, 0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7,
		0xD6, 0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED, 0xC3, 0xF2, 0xA1,
		0x90, 0x07, 0x36, 0x65, 0x54, 0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F,
		0xAE, 0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17, 0xFC, 0xCD, 0x9E,
		0xAF, 0x38, 0x09, 0x5A, 0x6B, 0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3,
		0xD2, 0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28, 0x06, 0x37, 0x64,
		0x55, 0xC2, 0xF3, 0xA0, 0x91, 0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1,
		0xD0, 0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69, 0x04, 0x35, 0x66,
		0x57, 0xC0, 0xF1, 0xA2, 0x93, 0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B,
		0x2A, 0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56, 0x78, 0x49, 0x1A,
		0x2B, 0xBC, 0x8D, 0xDE, 0xEF, 0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24,
		0x15, 0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC };
unsigned char motion_motor = 0;
unsigned char motion_time = 0;
unsigned char measure_range = 0;
unsigned char send_acks = 1;
void packet_parser(uint8_t address, uint8_t cmd, uint8_t numofbytes, uint8_t *packet, uint8_t packet_crc) {
	uint8_t i;
	uint8_t crc = 0xFF;
	crc = crc8Table[crc ^ FEND];
	crc = crc8Table[crc ^ address];
	crc = crc8Table[crc ^ cmd];
	crc = crc8Table[crc ^ numofbytes];

	for (i = 0; i < numofbytes; i++) {
		crc = crc8Table[crc ^ packet[i]];
	}
	if (crc != packet_crc) {
		// CRC mismatch!
		//return;
	}
	// Parse
	if (cmd == 0) {
		if(packet[0] == 0x50) { // Disable ACKs
			send_acks = 1;
		}
		if(packet[0] == 0x51) { // Enable ACKs
			send_acks = 0;
		}
	}
	if (cmd == 1) {
		motion_motor = 1;
		motion_time = packet[0];
	}
	if (cmd == 2) {
		//ov7670_set(packet[0],packet[1]);
		motion_motor = 2;
		motion_time = packet[0];
	}
	if (cmd == 3) {
		motion_motor = 3;
		motion_time = packet[0];
	}
	if (cmd == 4) {
		motion_motor = 4;
		motion_time = packet[0];
	}
	if (cmd == 5) {
		measure_range = 1;
	}
}
void usartSendChr(uint16_t data) {
	int j = 0;
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	USART_SendData(USART1, data);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	for (j = 0; j < 24000; ++j) {
		asm("nop");
	}
}

void usartSendWord(uint8_t address, uint8_t cmd, uint16_t word) {
	uint8_t crc = 0xFF;
	uint8_t b1 = word & 0xFF;
	uint8_t b2 = word >> 8;
	crc = crc8Table[crc ^ b2];
	crc = crc8Table[crc ^ b1];
	usartSendChr(FEND); // FEND
	usartSendChr(address); // ADDRESS
	usartSendChr(cmd); // CMD
	usartSendChr(2); // N
	usartSendChr(b2);
	usartSendChr(b1);
	usartSendChr(crc); // CRC
}

void usartSendByte(uint8_t address, uint8_t cmd, uint8_t byte) {
	uint8_t crc = 0xFF;
	crc = crc8Table[crc ^ byte];
	usartSendChr(FEND); // FEND
	usartSendChr(address); // ADDRESS
	usartSendChr(cmd); // CMD
	usartSendChr(1); // N
	usartSendChr(byte);
	usartSendChr(crc); // CRC
}
void vScanUsart(void *pvParameters) {
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitTypeDef USART_InitStructure;
	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);

	unsigned int usart_rx_buff, usart_rx_buff_temp;
	uint8_t wake_packet_status = header;
	uint8_t packet_started = 0;
	uint8_t wake_cmd = 0;
	static uint8_t wake_data[256];
	uint8_t wake_header[4];
	uint8_t wake_data_iterator = 0;
	uint8_t wake_numofbytes = 0;

	for (;;) {
		if ((USART1->SR & USART_FLAG_RXNE) != (u16) RESET) {
			usart_rx_buff = USART_ReceiveData(USART1);
			if (usart_rx_buff == FEND) {
				// Получили FEND, но пакет ещё не окончен - сбрасываем данные
				if (packet_started == 1) {
					wake_data_iterator = 0;
					wake_packet_status = header;
					usart_rx_buff_temp = 0xFF;
					memset(&wake_data[0], 0, sizeof(wake_data));
					memset(&wake_header[0], 0, sizeof(wake_header));
				}
				// Получили FEND = пакет начался
				packet_started = 1;
			} else if (packet_started == 1) {
				// Byte destuffing
				if (usart_rx_buff == TFEND && usart_rx_buff_temp == FESC) {
					usart_rx_buff = FEND;
				} else if (usart_rx_buff == TFESC
						&& usart_rx_buff_temp == FESC) {
					usart_rx_buff = FESC;
				}
				usart_rx_buff_temp = usart_rx_buff;

				// Заполняем массив с заголовком
				switch (wake_packet_status) {
				case header:
					wake_header[address] = usart_rx_buff;
					wake_packet_status = command;
					break;
				case command:
					wake_header[command] = usart_rx_buff;
					wake_cmd = usart_rx_buff;
					wake_packet_status = num_bytes;
					break;
				case num_bytes:
					wake_header[num_bytes] = usart_rx_buff;
					wake_numofbytes = usart_rx_buff;
					wake_packet_status = data;
					break;
				case data:
					if (wake_data_iterator < wake_header[num_bytes]) {
						wake_data[wake_data_iterator] = usart_rx_buff;
						wake_data_iterator++;
					} else if (wake_data_iterator >= wake_header[num_bytes]) {
						wake_header[crc] = usart_rx_buff;
						// Изъять пакет, очистить переменныe
						packet_parser(wake_header[address], wake_cmd, wake_numofbytes, wake_data,
								wake_header[crc]);
						wake_data_iterator = 0;
						wake_packet_status = header;
						packet_started = 0;
						wake_cmd = 0;
						wake_numofbytes = 0;
						usart_rx_buff_temp = 0xFF;
						memset(&wake_data[0], 0, sizeof(wake_data));
						memset(&wake_header[0], 0, sizeof(wake_header));
					}
					break;
				default:
					break;
				}
			}
		}
		taskYIELD();
	}
}

/***************************************************************************//**
 * @brief Init Clock
 ******************************************************************************/
void SetSysClockTo24(void) {
	ErrorStatus HSEStartUpStatus;
	/* SYSCLK, HCLK, PCLK2 and PCLK1 configuration -----------------------------*/
	/* RCC system reset(for debug purpose) */
	RCC_DeInit();

	/* Enable HSE */
	RCC_HSEConfig(RCC_HSE_ON);

	/* Wait till HSE is ready */
	HSEStartUpStatus = RCC_WaitForHSEStartUp();

	if (HSEStartUpStatus == SUCCESS) {
		/* Enable Prefetch Buffer */
		FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);
		/* Flash 0 wait state */
		FLASH_SetLatency(FLASH_Latency_0);
		/* HCLK = SYSCLK */
		RCC_HCLKConfig(RCC_SYSCLK_Div1);
		/* PCLK2 = HCLK */
		RCC_PCLK2Config(RCC_HCLK_Div1);
		/* PCLK1 = HCLK */
		RCC_PCLK1Config(RCC_HCLK_Div1);
		/* PLLCLK = 8MHz * 3 = 24 MHz */
		RCC_PLLConfig(0x00010000, 0x00040000);
		/* Enable PLL */
		RCC_PLLCmd(ENABLE);
		/* Wait till PLL is ready */
		while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET) {
		}
		/* Select PLL as system clock source */
		RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
		/* Wait till PLL is used as system clock source */
		while (RCC_GetSYSCLKSource() != 0x08) {
		}
	} else { /* If HSE fails to start-up, the application will have wrong clock configuration.
	 User can add here some code to deal with this error */
		/* Go to infinite loop */
		while (1) {
		}
	}
}
void vLCD(void *pvParameters) {
	uint8_t user_char[8]; //Сюда будем записывать пользовательский символ
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE); //Вкл порт С
	lcd_init(); //Инициализируем дисплей

	user_char[0]=0b01110; //А вот тут
	user_char[1]=0b10001; // рисуем
	user_char[2]=0b10001; // наш символ
	user_char[3]=0b10001; //
	user_char[4]=0b10001; // Это типа рыба :-)
	user_char[5]=0b01010;
	user_char[6]=0b10001;
	user_char[7]=0b10001;
	lcd_set_user_char(0, user_char); // Наша рыба это символ номер ноль

	lcd_out("This is fish"); //Выводм надпись в нулевую строку
	lcd_set_xy(0,1); //переводим курсор в первую строку
	lcd_send(0,DATA); //выводм символ номер ноль
	lcd_set_state(LCD_ENABLE, CURSOR_DISABLE, NO_BLINK); //Включаем курсор и мигалку
	for (;;) {
		taskYIELD();
	}
}
void vLed(void *pvParameters) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	for (;;) {
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		vTaskDelay(5000);
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		vTaskDelay(5000);
	}
}
void vRange(void *pvParameters) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4; // Echo
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; // Trig
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	uint8_t i = 0;
	uint16_t j = 0;
	for (;;) {
		if (measure_range == 1) {
			measure_range = 0;
			// Trig
			GPIO_SetBits(GPIOA, GPIO_Pin_5);
			vTaskDelay(1); // 100uS
			GPIO_ResetBits(GPIOA, GPIO_Pin_5);

			i = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
			while(!i) {
				i = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
				taskYIELD();
			}
			while(i) {
				i = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_4);
				j++;
				taskYIELD();
			}
			usartSendWord(7,5,j);
			j = 0;
		}
		taskYIELD();
	}
}
void vDrive(void *pvParameters) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	for (;;) {
		switch (motion_motor) {
			case 1:
				// Up
				GPIO_SetBits(GPIOA, GPIO_Pin_0);
				GPIO_ResetBits(GPIOA, GPIO_Pin_1);
				GPIO_SetBits(GPIOA, GPIO_Pin_2);
				GPIO_ResetBits(GPIOA, GPIO_Pin_3);
				vTaskDelay(motion_time * 10);
				motion_motor = 0;
				motion_time = 0;
				if(send_acks) {
					usartSendByte(7,1,1);
				}
				break;
			case 2:
				// Down
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);
				GPIO_SetBits(GPIOA, GPIO_Pin_1);
				GPIO_ResetBits(GPIOA, GPIO_Pin_2);
				GPIO_SetBits(GPIOA, GPIO_Pin_3);
				vTaskDelay(motion_time * 10);
				motion_motor = 0;
				motion_time = 0;
				if(send_acks) {
					usartSendByte(7,2,2);
				}
				break;
			case 3:
				// Left
				GPIO_SetBits(GPIOA, GPIO_Pin_0);
				GPIO_ResetBits(GPIOA, GPIO_Pin_1);
				GPIO_ResetBits(GPIOA, GPIO_Pin_2);
				GPIO_SetBits(GPIOA, GPIO_Pin_3);
				vTaskDelay(motion_time * 10);
				motion_motor = 0;
				motion_time = 0;
				if(send_acks) {
					usartSendByte(7,3,3);
				}
				break;
			case 4:
				// Right
				GPIO_ResetBits(GPIOA, GPIO_Pin_0);
				GPIO_SetBits(GPIOA, GPIO_Pin_1);
				GPIO_SetBits(GPIOA, GPIO_Pin_2);
				GPIO_ResetBits(GPIOA, GPIO_Pin_3);
				vTaskDelay(motion_time * 10);
				motion_motor = 0;
				motion_time = 0;
				if(send_acks) {
					usartSendByte(7,4,4);
				}
				break;
			default:
				motion_motor = 0;
				motion_time = 0;
				break;
		}
		GPIO_ResetBits(GPIOA, GPIO_Pin_0);
		GPIO_ResetBits(GPIOA, GPIO_Pin_1);
		GPIO_ResetBits(GPIOA, GPIO_Pin_2);
		GPIO_ResetBits(GPIOA, GPIO_Pin_3);
		taskYIELD();
	}
}
int main(void)
{
	SetSysClockTo24();
	xTaskCreate( vLed, ( signed char * ) "vLed",
			configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
	xTaskCreate( vSPI, ( signed char * ) "vSPI",
			configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
//	xTaskCreate( vDrive, ( signed char * ) "vDrive",
//			configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
//	xTaskCreate( vScanUsart, ( signed char * ) "vScanUsart",
//			configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
//	//xTaskCreate( vRange, ( signed char * ) "vRange",
//	//		configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
//	xTaskCreate( vLCD, ( signed char * ) "vLCD",
//			configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
	vTaskStartScheduler();
	return 0;
}
