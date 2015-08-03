#include <avr/interrupt.h>
#include <stdlib.h>
#include "program.h"
#include "HD44780.h"
#include "i2c.h"
#include "pcf8583.h"

//Funkcja s³u¿y do zmiany dnia tygodnia
void changeWeekdays(uint8_t local_date, int8_t value)
{
	if(local_date == 0 && value < 0)
	{
		registers.dateBRegister.weekdays = 6;
	}
	else if(local_date == 6 && value > 0)
	{
		registers.dateBRegister.weekdays = 0;
	}
	else
	{
		registers.dateBRegister.weekdays += value;
	}
}
//Funkcja s³u¿y do zmiany dnia
void changeDays(uint8_t local_date, int8_t value)
{
	uint8_t month = registers.dateBRegister.monthTensPlace * 10 + registers.dateBRegister.monthUnitPlace;
	if(value > 0)
	{
		if( 
			((registers.year.year % 4) == 0 && month == 2 && local_date == 29) ||
			((registers.year.year % 4) != 0 && month == 2 && local_date == 28) ||
			((month == 4 || month == 6 || month == 9 || month == 11) && local_date == 30) ||
			((month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12) && local_date == 31)
		)
		{
			registers.dateARegister.dayTensPlace = 0;
			registers.dateARegister.dayUnitPlace = 1; 
			
		}
		else if((local_date % 10) == 9 &&  local_date < 25)
		{
			registers.dateARegister.dayTensPlace += 1;
			registers.dateARegister.dayUnitPlace = 0; 
			
		}
		else
		{
			registers.dateARegister.dayUnitPlace += 1;
		}
	}
	else
	{
		if(local_date == 1 && month == 2 && (registers.year.year % 4) == 0)
		{
			registers.dateARegister.dayTensPlace = 2;
			registers.dateARegister.dayUnitPlace = 9; 
		}
		else if(local_date == 1 && month == 2 && (registers.year.year % 4) != 0)
		{
			registers.dateARegister.dayTensPlace = 2;
			registers.dateARegister.dayUnitPlace = 8; 
			
		}
		else if(local_date == 1 && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12))
		{
			registers.dateARegister.dayTensPlace = 3;
			registers.dateARegister.dayUnitPlace = 1; 
		}
		else if(local_date == 1 && (month == 4 || month == 6 || month == 9 || month == 11))
		{
			registers.dateARegister.dayTensPlace = 3;
			registers.dateARegister.dayUnitPlace = 0; 
		}
		else if((local_date % 10) == 0)
		{
			registers.dateARegister.dayTensPlace -= 1;
			registers.dateARegister.dayUnitPlace = 9; 
		}
		else
		{
			registers.dateARegister.dayUnitPlace -= 1;
		}
	}
}
//Funkcja s³u¿y do zmiany miesi¹ca
void changeMonths(uint8_t local_date, int8_t value)
{
	if(value > 0)
	{
		if(local_date == 12)
		{
			registers.dateBRegister.monthTensPlace = 0;
			registers.dateBRegister.monthUnitPlace = 1;
		}
		else if((local_date % 10) == 9)
		{
			registers.dateBRegister.monthTensPlace = 1;
			registers.dateBRegister.monthUnitPlace = 0;
		}
		else
		{
			registers.dateBRegister.monthUnitPlace += 1;
		}
	}
	else
	{
		if(local_date == 1)
		{
			registers.dateBRegister.monthTensPlace = 1;
			registers.dateBRegister.monthUnitPlace = 2;
		}
		else if((local_date % 10) == 0)
		{
			registers.dateBRegister.monthTensPlace -= 1;
			registers.dateBRegister.monthUnitPlace = 9;
		}
		else
		{
			registers.dateBRegister.monthUnitPlace -= 1;
		}
	}
}
//Funkcja s³u¿y do zmiany roku
void changeYears(uint8_t local_date, int8_t value)
{
	registers.year.year += value;
}