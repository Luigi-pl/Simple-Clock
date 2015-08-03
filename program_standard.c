#include <avr/interrupt.h>
#include <stdlib.h>
#include "program.h"
#include "HD44780.h"
#include "i2c.h"
#include "pcf8583.h"

//Funkcja s³u¿y do wybrania alarmu z listy alarmów w trybie domyœlnym, który ma byæ aktywny
void changeAlarm(int8_t value)
{
	int8_t maxValue = 19;
	int8_t minValue = 0;
	
	if(value > 0 && alarm == maxValue)
	{
		alarm = minValue;
	}
	else if(value < 0 && alarm == minValue)
	{
		alarm = maxValue;
	}
	else
	{
		alarm += value;
	}
}
//Funkcja s³u¿y do obslugi przycisków trybie domyœlnym
void alarmManager(int8_t przyciskLewo, int8_t przyciskPrawo, int8_t przyciskWyboru)
{
	if(alarmChoose == 0)
	{
		if(przyciskLewo == 0 && przyciskPrawo == 0)
		{
			stopBuzzer();
			_delay_ms(200);
		}
		else if(przyciskWyboru == 0)
		{
			alarmChoose = 1;
			cli();
			if(globalClockMode == STANDARD)
			{
				switchHighlightOnLCD();
			}
			_delay_ms(200);
		}
	}
	else if(alarmChoose == 1)
	{
		if(przyciskLewo == 0)
		{
			changeAlarm(-1);
			drawLCD_StandardMode();
			_delay_ms(200);
		}
		else if(przyciskPrawo == 0)
		{
			changeAlarm(1);
			drawLCD_StandardMode();
			_delay_ms(200);
		}
		else if(przyciskWyboru == 0)
		{
			alarmRegisters.control.nextActiveAlarm = alarm;
			PCF_saveAlarmListControl(&(alarmRegisters.control));
			alarmChoose = 0;
			sei();
			_delay_ms(200);
		}
	}
}