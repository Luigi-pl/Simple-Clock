#include <avr/interrupt.h>
#include <stdlib.h>
#include "program.h"
#include "HD44780.h"
#include "i2c.h"
#include "pcf8583.h"

//Funkcja s³u¿y do elementu alarmu/alarmu, który chcemy zmieniæ
void changeAlarmOptions(int8_t value)
{
	int8_t maxValue;
	int8_t minValue;

	if(alarmOptionsChoose == 0)
	{
		maxValue = 19;
		minValue = 0;
		
		if(value > 0)
		{
			if(alarmOptions < maxValue)
			{
				alarmOptions += 1;
			}
			else
			{
				alarmOptions = minValue;
			}
		}
		else if(value < 0)
		{
			if(alarmOptions > minValue)
			{
				alarmOptions -= 1;
			}
			else
			{
				alarmOptions = maxValue;
			}
		}
	}
	else if(alarmOptionsChoose == 1)
	{
		maxValue = 4;
		minValue = 0;
		
		if(value > 0)
		{
			if(alarmOptionsB < maxValue)
			{
				alarmOptionsB += 1;
			}
			else
			{
				alarmOptionsB = minValue;
			}
		}
		else if(value < 0)
		{
			if(alarmOptionsB > minValue)
			{
				alarmOptionsB -= 1;
			}
			else
			{
				alarmOptionsB = maxValue;
			}
		}
	}
}
//Funkcja s³u¿y do zmiany ustawieñ wybranego alarmu
void changeAlarmOptionsValue(int8_t value)
{
	if(alarmOptionsB == 0)
	{
		if(alarmRegisters.alarms[alarmOptions].weekly == 1)
		{
			alarmRegisters.alarms[alarmOptions].weekly = 0;
		}
		else
		{
			alarmRegisters.alarms[alarmOptions].weekly = 1;
		}
	}
	else if(alarmOptionsB == 1)
	{
		if(alarmRegisters.alarms[alarmOptions].weekdays == 6 && value > 0)
		{
			alarmRegisters.alarms[alarmOptions].weekdays = 0;
		}
		else if(alarmRegisters.alarms[alarmOptions].weekdays == 0 && value < 0)
		{
			alarmRegisters.alarms[alarmOptions].weekdays = 6;
		}
		else
		{
			alarmRegisters.alarms[alarmOptions].weekdays += value;
		}
	}
	else if(alarmOptionsB == 2)
	{
		if(alarmRegisters.alarms[alarmOptions].hours == 23 && value > 0)
		{
			alarmRegisters.alarms[alarmOptions].hours = 0;
		}
		else if(alarmRegisters.alarms[alarmOptions].hours == 0 && value < 0)
		{
			alarmRegisters.alarms[alarmOptions].hours = 23;
		}
		else
		{
			alarmRegisters.alarms[alarmOptions].hours += value;
		}
	}
	else if(alarmOptionsB == 3)
	{
		if(alarmRegisters.alarms[alarmOptions].minutes == 59 && value > 0)
		{
			alarmRegisters.alarms[alarmOptions].minutes = 0;
		}
		else if(alarmRegisters.alarms[alarmOptions].minutes == 0 && value < 0)
		{
			alarmRegisters.alarms[alarmOptions].minutes = 59;
		}
		else
		{
			alarmRegisters.alarms[alarmOptions].minutes += value;
		}
	}
	else if(alarmOptionsB == 4)
	{
		if(alarmRegisters.alarms[alarmOptions].active == 1)
		{
			alarmRegisters.alarms[alarmOptions].active = 0;
			alarmRegisters.control.numberOfAlarm -= 1;
		}
		else
		{
			alarmRegisters.alarms[alarmOptions].active = 1;
			alarmRegisters.control.numberOfAlarm += 1;
		}
	}
}
//Funkcja obs³uguj¹ca przyciski w trybie edycji alarmów i ich ustawieñ
void alarmOptionsManager(int8_t przyciskLewo, int8_t przyciskPrawo, int8_t przyciskWyboru)
{
	if(alarmOptionsChoose == 0)
	{
		if(przyciskLewo == 0 && przyciskPrawo == 0)
		{
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
		else if(przyciskLewo == 0)
		{
			changeAlarmOptions(-1);
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
		else if(przyciskPrawo == 0)
		{
			changeAlarmOptions(1);
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
		else if(przyciskWyboru == 0)
		{
			alarmOptionsChoose = 1;
			alarmOptionsB = 0;
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
	}
	else if(alarmOptionsChoose == 1)
	{
		if(przyciskLewo == 0 && przyciskPrawo == 0)
		{
			alarmOptionsChoose = 0;
			elapsedTime = 0;
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
		else if(przyciskLewo == 0)
		{
			changeAlarmOptions(-1);
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
		else if(przyciskPrawo == 0)
		{
			changeAlarmOptions(1);
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
		else if(przyciskWyboru == 0)
		{
			alarmOptionsChoose = 2;
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
	}
	else if(alarmOptionsChoose == 2)
	{
		if(przyciskLewo == 0)
		{
			changeAlarmOptionsValue(-1);
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
		else if(przyciskPrawo == 0)
		{
			changeAlarmOptionsValue(1);
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
		else if(przyciskWyboru == 0)
		{
			alarmOptionsChoose = 1;
			elapsedTime = 0;
			drawLCD_AlarmMode();
			_delay_ms(200);
		}
	}
}