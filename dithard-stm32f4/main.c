#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_spi.h>
#include <stm32f4xx_usart.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "nrf24l01_registers.h"

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


//Chip Enable Activates RX or TX mode
#define CE_H   GPIO_SetBits(GPIOD, GPIO_Pin_10)
#define CE_L   GPIO_ResetBits(GPIOD, GPIO_Pin_10)

//SPI Chip Select
#define CSN_H  GPIO_SetBits(GPIOD, GPIO_Pin_9)
#define CSN_L  GPIO_ResetBits(GPIOD, GPIO_Pin_9)

void PrintChar(char c)
{
	uint8_t ch;
	ch = c;
	/* Place your implementation of fputc here */
	/* e.g. write a character to the USART */
	USART_SendData(USART2, (uint8_t) ch);
	/* Loop until the end of transmission */
	while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET) {
	}
}

void PrintString(const char * str)
{
	int i = 0;
	while (str [i] != 0) {
		  /* Loop until the end of transmission */
		  while (USART_GetFlagStatus(USART2, USART_FLAG_TC) == RESET)
		  {}
		  USART_SendData(USART2, (uint8_t)str[i++]);
	}
}
void usartSendByte(uint8_t address, uint8_t cmd, uint8_t byte) {
	uint8_t crc = 0xFF;
	crc = crc8Table[crc ^ byte];
	PrintChar(FEND); // FEND
	PrintChar(address); // ADDRESS
	PrintChar(cmd); // CMD
	PrintChar(1); // N
	PrintChar(byte);
	PrintChar(crc); // CRC
}
uint8_t SPI2_send(uint8_t data){

	SPI2->DR = data; // write data to be transmitted to the SPI data register
	while( !(SPI2->SR & SPI_I2S_FLAG_TXE) ); // wait until transmit complete
	while( !(SPI2->SR & SPI_I2S_FLAG_RXNE) ); // wait until receive complete
	while( SPI2->SR & SPI_I2S_FLAG_BSY ); // wait until SPI is not busy anymore
	return SPI2->DR; // return received data from SPI data register
}

void vLED(void *pvParameters) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14
			| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	for (;;) {
		GPIO_SetBits(GPIOD, GPIO_Pin_12);
		vTaskDelay(1000);
		GPIO_ResetBits(GPIOD, GPIO_Pin_12);
		vTaskDelay(1000);
//		GPIO_SetBits(GPIOD, GPIO_Pin_14);
//		vTaskDelay(1000);
//		GPIO_SetBits(GPIOD, GPIO_Pin_15);
//		vTaskDelay(1000);
//		GPIO_ResetBits(GPIOD,
//				GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
//		vTaskDelay(1000);
	}
}

uint8_t nrf_read_reg(uint8_t reg){
	CSN_L;
	reg = SPI2_send(reg);
	reg = SPI2_send(NOP);
	CSN_H;
	return reg; // return received data from SPI data register
}
void nrf_write_reg(uint8_t reg, uint8_t data){
	CSN_L;
	reg = SPI2_send(reg | W_REGISTER);
	reg = SPI2_send(data);
	CSN_H;
}
void nrf_send_byte(uint8_t byte){
	CSN_L;
	SPI2_send(W_TX_PAYLOAD);
	SPI2_send(byte);
	CE_L;
	vTaskDelay(1);
	CE_H;
	CSN_H;
}

void vSPI(void *pvParameters) {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10; // 9 - SCN, 10 - ~CE
	GPIO_InitStructure.GPIO_Mode	= GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed	= GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	CSN_L;
	CE_L;

	uint8_t SPI_Data, rx_data = 0xFF;

	/*------ ENABLE all the clocks and the SPI2-Interface ------*/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	/*--- assign our pins explicitly to the SPI2 interface ---*/
	GPIO_PinAFConfig(GPIOB,	GPIO_PinSource12,	GPIO_AF_SPI2); // NSS
	GPIO_PinAFConfig(GPIOB,	GPIO_PinSource13,	GPIO_AF_SPI2); // SCK
	GPIO_PinAFConfig(GPIOB,	GPIO_PinSource14,	GPIO_AF_SPI2); // MISO
	GPIO_PinAFConfig(GPIOB,	GPIO_PinSource15,	GPIO_AF_SPI2); // MOSI

	/*-------- Configuring SCK, MISO, MOSI --------*/
	GPIO_InitTypeDef GPIO_InitStruct;
	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin	= GPIO_Pin_12;
	GPIO_InitStruct.GPIO_Mode	= GPIO_Mode_AF;
	GPIO_InitStruct.GPIO_Speed	= GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_OType	= GPIO_OType_PP;
	GPIO_InitStruct.GPIO_PuPd	= GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStruct);

	/*------ SPI init structure ------*/
	SPI_I2S_DeInit(SPI2);
	SPI_InitTypeDef SPI_InitTypeDefStruct;
	SPI_InitTypeDefStruct.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_16;
	SPI_InitTypeDefStruct.SPI_Direction		= SPI_Direction_2Lines_FullDuplex;
	SPI_InitTypeDefStruct.SPI_Mode			= SPI_Mode_Master;
	SPI_InitTypeDefStruct.SPI_DataSize		= SPI_DataSize_8b;
	SPI_InitTypeDefStruct.SPI_NSS			= SPI_NSS_Soft | SPI_NSSInternalSoft_Set;
	SPI_InitTypeDefStruct.SPI_FirstBit		= SPI_FirstBit_MSB;
	SPI_InitTypeDefStruct.SPI_CPOL			= SPI_CPOL_Low;
	SPI_InitTypeDefStruct.SPI_CPHA			= SPI_CPHA_1Edge;
	SPI_Init(SPI2, &SPI_InitTypeDefStruct);
	SPI_Cmd(SPI2, ENABLE);
	vTaskDelay(100);
	nrf_write_reg(CONFIG, 0x0B);
	nrf_write_reg(SETUP_RETR, 0x00);
	nrf_write_reg(RX_PW_P0, 0x01);
	nrf_write_reg(CONFIG, 0x0B);
	for (;;) {
		CE_H;
		SPI_Data = nrf_read_reg(CONFIG);
		SPI_Data = nrf_read_reg(RX_PW_P0);
		SPI_Data = nrf_read_reg(CD);
		SPI_Data = nrf_read_reg(EN_AA);
		SPI_Data = nrf_read_reg(EN_RXADDR);
		SPI_Data = nrf_read_reg(SETUP_AW);
		SPI_Data = nrf_read_reg(SETUP_RETR);
		SPI_Data = nrf_read_reg(RF_SETUP);
		SPI_Data = nrf_read_reg(STATUS);
		if ((SPI_Data & MASK_RX_DR) == MASK_RX_DR) {
			rx_data = nrf_read_reg(R_RX_PAYLOAD);
			if (rx_data == 0xAB) {
				GPIO_SetBits(GPIOD, GPIO_Pin_14);
			} else if (rx_data == 0xCC) {
				GPIO_ResetBits(GPIOD, GPIO_Pin_14);
			}
			nrf_write_reg(STATUS,SPI_Data);
			nrf_write_reg(STATUS,MASK_TX_DS);
			nrf_write_reg(CONFIG,0x0B);
		}
		SPI_Data = nrf_read_reg(STATUS);
		SPI_Data = nrf_read_reg(OBSERVE_TX);
		taskYIELD();
//		SPI_Data = SPI_I2S_ReceiveData(SPI2); //Clear RXNE bit
//		SPI_I2S_SendData(SPI2, SPI_Data);
//		while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)){
//			taskYIELD();
//		};

		//return  SPI_I2S_ReceiveData(SPI2);

	}
}
void usart_init(void) {
	/* USARTx configured as follow:
	 - BaudRate = 115200 baud
	 - Word Length = 8 Bits
	 - One Stop Bit
	 - No parity
	 - Hardware flow control disabled (RTS and CTS signals)
	 - Receive and transmit enabled
	 */
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;

	/* Enable GPIO clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);

	/* Enable UART clock */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);

	/* Connect PXx to USARTx_Tx*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource5, GPIO_AF_USART2);

	/* Connect PXx to USARTx_Rx*/
	GPIO_PinAFConfig(GPIOD, GPIO_PinSource6, GPIO_AF_USART2);

	USART_OverSampling8Cmd(USART2, ENABLE);
	/* Configure USART Tx as alternate function  */
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function  */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

	/* USART configuration */
	USART_Init(USART2, &USART_InitStructure);

	/* Enable USART */
	USART_Cmd(USART2, ENABLE);

}
void packet_parser(uint8_t address, uint8_t cmd, uint8_t numofbytes, uint8_t *packet, uint8_t packet_crc) {
	uint8_t i;
	uint8_t crc = 0xFF;
	crc = crc8Table[crc ^ FEND];
	crc = crc8Table[crc ^ address];
	crc = crc8Table[crc ^ cmd];
	crc = crc8Table[crc ^ numofbytes];
	uint8_t val = 0;
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
			//send_acks = 1;
		}
		if(packet[0] == 0x51) { // Enable ACKs
			//send_acks = 0;
		}
	}
	if (cmd == 2) {
		val = nrf_read_reg(packet[0]);
	}
}
void vScanUsart(void *pvParameters) {
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
						//Изъять пакет, очистить переменныe
						packet_parser(wake_header[address], wake_cmd, wake_numofbytes, wake_data,wake_header[crc]);
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

int main(void) {
	RCC_HSEConfig(RCC_HSE_ON);
	while (!RCC_WaitForHSEStartUp()) {
	}
	usart_init();
	xTaskCreate( vLED, "vLED", configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
	xTaskCreate( vScanUsart, "vScanUsart", configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
	xTaskCreate( vSPI, "vSPI", configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
	vTaskStartScheduler();
	return 0;
}
