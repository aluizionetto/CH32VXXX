/*
 * i2c_com.h
 *
 *  Created on: Nov 16, 2024
 *      Author: aluiz
 */

#ifndef USER_I2C_COM_H_
#define USER_I2C_COM_H_

#include <ch32v00x.h>

void IIC_Init(u32 bound, u16 address);
void i2c_write_data(uint8_t address,uint8_t *d, uint8_t len, uint8_t stop);
void i2c_read_data (uint8_t address, uint8_t *d, uint8_t len);

#endif /* USER_I2C_COM_H_ */
