#include "stm32f10x.h"
#include "stm32f10x_dma.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_crc.h"
#include "stm32f10x_flash.h"
#include "misc.h"
#include "FreeRTOS.h"
#include "task.h"
#include <string.h>

#define SOH 1 // Start of heading
#define EOT 4 // End of transmission
#define ACK 6 //
#define NAK 21 // Negative ACK

int usart_rx_buff[8];

void itoa(int n, char s[]) {
	int i, j, k, sign;
	char c;
	if ((sign = n) < 0)
		n = -n;
	i = 0;
	do {
		s[i++] = n % 10 + '0';
	} while ((n /= 10) > 0);
	if (sign < 0)
		s[i++] = '-';
	s[i] = '\0';
	for (k = 0, j = strlen(s) - 1; k < j; k++, j--) {
		c = s[k];
		s[k] = s[j];
		s[j] = c;
	}
}

/***************************************************************************//**
 * @brief Init USART1
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

void NVIC_Configuration(void) {
	NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM
	/* Set the Vector Table base location at 0x20000000 */
	NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
	/* Set the Vector Table base location at 0x08000000 */
	NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
	/* Enable DMA1 channel6 IRQ */
	NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void SetupUSART() {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	DMA_InitTypeDef DMA_InitStructure;
	RCC_APB2PeriphClockCmd(
			RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO,
			ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 9600;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl =
			USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);

	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
	DMA_DeInit(DMA1_Channel5);
	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
	DMA_InitStructure.DMA_MemoryBaseAddr = usart_rx_buff;
	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
	DMA_InitStructure.DMA_BufferSize = sizeof(usart_rx_buff)/4;
	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
	DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
	DMA_Init(DMA1_Channel5, &DMA_InitStructure);

	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC | DMA_IT_TE, ENABLE);
	DMA_Cmd(DMA1_Channel5, ENABLE);

	 RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
}

void SetupADC() {
	GPIO_InitTypeDef GPIO_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	RCC_ADCCLKConfig(RCC_PCLK2_Div8);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;

	ADC_RegularChannelConfig(ADC1, ADC_Channel_17, 1,
			ADC_SampleTime_239Cycles5);
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while (ADC_GetResetCalibrationStatus(ADC1))
		;
	ADC_StartCalibration(ADC1);
	while (ADC_GetCalibrationStatus(ADC1))
		;

	ADC_Cmd(ADC1, ENABLE);
	ADC_SoftwareStartConvCmd(ADC1, ENABLE);
}

void SetupLED() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

void SetupRangeMeter() {
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
}

void usartSendChr(uint16_t data) {
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
	USART_SendData(USART1, data);
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET)
		;
}

void usartSendStr(char *str) {
	int i = 0;
	while (str[i] != 0) {
		usartSendChr(str[i++]);
	}
}
char dst[256];
unsigned int usartSendPacket(int cmd, char *data) {
	//uint32_t crc = CRC_CalcBlockCRC(data,11);
	short j = 0;
	//for (j = 2; j < 11; j++) {
	//	packet[j] = data[j-2];
	//}
	usartSendChr(SOH);
	usartSendChr(sizeof(data)/sizeof(*dst));
	for (j = 0; j < sizeof(data); j++) {
		usartSendChr(data[j]);
	}
	usartSendStr("\n");
	//usartSendChr(EOT);
	return 0;

}

void usartSendByte(int cmd, uint16_t word) {
	uint8_t b1 = word & 0xFF;
	uint8_t b2 = word >> 8;
	usartSendChr(0xC0);
	usartSendChr(5);
	usartSendChr(51);
	usartSendChr(b2);
	usartSendChr(b1);

	//usartSendChr(EOT);

}

void vPrintTime(void *pvParameters) {
	uint16_t adc;

	for (;;) {
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		vTaskDelay(500);
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
		vTaskDelay(500);
		adc = ADC_GetConversionValue(ADC1);
		//itoa(adv, dst);
		usartSendByte(50, adc);
	}
}

void vPrintTemp(void *pvParameters) {
	for (;;) {
		GPIO_SetBits(GPIOC, GPIO_Pin_9);
		vTaskDelay(321);
		GPIO_ResetBits(GPIOC, GPIO_Pin_9);
		vTaskDelay(321);
	}
}

int main(void) {
	SetSysClockTo24();
	NVIC_Configuration();
	SetupUSART();
	SetupLED();
	SetupADC();

	xTaskCreate( vPrintTime, ( signed char * ) "vPrintTime", configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
	xTaskCreate( vPrintTemp, ( signed char * ) "vPrintTemp", configMINIMAL_STACK_SIZE, NULL, 0, ( xTaskHandle * ) NULL);
	vTaskStartScheduler();
	return 0;
}

void DMA1_Channel5_IRQHandler(void) {
	if (DMA1->ISR & DMA_ISR_TCIF5) {
		DMA_ClearITPendingBit(DMA1_IT_TC5);
	}

	if (DMA1->ISR & DMA_ISR_TEIF5) {
		DMA_ClearITPendingBit(DMA1_IT_TE5);
	}
}

