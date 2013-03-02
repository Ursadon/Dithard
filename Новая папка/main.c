#include "stm32f10x.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_adc.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_flash.h"
#include "stm32f10x_tim.h"
#include "stm32f10x_i2c.h"
#include "FreeRTOS.h"
#include "task.h"
#include "drivers/lcd_nokia5110.h"
#include "drivers/hd44780_driver.h"
#include <string.h>

void itoa(int n, char s[]) {
	int i,j,k, sign;
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

void SetupUSART() {
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
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
	USART_InitStructure.USART_HardwareFlowControl
			= USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
	USART_Cmd(USART1, ENABLE);
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

	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_239Cycles5);
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

void usartSendChr(char data) {
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

void vPrintTime(void *pvParameters) {

	for (;;) {
		GPIO_SetBits(GPIOC, GPIO_Pin_8);
		vTaskDelay(500);
		GPIO_ResetBits(GPIOC, GPIO_Pin_8);
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

void vReadDistance(void *pvParameters) {
	int distance = 0;
	char dst[5];
	for (;;) {
		GPIO_SetBits(GPIOB, GPIO_Pin_13);
		vTaskDelay(1);
		GPIO_ResetBits(GPIOB, GPIO_Pin_13);

		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 0) {
			asm("nop");
		}
		while (GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_12) == 1) {
			distance++;
			if(distance == 23200) break;
		}
		distance = distance / 68;
		itoa(distance, dst);
		distance = 0;

		usartSendStr("[USR] Measuered length:");


		usartSendStr(dst);
		usartSendStr("\r\n");
		vTaskDelay(500);
	}
}

int main(void) {
	//SetSysClockTo24();
	SetupUSART();
	usartSendStr("[KERN] USART driver loaded\r\n");
	SetupLED();
	usartSendStr("[KERN] LED driver loaded\r\n");
	SetupADC();
	usartSendStr("[KERN] ADC driver loaded\r\n");
	SetupRangeMeter();
	usartSendStr("[KERN] RangeMeter driver loaded\r\n");
	hd44780_init();
	usartSendStr("[KERN] HD44780 driver loaded\r\n");

	uint8_t user_char[8]; 
	user_char[0]=0b00000;
	user_char[1]=0b11011;
	user_char[2]=0b10101;
	user_char[3]=0b10001;
	user_char[4]=0b01010;
	user_char[5]=0b00100;
	user_char[6]=0b00000;
	user_char[7]=0b00000;
	hd44780_set_user_char(0,&user_char);

	hd44780_set_xy(0,0);
	hd44780_out("  ���� + ��� = ");
	hd44780_set_xy(6,1);
	hd44780_send(0,DATA);
	hd44780_send(0,DATA);
	hd44780_send(0,DATA);

	hd44780_set_state(LCD_ENABLE, CURSOR_DISABLE, NO_BLINK); 
	xTaskCreate( vPrintTime, ( signed char * ) "vPrintTime", configMINIMAL_STACK_SIZE, NULL, 0,
			( xTaskHandle * ) NULL);
	xTaskCreate( vPrintTemp, ( signed char * ) "vPrintTemp", configMINIMAL_STACK_SIZE, NULL, 0,
			( xTaskHandle * ) NULL);
	//xTaskCreate( vReadDistance, ( signed char * ) "vReadDistance", configMINIMAL_STACK_SIZE, NULL, 0,
	//		( xTaskHandle * ) NULL);
	usartSendStr("[KERN] System loaded, starting sheduler...\r\n");
	vTaskStartScheduler();
}

