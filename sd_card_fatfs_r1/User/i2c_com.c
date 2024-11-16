/*
 * i2c_com.c
 *
 *  Created on: Nov 16, 2024
 *      Author: Aluizio d'Affonsêca Netto
 */
#include "i2c_com.h"
#include <ch32v00x.h>

void IIC_Init(u32 bound, u16 address) {
	GPIO_InitTypeDef GPIO_InitStructure = { 0 };
	I2C_InitTypeDef I2C_InitTSturcture = { 0 };

	RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOC | RCC_APB2Periph_AFIO, ENABLE);
	RCC_APB1PeriphClockCmd( RCC_APB1Periph_I2C1, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2; //SCL
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1; //SDA
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_OD;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init( GPIOC, &GPIO_InitStructure);

	I2C_InitTSturcture.I2C_ClockSpeed = bound;
	I2C_InitTSturcture.I2C_Mode = I2C_Mode_I2C;
	I2C_InitTSturcture.I2C_DutyCycle = I2C_DutyCycle_2;
	I2C_InitTSturcture.I2C_OwnAddress1 = (address << 1);
	I2C_InitTSturcture.I2C_Ack = I2C_Ack_Enable;
	I2C_InitTSturcture.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
	I2C_Init( I2C1, &I2C_InitTSturcture);

	I2C_Cmd( I2C1, ENABLE);

	I2C_AcknowledgeConfig( I2C1, ENABLE);
}

void i2c_write_data(uint8_t address,uint8_t *d, uint8_t len, uint8_t stop) {
	uint8_t k;
	while( I2C_GetFlagStatus( I2C1, I2C_FLAG_BUSY ) != RESET );
	I2C_GenerateSTART( I2C1, ENABLE);

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT  ) );
	I2C_Send7bitAddress( I2C1, (address)<<1, I2C_Direction_Transmitter);
	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED  ) );

	for (k = 0; k < len; k++) {
		I2C_SendData( I2C1,*d++);
		while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_TRANSMITTED ));
	}

	if (stop)
		I2C_GenerateSTOP( I2C1, ENABLE);
}

void i2c_read_data (uint8_t address, uint8_t *d, uint8_t len) {
	uint8_t k;

	I2C_GenerateSTART( I2C1, ENABLE );

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_MODE_SELECT ) );
	I2C_Send7bitAddress( I2C1, (address)<<1, I2C_Direction_Receiver );

	while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED ) );


	for (k = 0; k < len; k++) {
		while( !I2C_CheckEvent( I2C1, I2C_EVENT_MASTER_BYTE_RECEIVED ) );
		*d++ = I2C_ReceiveData( I2C1);
	}
	I2C_GenerateSTOP( I2C1, ENABLE);
}


