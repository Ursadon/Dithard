#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_spi.h>
#include "FreeRTOS.h"
#include "task.h"
#include "nrf24l01_registers.h"

//Chip Enable Activates RX or TX mode
#define CE_H   GPIO_SetBits(GPIOD, GPIO_Pin_10)
#define CE_L   GPIO_ResetBits(GPIOD, GPIO_Pin_10)

//SPI Chip Select
#define CSN_H  GPIO_SetBits(GPIOD, GPIO_Pin_9)
#define CSN_L  GPIO_ResetBits(GPIOD, GPIO_Pin_9)

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
//		SPI_Data = SPI_I2S_ReceiveData(SPI2); //Clear RXNE bit
//		SPI_I2S_SendData(SPI2, SPI_Data);
//		while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)){
//			taskYIELD();
//		};

		//return  SPI_I2S_ReceiveData(SPI2);

	}
}
int main(void) {
	RCC_HSEConfig(RCC_HSE_ON);
	while (!RCC_WaitForHSEStartUp()) {
	}

	xTaskCreate( vLED, "vLED", configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
	xTaskCreate( vSPI, "vSPI", configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
	vTaskStartScheduler();
	return 0;
}

//void SPI2_IRQHandler (void) {
//	if (SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==SET) {
//		// Прерывание вызвано приемом байта ?
//		uint8_t data = SPI2->DR; //Читаем то что пришло
//		GPIOC->ODR ^= (GPIO_Pin_9 | GPIO_Pin_8); //Инвертируем состояние светодиодов
//		//SPI2->DR = data; //И отправляем обратно то что приняли
//	}
//}
