/*
 * rtc_ds1307.h
 *
 *  Created on: Nov 15, 2024
 *      Author: Aluizio d'Affonsêca Netto
 */

#ifndef USER_RTC_DS1307_H_
#define USER_RTC_DS1307_H_

#include <ch32v00x.h>

typedef struct  {
	uint8_t second;
	uint8_t minute;
	uint8_t hour;
	uint8_t dayOfWeek;// day of week, 1 = Monday
	uint8_t dayOfMonth;
	uint8_t month;
	uint16_t year;
}Time_RTC;

extern Time_RTC time_rtc;

#define MON 1
#define TUE 2
#define WED 3
#define THU 4
#define FRI 5
#define SAT 6
#define SUN 7


void RTC_Init(void) ;

uint8_t DS1307_decToBcd(uint8_t val);

//Convert binary coded decimal to normal decimal numbers
uint8_t DS1307_bcdToDec(uint8_t val);

/*Function: The clock timing will start */
void DS1307_startClock(void);
/*Function: The clock timing will stop */
void DS1307_stopClock(void);
/****************************************************************/
/*Function: Read time and date from RTC */
void DS1307_getTime(void);
/*******************************************************************/
/*Frunction: Write the time that includes the date to the RTC chip */
void DS1307_setTime(void);
void DS1307_fillByHMS(uint8_t _hour, uint8_t _minute, uint8_t _second);
void DS1307_fillByYMD(uint16_t _year, uint8_t _month, uint8_t _day);
void DS1307_fillDayOfWeek(uint8_t _dow);



#endif /* USER_RTC_DS1307_H_ */
