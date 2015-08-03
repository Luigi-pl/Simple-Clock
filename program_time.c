#include <avr/interrupt.h>
#include <stdlib.h>
#include "program.h"
#include "HD44780.h"
#include "i2c.h"
#include "pcf8583.h"

//Funkcja s³u¿y do zmiany godzin
void changeHours(uint8_t local_time, int8_t value)
{
	if(value < 0)
	{
		if(local_time == 0)
		{
			registers.hoursRegister.tensPlace = 2;
			registers.hoursRegister.unitPlace = 3;
		}
		else if ((local_time % 10) == 0)
		{
			registers.hoursRegister.tensPlace -= 1;
			registers.hoursRegister.unitPlace = 9;
		}
		else
		{
			registers.hoursRegister.unitPlace -= 1;
		}
	}
	else
	{
		if(local_time == 23)
		{
			registers.hoursRegister.tensPlace = 0;
			registers.hoursRegister.unitPlace = 0;
		}
		else if ((local_time % 10) == 9)
		{
			registers.hoursRegister.tensPlace += 1;
			registers.hoursRegister.unitPlace = 0;
		}
		else
		{
			registers.hoursRegister.unitPlace += 1;
		}
	}
}
//Funkcja s³u¿y do zmiany minut
void changeMinutes(uint8_t local_time, int8_t value)
{
	if(value < 0)
	{
		if(local_time == 0)
		{
			registers.minutesRegister.tensPlace = 5;
			registers.minutesRegister.unitPlace = 9;
		}
		else if ((local_time % 10) == 0)
		{
			registers.minutesRegister.tensPlace -= 1;
			registers.minutesRegister.unitPlace = 9;
		}
		else
		{
			registers.minutesRegister.unitPlace -= 1;
		}
	}
	else
	{
		if(local_time == 59)
		{
			registers.minutesRegister.tensPlace = 0;
			registers.minutesRegister.unitPlace = 0;
		}
		else if ((local_time % 10) == 9)
		{
			registers.minutesRegister.tensPlace += 1;
			registers.minutesRegister.unitPlace = 0;
		}
		else
		{
			registers.minutesRegister.unitPlace += 1;
		}
	}
}