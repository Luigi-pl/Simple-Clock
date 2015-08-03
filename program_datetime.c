#include <avr/interrupt.h>
#include <stdlib.h>
#include "program.h"
#include "HD44780.h"
#include "i2c.h"
#include "pcf8583.h"

//Funkcja s³u¿y do wybrania elementu do zmiany w trybie edycji czasu i daty
void changeDateTime(int8_t value)
{
	int maxValue = 5;
	int minValue = 0;

	if(value > 0)
	{
		if(dateTime < maxValue)
		{
			dateTime += 1;
		}
		else
		{
			dateTime = minValue;
		}
	}
	else if(value < 0)
	{
		if(dateTime > minValue)
		{
			dateTime -= 1;
		}
		else
		{
			dateTime = maxValue;
		}
	}
}
//Funkcja s³u¿y do obs³ugi zmian wybranego elementu w trybie edycji czasu i daty
void changeDateTimeValue(int8_t value)
{
	if(dateTime == 0)
	{
		changeHours(registers.hoursRegister.tensPlace * 10 + registers.hoursRegister.unitPlace, value);
	}
	else if(dateTime == 1)
	{
		changeMinutes(registers.minutesRegister.tensPlace * 10 + registers.minutesRegister.unitPlace, value);
	}
	else if(dateTime == 2)
	{
		changeWeekdays(registers.dateBRegister.weekdays, value);
	}
	else if(dateTime == 3)
	{
		changeDays(registers.dateARegister.dayTensPlace * 10 + registers.dateARegister.dayUnitPlace, value);
	}
	else if(dateTime == 4)
	{
		changeMonths(registers.dateBRegister.monthTensPlace * 10 + registers.dateBRegister.monthUnitPlace , value);
	}
	else if(dateTime == 5)
	{
		changeYears(registers.year.year , value);
	}
}
//Funkcja s³u¿y do obs³ugi przycisków w trybie edycji czasu i daty
void dateTimeManager(int8_t przyciskLewo, int8_t przyciskPrawo, int8_t przyciskWyboru)
{
	if(dateTimeChoose == 0)
	{
		if(przyciskLewo == 0)
		{
			changeDateTime(-1);
			drawLCD_TimeDateMode();
			_delay_ms(200);
		}
		else if(przyciskPrawo == 0)
		{
			changeDateTime(1);
			drawLCD_TimeDateMode();
			_delay_ms(200);
		}
		else if(przyciskWyboru == 0)
		{
			dateTimeChoose = 1;
			elapsedTime = 0;
			drawLCD_TimeDateMode();
			_delay_ms(200);
		}
	}
	else if(dateTimeChoose == 1)
	{
		if(przyciskLewo == 0)
		{
			changeDateTimeValue(-1);
			drawLCD_TimeDateMode();
			_delay_ms(200);
		}
		else if(przyciskPrawo == 0)
		{
			changeDateTimeValue(1);
			drawLCD_TimeDateMode();
			_delay_ms(200);
		}
		else if(przyciskWyboru == 0)
		{
			dateTimeChoose = 0;
			elapsedTime = 0;
			_delay_ms(200);
		}
		
	}
}