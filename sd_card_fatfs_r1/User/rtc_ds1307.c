/*
 * rtc_ds1307.c
 *
 *  Created on: Nov 15, 2024
 *      Author: Aluizio d' Affonsêca Netto
 */

#include "rtc_ds1307.h"
#include "i2c_com.h"
#include <ch32v00x.h>

#define DS1307_I2C_ADDRESS (0x68)

//estrutura para registro de tempo
Time_RTC time_rtc;

void RTC_Init(void) {

	IIC_Init(400000,(DS1307_I2C_ADDRESS));
}


uint8_t DS1307_decToBcd(uint8_t val) {
	return ((val / 10 * 16) + (val % 10));
}

//Convert binary coded decimal to normal decimal numbers
uint8_t DS1307_bcdToDec(uint8_t val) {
	return ((val / 16 * 10) + (val % 16));
}


/*Function: The clock timing will start */
void DS1307_startClock(void) {      // set the ClockHalt bit low to start the rtc

	uint8_t buff_d[2];
	// Register 0x00 holds the oscillator start/stop bit
	buff_d[0] = 0;
	i2c_write_data(DS1307_I2C_ADDRESS,buff_d,1,1);

	// save actual seconds and AND sec with bit 7 (sart/stop bit) = clock started
	i2c_read_data(DS1307_I2C_ADDRESS,buff_d,1);
	time_rtc.second = buff_d[0] & 0x7f;

	// write seconds back and start the clock
	buff_d[0] = 0x00;
	buff_d[1] = time_rtc.second;
	i2c_write_data(DS1307_I2C_ADDRESS,buff_d,2,1);


}
/*Function: The clock timing will stop */
void DS1307_stopClock(void) {       // set the ClockHalt bit high to stop the rtc
	uint8_t buff_d[2];
	buff_d[0] = 0x00;
	i2c_write_data(DS1307_I2C_ADDRESS,buff_d,1,1);  // Register 0x00 holds the oscillator start/stop bit

	i2c_read_data(DS1307_I2C_ADDRESS,buff_d,1);
	time_rtc.second = buff_d[0] | 0x80; // save actual seconds and OR sec with bit 7 (sart/stop bit) = clock stopped

	buff_d[0] = 0x00;           // write seconds back and stop the clock
	buff_d[1] = time_rtc.second;
	i2c_write_data(DS1307_I2C_ADDRESS,buff_d,2,1);

}
/****************************************************************/
/*Function: Read time and date from RTC */
void DS1307_getTime() {

	uint8_t buff_d[7];
	// Register 0x00 holds the oscillator start/stop bit
	buff_d[0] = 0;
	i2c_write_data(DS1307_I2C_ADDRESS,buff_d,1,1);

	i2c_read_data(DS1307_I2C_ADDRESS,buff_d,7);

	// A few of these need masks because certain bits are control bits
	time_rtc.second     = DS1307_bcdToDec(buff_d[0] & 0x7f);
	time_rtc.minute     = DS1307_bcdToDec(buff_d[1]);
	time_rtc.hour       = DS1307_bcdToDec(buff_d[2] & 0x3f);// Need to change this if 12 hour am/pm
	time_rtc.dayOfWeek  = DS1307_bcdToDec(buff_d[3]);
	time_rtc.dayOfMonth = DS1307_bcdToDec(buff_d[4]);
	time_rtc.month      = DS1307_bcdToDec(buff_d[5]);
	time_rtc.year       = DS1307_bcdToDec(buff_d[6]);
}
/*******************************************************************/
/*Frunction: Write the time that includes the date to the RTC chip */
void DS1307_setTime() {
	uint8_t buff_d[8];

	buff_d[0] = (uint8_t)0x00;
	buff_d[1] =DS1307_decToBcd(time_rtc.second);// 0 to bit 7 starts the clock
	buff_d[2] =DS1307_decToBcd(time_rtc.minute);
	buff_d[3] =DS1307_decToBcd(time_rtc.hour);  // If you want 12 hour am/pm you need to set bit 6
	buff_d[4] =DS1307_decToBcd(time_rtc.dayOfWeek);
	buff_d[5] =DS1307_decToBcd(time_rtc.dayOfMonth);
	buff_d[6] =DS1307_decToBcd(time_rtc.month);
	buff_d[7] =DS1307_decToBcd(time_rtc.year);

	i2c_write_data(DS1307_I2C_ADDRESS,buff_d,8,1);

}
void DS1307_fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second) {
	// assign variables
	time_rtc.hour = _hour;
	time_rtc.minute = _minute;
	time_rtc.second = _second;
}
void DS1307_fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day) {
	time_rtc.year = _year - 2000;
	time_rtc.month = _month;
	time_rtc.dayOfMonth = _day;
}
void DS1307_fillDayOfWeek(uint8_t _dow) {
	time_rtc.dayOfWeek = _dow;
}



