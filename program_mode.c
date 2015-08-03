#include <avr/interrupt.h>
#include <stdlib.h>
#include "program.h"
#include "HD44780.h"
#include "i2c.h"
#include "pcf8583.h"

//Funkcja s³u¿y do obs³ugi zmiany trybów
void changeMode(void)
{
	if(globalClockMode == STANDARD)
	{
		switchHighlightOnLCD();
		globalClockMode = HIGHLIGHTED;
	}
	else if(globalClockMode == HIGHLIGHTED)
	{
		option = 0;
		optionChoose = 0;
		
		LCD_clear();
		
		globalClockMode = EDIT_OPTIONS;
		drawOptionsMode();
	}
	else if(globalClockMode == EDIT_OPTIONS)
	{
		PCF_saveHighlightSettings(&highlightControl);
		PCF_saveControlSettings(&registers);
		
		dateTime = 0;
		dateTimeChoose = 0;
		elapsedTime = 1;
		
		LCD_clear();
		
		globalClockMode = EDIT_DATETIME;
		drawTimeDateMode();
	}
	else if(globalClockMode == EDIT_DATETIME)
	{
		PCF_saveTimeDate(&registers);
		
		alarmOptions = 0;
		alarmOptionsB = 0;
		alarmOptionsChoose = 0;
		
		LCD_clear();
		
		drawAlarmMode();
		globalClockMode = ALARMS;
	}
	else if(globalClockMode == ALARMS)
	{
		PCF_saveAlarmList(&alarmRegisters);
		PCF_readRegisters(&registers);
		
		LCD_clear();
		
		drawStandardMode();
		globalClockMode = HIGHLIGHTED;
		
		sei();
		alarmChoose = 0;
		alarm = 0;
	}
	_delay_ms(200);
}