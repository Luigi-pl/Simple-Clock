#include <avr/interrupt.h>
#include <stdlib.h>
#include "program.h"
#include "HD44780.h"
#include "i2c.h"
#include "pcf8583.h"

//Funkcja s³u¿y do wybrania opcji, która ma byæ zmieniona w trybie edycji opcji
void changeOptions(int8_t value)
{
	int8_t maxValue = 2;
	int8_t minValue = 0;

	if(value > 0)
	{
		if(option < maxValue)
		{
			option += 1;
		}
		else
		{
			option = minValue;
		}
	}
	else if(value < 0)
	{
		if(option > minValue)
		{
			option -= 1;
		}
		else
		{
			option = maxValue;
		}
	}
}
//Funkcja s³u¿y edycji wybranej opcji w trybie edycji opcji
void changeOptionsValue(int8_t value)
{
	if(option == 0)
	{
		if(highlightControl.highlightType == 0)
		{
			highlightControl.highlightType = 1;
		}
		else
		{
			highlightControl.highlightType = 0;
		}
	}
	else if(option == 1)
	{
		if(3 < highlightControl.highlightTime && highlightControl.highlightTime < 120)
		{
			highlightControl.highlightTime += value;
		}
		else if(3 == highlightControl.highlightTime && value > 0)
		{
			highlightControl.highlightTime += 1;
		}
		else if(highlightControl.highlightTime == 120 && value < 0)
		{
			highlightControl.highlightTime -= 1;
		}
	}
	else if(option == 2)
	{
		if(alarmEnableBit == 0)
		{
			alarmEnableBit = 1;
		}
		else
		{
			alarmEnableBit = 0;
		}
	}
}
//Funkcja obs³uguj¹ca przyciski w trybie edycji opcji
void optionsManager(int8_t przyciskLewo, int8_t przyciskPrawo, int8_t przyciskWyboru)
{
	if(optionChoose == 0)
	{
		if(przyciskLewo == 0)
		{
			changeOptions(-1);
			drawLCD_OptionsMode();
			_delay_ms(200);
		}
		else if(przyciskPrawo == 0)
		{
			changeOptions(1);
			drawLCD_OptionsMode();
			_delay_ms(200);
		}
		else if(przyciskWyboru == 0)
		{
			optionChoose = 1;
			editMode();
			_delay_ms(200);
		}
	}
	else if(optionChoose == 1)
	{
		if(przyciskLewo == 0)
		{
			changeOptionsValue(-1);
			drawLCD_OptionsMode();
			editMode();
			_delay_ms(200);
		}
		else if(przyciskPrawo == 0)
		{
			changeOptionsValue(1);
			drawLCD_OptionsMode();
			editMode();
			_delay_ms(200);
		}
		else if(przyciskWyboru == 0)
		{
			optionChoose = 0;
			elapsedTime = 0;
			editMode();
			_delay_ms(200);
		}
	}
}