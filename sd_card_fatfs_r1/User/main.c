/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2022/08/08
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *Multiprocessor communication mode routine:
 *Master:USART1_Tx(PD5)\USART1_Rx(PD6).
 *This routine demonstrates that USART1 receives the data sent by CH341 and inverts
 *it and sends it (baud rate 115200).
 *
 *Hardware connection:PD5 -- Rx
 *                     PD6 -- Tx
 *
 */

#include "debug.h"
#include "hw_spi\hw_spi.h"
#include "ff/ff.h"		/* Declarations of FatFs API */
#include "ff/diskio.h"
#include "rtc_ds1307.h"

/* Global define */


/* Global Variable */
FATFS FatFs;		/* FatFs work area needed for each volume */
FIL Fil;           /*Estruta para manipular arquivo*/

volatile uint16_t adc_value;
volatile int flag_adc = 0;
volatile uint16_t count_adc = 0;

char buff_msg[64];  //buffer para texto
//buffer para comando de serial
#define LEN_BUFF_SERIAL_IN (32)
char buff_serial_in[LEN_BUFF_SERIAL_IN]; // buufer para recepção da serial
volatile int8_t buff_serial_idx = 0;
volatile uint16_t flag_cmd = 0;

/*********************************************************************
 * @fn      USARTx_CFG
 *
 * @brief   Initializes the USART2 & USART3 peripheral.
 *
 * @return  none
 */
void USARTx_CFG(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure = {0};
	USART_InitTypeDef USART_InitStructure = {0};
	NVIC_InitTypeDef  NVIC_InitStructure = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD | RCC_APB2Periph_USART1, ENABLE);

	/* USART1 TX-->D.5   RX-->D.6 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	USART_InitStructure.USART_BaudRate = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	USART_InitStructure.USART_Parity = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode = USART_Mode_Tx | USART_Mode_Rx;

	USART_Init(USART1, &USART_InitStructure);

	//Ativa e configura interrupção da porta serial
	USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);

	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	USART_Cmd(USART1, ENABLE);
}

void USART_char(char c) {
	while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET);
	USART_SendData(USART1, c);
}
void USART_str(char *d) {

	while (*d != '\0') {
		USART_char(*d++);
	}
}

void USART_int(int32_t num)
{
	uint32_t g;
	uint32_t div = 1000000000;
	int8_t s0 = 0;
	if (num < 0)
	{
		USART_char('-');
		num = -1*num;
	}

	if (num == 0)
	{
		USART_char('0');
	}
	else
	{
		for (g = 0 ; g < 10; g++)
		{
			if (num >= div)
			{
				s0 = 1;
				USART_char((num/div) + '0');
				num %= div;
			}
			else if(s0)
			{
				USART_char('0');
			}
			div /=10;
		}
	}
}

void conv_int2str (char *bf,uint32_t num, uint8_t cat) {
	uint32_t g;
	uint32_t div = 1000000000;
	int8_t s0 = 0;

	if (cat) {
		while (*bf) bf++;
	}

	if (num < 0)
	{
		*bf++ = '-';
		*bf = '\0';
		num = -1*num;
	}

	if (num == 0)
	{
		*bf++ = '0';
		*bf = '\0';
	}
	else
	{
		for (g = 0 ; g < 10; g++)
		{
			if (num >= div)
			{
				s0 = 1;
				*bf++ = (num/div) + '0';
				*bf = '\0';
				num %= div;
			}
			else if(s0)
			{
				*bf++ = '0';
				*bf = '\0';
			}
			div /=10;
		}
	}
}
void str_cat(char *bf, char *cat) {
	while (*bf) bf++;
	while (*cat) *bf++ = *cat++;
	*bf = '\0';
}

uint32_t str_len(char *bf) {
	uint32_t l=0;
	while (*bf++) l++;

	return l;
}

int32_t conv_strtoint(char *bf){
	int32_t n = 0;
	uint32_t k;

	for (k = 0; (bf[k] != '\0') && (k < 10);k++) {
		if ((k == 0) && (bf[k] == '-')) {
			n = -n;
			continue;
		}
		if ((bf[k] >= '0') && (bf[k] <= '9')) {
			n = 10*n + (bf[k] - '0');
		}
	}
	return n;
}

void conv_rtc2str(char *bf, uint8_t cat){
	conv_int2str(bf,time_rtc.dayOfMonth,cat);
	str_cat(bf,"/");
	conv_int2str(bf,time_rtc.month,1);
	str_cat(bf,"/");
	conv_int2str(bf,time_rtc.year,1);
	str_cat(bf," ");
	conv_int2str(bf,time_rtc.hour,1);
	str_cat(bf,":");
	conv_int2str(bf,time_rtc.minute,1);
	str_cat(bf,":");
	conv_int2str(bf,time_rtc.second,1);
}


void USART1_IRQHandler(void) __attribute__((interrupt));
void USART1_IRQHandler(void)
{
	char c_in;
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
	{
		c_in = USART_ReceiveData(USART1);

		if (c_in == '\n') return;  //desconsidera caracter

		if (c_in == '\r') {
			buff_serial_idx = 0;
			flag_cmd = 1;
			return;
		}

		buff_serial_in[buff_serial_idx] = c_in;
		buff_serial_idx++;
		if(buff_serial_idx > (LEN_BUFF_SERIAL_IN-1)) buff_serial_idx = LEN_BUFF_SERIAL_IN-1;
		buff_serial_in[buff_serial_idx] = '\0';

	}
}

void processa_cmd(void) {

	int8_t n_arg = 0;
	char *p_arg[10];
	char *p = buff_serial_in;

	p_arg[n_arg++] = p;
	while (*p != '\0') {
		if (*p == ' ' || *p ==',' || *p ==';') {
			*p++ = '\0';
			p_arg[n_arg++] = p;
		}
		else p++;
	}
	//USART_str(p_arg[0]);
	//USART_int(n_arg);

	//acerta relogio
	if (*p_arg[0] == 't' && (n_arg > 6)) {
		DS1307_stopClock();
		time_rtc.dayOfMonth = conv_strtoint(p_arg[1]);
		time_rtc.month = conv_strtoint(p_arg[2]);
		time_rtc.year = conv_strtoint(p_arg[3]);
		time_rtc.hour = conv_strtoint(p_arg[4]);
		time_rtc.minute = conv_strtoint(p_arg[5]);
		time_rtc.second = conv_strtoint(p_arg[6]);
		DS1307_setTime();
		DS1307_startClock();

		USART_str("RTC_AJUSTE OK\n\r");
	}

}




void ADC_Function_Init(void)
{
	ADC_InitTypeDef  ADC_InitStructure = {0};
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);
	RCC_ADCCLKConfig(RCC_PCLK2_Div2);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	ADC_DeInit(ADC1);
	ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfChannel = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 1, ADC_SampleTime_241Cycles);
	ADC_Calibration_Vol(ADC1, ADC_CALVOL_50PERCENT);
	ADC_AutoInjectedConvCmd(ADC1, ENABLE);
	ADC_Cmd(ADC1, ENABLE);

	ADC_ResetCalibration(ADC1);
	while(ADC_GetResetCalibrationStatus(ADC1));
	ADC_StartCalibration(ADC1);
	while(ADC_GetCalibrationStatus(ADC1));

	//habilita interrupção do AD
	ADC_ITConfig(ADC1,ADC_IT_EOC,ENABLE);
	NVIC_InitStruct.NVIC_IRQChannel = ADC_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
}


void TIM2_INT_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIMBase_InitStruct;
	NVIC_InitTypeDef NVIC_InitStruct;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

	TIMBase_InitStruct.TIM_Period = arr;
	TIMBase_InitStruct.TIM_CounterMode = TIM_CounterMode_Up;
	TIMBase_InitStruct.TIM_Prescaler = psc;
	TIMBase_InitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInit(TIM2, &TIMBase_InitStruct);

	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);

	NVIC_InitStruct.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStruct.NVIC_IRQChannelPreemptionPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStruct);
	TIM_Cmd(TIM2, ENABLE);
}

//interrupção do tiemer 2
void TIM2_IRQHandler() __attribute__((interrupt));
void TIM2_IRQHandler(void) {
	if (TIM_GetITStatus(TIM2, TIM_IT_Update) == SET) {
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
		//Start no conversor
		ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	}
}


//interrupção do ADC
void ADC1_IRQHandler()  __attribute__((interrupt));
void ADC1_IRQHandler(void) {
	if (ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC) == SET) {
		adc_value = ADC_GetConversionValue(ADC1);
		flag_adc = 1;
	}
}

void LED_OFF(void) {
	GPIOD->BSHR = GPIO_Pin_4;
}
void LED_ON (void) {
	GPIOD->BCR = GPIO_Pin_4;
}
void Init_GPIOLED(void) {
	GPIO_InitTypeDef  GPIO_InitStructure = {0};

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);

	/* LED NO PD4 */
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(GPIOD, &GPIO_InitStructure);

	LED_OFF();
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
	//variaveis para controle ff
	UINT bw;
	FRESULT fr;
	uint32_t n_sec;


	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	SystemCoreClockUpdate();
	Delay_Init();
#if (SDI_PRINT == SDI_PR_OPEN)
	SDI_Printf_Enable();
#else
	//USART_Printf_Init(115200);
#endif
	USARTx_CFG();
	ADC_Function_Init(); //configura ADC A2 - PC4
	Init_GPIOLED();
	RTC_Init();
	DS1307_startClock();
	DS1307_getTime();
	TIM2_INT_Init(1000-1,48000-1);//interrupção do timer 2 para 1s



	//printf("SystemClk:%d\r\n",SystemCoreClock);
	//printf( "ChipID:%08x\r\n", DBGMCU_GetCHIPID() );
	USART_str("Data Log ADC\n\r");
	USART_str("SystemClk:");
	USART_int(SystemCoreClock);
	USART_str("\r\n");


	// Enable SPI
	SPI_init();
	SPI_begin_8();

	Delay_Ms( 500 );


	//monta unidade
	fr = f_mount(&FatFs, "", 1);		/* Give a work area to the default drive */

	//tamanho da unidade
	disk_ioctl(FatFs.pdrv,GET_SECTOR_COUNT,(void *)&n_sec);
	USART_str("SD len:");
	USART_int(n_sec>>1);  //cada sector de 512 bytes
	USART_str(" kB\r\n");


	while(1)
	{
		//amostra disponível do conversor ADC
		if(flag_adc) {
			DS1307_getTime();  //atuliza time_rtc
			//monta texto
			buff_msg[0] = '\0';
			str_cat(buff_msg,"AD[");
			conv_rtc2str(buff_msg,1);
			str_cat(buff_msg,"];");
			conv_int2str(buff_msg,count_adc,1);
			str_cat(buff_msg,";");
			conv_int2str(buff_msg,adc_value,1);
			str_cat(buff_msg,"\r\n");

			//envia pela serial
			USART_str(buff_msg);

			//grava em arquivo
			LED_ON();
			fr = f_open(&Fil, "newfile.txt", FA_WRITE | FA_OPEN_APPEND);	/* Create a file */
			if (fr == FR_OK) {
				f_write(&Fil, buff_msg, str_len(buff_msg), &bw);	/* Write data to the file */
				fr = f_close(&Fil);							/* Close the file */
				if (fr == FR_OK && bw == str_len(buff_msg)) {		/* Arquivo Escrito com sucesso */
					USART_str(" >OK\n\r");
					LED_OFF();
				}
				else
				{
					USART_str(" >ERRO 1\n\r");
				}
			}
			else
			{
				USART_str(" >ERRO 2\n\r");
			}

			count_adc++;
			flag_adc = 0;
		}
		//comando disponível
		if (flag_cmd) {

			USART_str(buff_serial_in);
			processa_cmd();

			flag_cmd = 0;
		}


	}
}
