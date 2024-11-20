/*
 * sensor_aht20.c
 *
 *  Created on: Nov 20, 2024
 *      Author: Aluizio d'Affonsêca Netto
 */

#include "debug.h"
#include <ch32v00x.h>
#include "i2c_com.h"


#define AHT_ADDR	(0x38)

void AHT20_Read(int32_t* Temp, int32_t* Humid)
{
	uint8_t dum[6];
	dum[0] = 0x71;
	i2c_write_data(AHT_ADDR,dum,1,0);
	i2c_read_data(AHT_ADDR,dum,1);
	//HAL_I2C_Mem_Read(&HI2C, AHT_ADDR, 0x71, 1, dum, 1, 100);

	if(!(dum[0]&(1<<3)))
	{
		dum[0] = 0xBE, dum[1] = 0x08, dum[2] = 0x00;
		i2c_write_data(AHT_ADDR,dum,3,1);
		//HAL_I2C_Master_Transmit(&HI2C, AHT_ADDR, dum, 3, 100);
		Delay_Ms(10);
	}

	dum[0] = 0xAC, dum[1] = 0x33, dum[2] = 0x00;
	i2c_write_data(AHT_ADDR,dum,3,1);
	//HAL_I2C_Master_Transmit(&HI2C, AHT_ADDR, dum, 3, 100);
	Delay_Ms(80);

	do {
		dum[0] = 0x71;
		i2c_write_data(AHT_ADDR,dum,1,0);
		i2c_read_data(AHT_ADDR,dum,1);
		//HAL_I2C_Mem_Read(&HI2C, AHT_ADDR, 0x71, 1, dum, 1, 100);
		Delay_Ms(1);
	} while(dum[0]&(1<<7));

	i2c_read_data(AHT_ADDR,dum,6);
	//HAL_I2C_Master_Receive(&HI2C, AHT_ADDR, dum, 6, 100);
	uint32_t h20 = (dum[1])<<12 | (dum[2])<<4 | (dum[3]>>4);
	uint32_t t20 = (dum[3]&0x0F)<<16 | (dum[4])<<8 | dum[5];
	//*Temp = (int32_t)((t20 / 1048576.0)*2000.0 - 500.0);  //x10
	//*Humid = (int32_t)(h20 / 1048.576);   //x10
	*Temp = t20/524 - 500;
	*Humid = h20/1049;
}


