#include <stm32f4xx.h>
#include <stm32f4xx_gpio.h>
#include <stm32f4xx_rcc.h>
#include <stm32f4xx_spi.h>
#include "FreeRTOS.h"
#include "task.h"

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
		vTaskDelay(500);
		GPIO_SetBits(GPIOD, GPIO_Pin_13);
		vTaskDelay(500);
		GPIO_SetBits(GPIOD, GPIO_Pin_14);
		vTaskDelay(500);
		GPIO_SetBits(GPIOD, GPIO_Pin_15);
		vTaskDelay(500);
		GPIO_ResetBits(GPIOD,
				GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15);
		vTaskDelay(500);
	}
}

void Out_Data_SPI2(uint8_t SPI_Data)
{
	SPI_Cmd(SPI2, ENABLE);
	SPI_SSOutputCmd(SPI2,DISABLE);
	SPI_I2S_SendData(SPI2, SPI_Data);
	SPI_SSOutputCmd(SPI2,ENABLE);
	SPI_Cmd(SPI2, DISABLE);
}
void vSPI(void *pvParameters) {
	uint8_t SPI_Data = 0x80;

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
	SPI_InitTypeDefStruct.SPI_Mode			= SPI_Mode_Slave;
	SPI_InitTypeDefStruct.SPI_DataSize		= SPI_DataSize_8b;
	SPI_InitTypeDefStruct.SPI_NSS			= SPI_NSS_Hard;
	SPI_InitTypeDefStruct.SPI_FirstBit		= SPI_FirstBit_MSB;
	SPI_InitTypeDefStruct.SPI_CPOL			= SPI_CPOL_Low;
	SPI_InitTypeDefStruct.SPI_CPHA			= SPI_CPHA_1Edge;
	SPI_Init(SPI2, &SPI_InitTypeDefStruct);
	SPI_Cmd(SPI2, ENABLE);
//
//	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
//	SPI_I2S_SendData(SPI2, 0x80);
//	while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE));
	//Out_Data_SPI2(0x05);
	for (;;) {
		//GPIO_ResetBits(GPIOE, GPIO_Pin_3);
		//adress = 0x80 | adress;

//		while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)) {
//			taskYIELD();
//		};
//		SPI_I2S_SendData(SPI2, adress);
		while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE)){
			taskYIELD();
		};
		SPI_Data = SPI_I2S_ReceiveData(SPI2); //Clear RXNE bit
		SPI_I2S_SendData(SPI2, SPI_Data);
		while(!SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE)){
			taskYIELD();
		};
		//GPIO_SetBits(GPIOE, GPIO_Pin_3);

		//return  SPI_I2S_ReceiveData(SPI2);

	}
}
int main(void) {
	RCC_HSEConfig(RCC_HSE_ON);
	while (!RCC_WaitForHSEStartUp()) {
	}

	xTaskCreate( vLED, ( signed char * ) "vLED", configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
	xTaskCreate( vSPI, ( signed char * ) "vSPI", configMINIMAL_STACK_SIZE,
			NULL, 0, ( xTaskHandle * ) NULL);
	vTaskStartScheduler();
	return 0;
}

void SPI2_IRQHandler (void) {
	if (SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE)==SET) {
		// Прерывание вызвано приемом байта ?
		uint8_t data = SPI2->DR; //Читаем то что пришло
		GPIOC->ODR ^= (GPIO_Pin_9 | GPIO_Pin_8); //Инвертируем состояние светодиодов
		//SPI2->DR = data; //И отправляем обратно то что приняли
	}
}
