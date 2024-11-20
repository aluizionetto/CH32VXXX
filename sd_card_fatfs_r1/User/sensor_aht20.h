/*
 * sensor_aht20.h
 *
 *  Created on: Nov 20, 2024
 *      Author: Aluizio d'Affonsêca Netto
 */

#ifndef USER_SENSOR_AHT20_H_
#define USER_SENSOR_AHT20_H_

#include <ch32v00x.h>

//temperatura e humidade multiplicados por 10
void AHT20_Read(int32_t* Temp, int32_t* Humid);


#endif /* USER_SENSOR_AHT20_H_ */
