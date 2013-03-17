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

enum wake_packet { header = 0, address, command, num_bytes, data, crc};
const unsigned char
  FEND  = 0xC0,        // Frame END
  FESC  = 0xDB,        // Frame ESCape
  TFEND = 0xDC,        // Transposed Frame END
  TFESC = 0xDD;        // Transposed Frame ESCape
unsigned int usart_rx_buff[256], usart_rx_buff_temp;
uint8_t wake_packet_status = header;
uint8_t packet_started = 0;
uint8_t wake_cmd = 0;
uint8_t wake_data[256];
uint8_t wake_header[4];
uint8_t wake_data_iterator = 0;

const unsigned char crc8Table[256] = {
    0x00, 0x31, 0x62, 0x53, 0xC4, 0xF5, 0xA6, 0x97,
    0xB9, 0x88, 0xDB, 0xEA, 0x7D, 0x4C, 0x1F, 0x2E,
    0x43, 0x72, 0x21, 0x10, 0x87, 0xB6, 0xE5, 0xD4,
    0xFA, 0xCB, 0x98, 0xA9, 0x3E, 0x0F, 0x5C, 0x6D,
    0x86, 0xB7, 0xE4, 0xD5, 0x42, 0x73, 0x20, 0x11,
    0x3F, 0x0E, 0x5D, 0x6C, 0xFB, 0xCA, 0x99, 0xA8,
    0xC5, 0xF4, 0xA7, 0x96, 0x01, 0x30, 0x63, 0x52,
    0x7C, 0x4D, 0x1E, 0x2F, 0xB8, 0x89, 0xDA, 0xEB,
    0x3D, 0x0C, 0x5F, 0x6E, 0xF9, 0xC8, 0x9B, 0xAA,
    0x84, 0xB5, 0xE6, 0xD7, 0x40, 0x71, 0x22, 0x13,
    0x7E, 0x4F, 0x1C, 0x2D, 0xBA, 0x8B, 0xD8, 0xE9,
    0xC7, 0xF6, 0xA5, 0x94, 0x03, 0x32, 0x61, 0x50,
    0xBB, 0x8A, 0xD9, 0xE8, 0x7F, 0x4E, 0x1D, 0x2C,
    0x02, 0x33, 0x60, 0x51, 0xC6, 0xF7, 0xA4, 0x95,
    0xF8, 0xC9, 0x9A, 0xAB, 0x3C, 0x0D, 0x5E, 0x6F,
    0x41, 0x70, 0x23, 0x12, 0x85, 0xB4, 0xE7, 0xD6,
    0x7A, 0x4B, 0x18, 0x29, 0xBE, 0x8F, 0xDC, 0xED,
    0xC3, 0xF2, 0xA1, 0x90, 0x07, 0x36, 0x65, 0x54,
    0x39, 0x08, 0x5B, 0x6A, 0xFD, 0xCC, 0x9F, 0xAE,
    0x80, 0xB1, 0xE2, 0xD3, 0x44, 0x75, 0x26, 0x17,
    0xFC, 0xCD, 0x9E, 0xAF, 0x38, 0x09, 0x5A, 0x6B,
    0x45, 0x74, 0x27, 0x16, 0x81, 0xB0, 0xE3, 0xD2,
    0xBF, 0x8E, 0xDD, 0xEC, 0x7B, 0x4A, 0x19, 0x28,
    0x06, 0x37, 0x64, 0x55, 0xC2, 0xF3, 0xA0, 0x91,
    0x47, 0x76, 0x25, 0x14, 0x83, 0xB2, 0xE1, 0xD0,
    0xFE, 0xCF, 0x9C, 0xAD, 0x3A, 0x0B, 0x58, 0x69,
    0x04, 0x35, 0x66, 0x57, 0xC0, 0xF1, 0xA2, 0x93,
    0xBD, 0x8C, 0xDF, 0xEE, 0x79, 0x48, 0x1B, 0x2A,
    0xC1, 0xF0, 0xA3, 0x92, 0x05, 0x34, 0x67, 0x56,
    0x78, 0x49, 0x1A, 0x2B, 0xBC, 0x8D, 0xDE, 0xEF,
    0x82, 0xB3, 0xE0, 0xD1, 0x46, 0x77, 0x24, 0x15,
    0x3B, 0x0A, 0x59, 0x68, 0xFF, 0xCE, 0x9D, 0xAC
};

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
	//NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}

void SetupUSART() {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	//DMA_InitTypeDef DMA_InitStructure;
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
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
	USART_Cmd(USART1, ENABLE);

//	USART_DMACmd(USART1, USART_DMAReq_Rx, ENABLE);
//
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
//	DMA_DeInit(DMA1_Channel5);
//	DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)(&USART1->DR);
//	DMA_InitStructure.DMA_MemoryBaseAddr = usart_rx_buff;
//	DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
//	DMA_InitStructure.DMA_BufferSize = 8;
//	DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
//	DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
//	DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
//	DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Word;
//	DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
//	DMA_InitStructure.DMA_Priority = DMA_Priority_Low;
//	DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
//	DMA_Init(DMA1_Channel5, &DMA_InitStructure);
//
//	NVIC_EnableIRQ(DMA1_Channel5_IRQn);
//	DMA_ITConfig(DMA1_Channel5, DMA_IT_TC | DMA_IT_TE, ENABLE);
//	DMA_Cmd(DMA1_Channel5, ENABLE);
//
//	RCC_AHBPeriphClockCmd(RCC_AHBPeriph_CRC, ENABLE);
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

void usartSendByte(int cmd, uint16_t word) {
	uint8_t crc = 0xFF;
	uint8_t b1 = word & 0xFF;
	uint8_t b2 = word >> 8;
	crc = crc8Table[crc ^ b2];
	crc = crc8Table[crc ^ b1];
	usartSendChr(FEND); // FEND
	usartSendChr(200); // ADDRESS
	usartSendChr(cmd); // CMD
	usartSendChr(2); // N
	usartSendChr(b2);
	usartSendChr(b1);
	usartSendChr(crc); // CRC
}

void vPrintTime(void *pvParameters) {
	uint16_t adc;
	for (;;) {
		adc = ADC_GetConversionValue(ADC1);
		usartSendByte(51, adc);
		vTaskDelay(500);
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
		if(usart_rx_buff[0] == FEND) {
			wake_packet_status = header;
		} else if(wake_packet_status == header) {
			if (usart_rx_buff[0] == TFEND && usart_rx_buff_temp == FESC) {
				usart_rx_buff[0] = FEND;
			} else if (usart_rx_buff[0] == TFESC && usart_rx_buff_temp == FESC) {
				usart_rx_buff[0] = FESC;
			}
			usart_rx_buff_temp = usart_rx_buff[0];
			switch (wake_packet_status) {
				case address:
					wake_header[address] = usart_rx_buff[0];
					wake_packet_status = command;
					break;
				case command:
					wake_header[command] = usart_rx_buff[0];
					wake_packet_status = num_bytes;
					break;
				case num_bytes:
					wake_header[num_bytes] = usart_rx_buff[0];
					wake_packet_status = data;
					break;
				case data:
					wake_data[wake_data_iterator] = usart_rx_buff[0];
					wake_data_iterator++;
					if (wake_data_iterator == wake_header[num_bytes]) {
						wake_data_iterator = 0;
						wake_packet_status = 255;
						usart_rx_buff_temp = 0xFF;
						memset(&wake_data[0], 0, sizeof(wake_data));
					}
					break;

				default:
					break;
			}
		}
		DMA_ClearITPendingBit(DMA1_IT_TC5);

	} else if (DMA1->ISR & DMA_ISR_TEIF5) {
		DMA_ClearITPendingBit(DMA1_IT_TE5);
	}
}

unsigned int i;
void USART1_IRQHandler(void)
{
    if ((USART1->SR & USART_FLAG_RXNE) != (u16)RESET)
	{
    	usart_rx_buff[0] = USART_ReceiveData(USART1);
    	if(usart_rx_buff[0] == FEND) {
    				packet_started = 1;
    			} else if(packet_started == 1) {
    				if (usart_rx_buff[0] == TFEND && usart_rx_buff_temp == FESC) {
    					usart_rx_buff[0] = FEND;
    				} else if (usart_rx_buff[0] == TFESC && usart_rx_buff_temp == FESC) {
    					usart_rx_buff[0] = FESC;
    				}
    				usart_rx_buff_temp = usart_rx_buff[0];
    				switch (wake_packet_status) {
    					case header:
    						wake_header[address] = usart_rx_buff[0];
    						wake_packet_status = command;
    						break;
    					case command:
    						wake_header[command] = usart_rx_buff[0];
    						wake_packet_status = num_bytes;
    						break;
    					case num_bytes:
    						wake_header[num_bytes] = usart_rx_buff[0];
    						wake_packet_status = data;
    						break;
    					case data:
    						wake_data[wake_data_iterator] = usart_rx_buff[0];
    						wake_data_iterator++;
    						if (wake_data_iterator == wake_header[num_bytes]) {
    							wake_data_iterator = 0;
    							wake_packet_status = 255;
    							usart_rx_buff_temp = 0xFF;
    							memset(&wake_data[0], 0, sizeof(wake_data));
    						}
    						break;

    					default:
    						break;
    				}
    			}
    	}
    //USART_ClearITPendingBit(USART_)
}
