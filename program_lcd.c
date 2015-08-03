#include <avr/interrupt.h>
#include <stdlib.h>
#include "program.h"
#include "HD44780.h"
#include "i2c.h"
#include "pcf8583.h"

//Funkcja s³u¿y do wypisania liczby na okreœlonej pozycji
void drawNumberOnLCD(int number, int position, int row)
{
	char text[5];
	itoa(number, text, 10);
	if(number < 10)
	{
		LCD_goto(position, row);
		LCD_write_text("0");
	}
	else
	{
		LCD_goto(position, row);
	}
	LCD_write_text(text);
}
//Funkcja s³u¿y do wypisania tekstu na okreœlonej pozycji
void drawTextOnLCD(char* text, int position, int row)
{
	LCD_goto(position, row);
	LCD_write_text(text);
}
//Funkcja s³u¿y do wypisania : na okreœlonej pozycji
void drawColon(int position, int row)
{
	drawTextOnLCD(":", position, row);
}
//Funkcja s³u¿y do wypisania / na okreœlonej pozycji
void drawSlash(int position, int row)
{
	drawTextOnLCD("/", position, row);
}
//Funkcja s³u¿y do wypisania separatora czasu : i daty / w okreœlonych pozycjach
void drawColonAndSlashForTimeAndDate(void)
{
	drawColon(TIME_POSITION + 2, TIME_ROW);
	drawColon(TIME_POSITION + 5, TIME_ROW);
	
	drawSlash(DATE_POSITION + 2, DATE_ROW);
	drawSlash(DATE_POSITION + 5, DATE_ROW);
}
//Funkcja s³u¿y do wypisania separatora czasu w trybie wyœwietlania listy alarmów
void drawColonForAlarm(void)
{
	drawColon(ALARM_POSITION + 10, ALARM_ROW);
}
//Funkcja s³u¿y do wypisania separatora czasu w trybie edycji alarmów i ich ustawieñ
void drawColonForAlarmEdit(void)
{
	drawColon(ALARM_EDIT_POSITION_A + 2, ALARM_EDIT_ROW_A);
}
//Funkcja s³u¿y do wyœwietlenia informacji o sposobie kontroli zegara
void drawInformationAboutOptionControl(void)
{
	drawTextOnLCD("L: K1", OPTION_LEFT, OPTION_1_ROW);
	drawTextOnLCD("P: K3", OPTION_RIGHT, OPTION_1_ROW);
	
	drawTextOnLCD("Z: K2", OPTION_LEFT, OPTION_2_ROW);
	drawTextOnLCD("M: K5", OPTION_RIGHT, OPTION_2_ROW);
}
//Funkcja s³u¿y do wyœwietlenia informacji o sposobie kontroli zegara
void drawInformationAboutTimeDateControl(void)
{
	drawTextOnLCD("P: K1", OPTION_LEFT, OPTION_1_ROW);
	drawTextOnLCD("N: K3", OPTION_RIGHT, OPTION_1_ROW);
	
	drawTextOnLCD("Z: K2", OPTION_LEFT, OPTION_2_ROW);
	drawTextOnLCD("M: K5", OPTION_RIGHT, OPTION_2_ROW);
}
//Funkcja s³u¿y do wyœwietlenia informacji o sposobie kontroli zegara
void drawInformationAboutAlarmChooseControl(void)
{
	if(alarmOptionsChoose == 0)
	{
		drawTextOnLCD("P: K1", OPTION_LEFT, OPTION_1_ROW);
		drawTextOnLCD("N: K3", OPTION_RIGHT, OPTION_1_ROW);
		
		drawTextOnLCD("Z: K2", OPTION_LEFT, OPTION_2_ROW);
		drawTextOnLCD("M: K5", OPTION_RIGHT, OPTION_2_ROW);
	}
	else
	{
		drawTextOnLCD("P: K1", OPTION_LEFT, OPTION_1_ROW);
		drawTextOnLCD("N: K3", OPTION_RIGHT, OPTION_1_ROW);
		
		drawTextOnLCD("Z: K2", OPTION_LEFT, OPTION_2_ROW);
		drawTextOnLCD("B: K13", OPTION_RIGHT - 1, OPTION_2_ROW);
	}
}
//Funkcja s³u¿y do wypisania w formie tekstowej na okreœlonej pozycji jaki jest dzieñ tygodnia
void drawDayOfWeek(uint8_t weekday, uint8_t position, uint8_t row)
{
	if(weekday == 0)
	{
		drawTextOnLCD("PN", position, row);
	}
	else if(weekday == 1)
	{
		drawTextOnLCD("WT", position, row);
	}
	else if(weekday == 2)
	{
		drawTextOnLCD("SR", position, row);
	}
	else if(weekday == 3)
	{
		drawTextOnLCD("CZ", position, row);
	}
	else if(weekday == 4)
	{
		drawTextOnLCD("PT", position, row);
	}
	else if(weekday == 5)
	{
		drawTextOnLCD("SB", position, row);
	}
	else if(weekday == 6)
	{
		drawTextOnLCD("ND", position, row);
	}
	else
	{
		drawTextOnLCD("  ", position, row);
	}
}
//Funkcja s³u¿y do wypisania w formie tekstowej na okreœlonej pozycji jaki jest dzieñ tygodnia dla trybu domyœlnego
void drawDayOfWeek_Standard(uint8_t weekday)
{
	drawDayOfWeek(weekday, DATE_POSITION - 4, DATE_ROW);
}
//Funkcja s³u¿y do wypisania w formie tekstowej na okreœlonej pozycji jaki jest dzieñ tygodnia dla trybu wyœwietlania listy alarmów
void drawDayOfWeek_Alarm(uint8_t weekday)
{
	drawDayOfWeek(weekday, ALARM_POSITION +  5, ALARM_ROW);
}
//Funkcja s³u¿y do wypisania w formie tekstowej na okreœlonej pozycji jaki jest dzieñ tygodnia dla trybu edycji alarmów i ich ustawieñ
void drawDayOfWeek_AlarmEdit(uint8_t weekday)
{
	drawDayOfWeek(weekday, ALARM_EDIT_POSITION_A - 3, ALARM_EDIT_ROW_A);
}
//Funkcja s³u¿y do wyœwietlenia na ekranie LCD trybu domyœlnego
void drawStandardMode(void)
{
	drawNumberOnLCD(	registers.hoursRegister.tensPlace * 10 + registers.hoursRegister.unitPlace, 			
					TIME_POSITION + 0, 
					TIME_ROW);
	drawNumberOnLCD(	registers.minutesRegister.tensPlace * 10 + registers.minutesRegister.unitPlace, 
					TIME_POSITION + 3, 
					TIME_ROW);
	drawNumberOnLCD(	registers.secondsRegister.tensPlace * 10 + registers.secondsRegister.unitPlace, 
					TIME_POSITION + 6, 
					TIME_ROW);
	
	drawDayOfWeek_Standard(registers.dateBRegister.weekdays);
	drawNumberOnLCD(	registers.dateARegister.dayTensPlace * 10 	+ registers.dateARegister.dayUnitPlace, 
					DATE_POSITION + 0, 
					DATE_ROW);
	drawNumberOnLCD(	registers.dateBRegister.monthTensPlace * 10 + registers.dateBRegister.monthUnitPlace, 
					DATE_POSITION + 3, 
					DATE_ROW);
	drawNumberOnLCD(	registers.year.year, 
					DATE_POSITION + 6, 
					DATE_ROW);
	
	//Kod odpowiedzialny za wyœwietlanie aktualnego budzika - do zrobienia
	if(alarmRegisters.alarms[alarm].active == 1 && alarmEnableBit == 1)
	{
		drawTextOnLCD("    ", ALARM_POSITION, ALARM_ROW);
		drawNumberOnLCD(alarm, ALARM_POSITION + 1, ALARM_ROW);
		drawDayOfWeek_Alarm(alarmRegisters.alarms[alarm].weekdays);
		drawNumberOnLCD(alarmRegisters.alarms[alarm].hours, 		ALARM_POSITION +  8, ALARM_ROW);
		drawNumberOnLCD(alarmRegisters.alarms[alarm].minutes, 	ALARM_POSITION + 11, ALARM_ROW);
		if(alarmRegisters.alarms[alarm].weekly == 1)
		{
			drawTextOnLCD("T", ALARM_POSITION + 14, ALARM_ROW);
		}
		else
		{
			drawTextOnLCD("J", ALARM_POSITION + 14, ALARM_ROW);
		}
		drawColonForAlarm();
	}
	else if(alarmEnableBit == 1)
	{
		drawNumberOnLCD(alarm, ALARM_POSITION + 1, ALARM_ROW);
		drawTextOnLCD("    BRAK    ", ALARM_POSITION + 4, ALARM_ROW);
	}
	
	drawColonAndSlashForTimeAndDate();
}
//Funkcja s³u¿y do wyœwietlenia na ekranie LCD trybu edycji opcji
void drawOptionsMode(void)
{
	LCD_clear();
	drawInformationAboutOptionControl();
	
	if(option == 0)
	{
		drawTextOnLCD("Typ Podswiet.", OPTION_TEXT_FIRST, OPTION_TEXT_1_ROW);
		if(highlightControl.highlightType == 0)
		{
			drawTextOnLCD("Standardowe", OPTION_TEXT_FIRST, OPTION_TEXT_2_ROW);
		}
		else
		{
			drawTextOnLCD("Stale", OPTION_TEXT_FIRST, OPTION_TEXT_2_ROW);
		}
	}
	else if(option == 1)
	{
		drawTextOnLCD("Czas Podswiet.", OPTION_TEXT_FIRST, OPTION_TEXT_1_ROW);
		drawNumberOnLCD(highlightControl.highlightTime, OPTION_TEXT_FIRST, OPTION_TEXT_2_ROW);
		LCD_write_text(" s");
	}
	else if(option == 2)
	{
		drawTextOnLCD("Alarm aktywny?", OPTION_TEXT_FIRST, OPTION_TEXT_1_ROW);
		if(alarmEnableBit == 0)
		{
			drawTextOnLCD("NIE", OPTION_TEXT_FIRST, OPTION_TEXT_2_ROW);
		}
		else
		{
			drawTextOnLCD("TAK", OPTION_TEXT_FIRST, OPTION_TEXT_2_ROW);
		}
	}
}
//Funkcja s³u¿y do wyœwietlenia na ekranie LCD trybu edycji czasu i daty
void drawTimeDateMode(void)
{
	drawInformationAboutTimeDateControl();

	if(dateTime == 0 && elapsedTime == 1)
	{
		drawTextOnLCD("  ", 
					TIME_POSITION + 0, 
					TIME_ROW);
	}
	else
	{
		drawNumberOnLCD(	registers.hoursRegister.tensPlace * 10 + registers.hoursRegister.unitPlace, 			
					TIME_POSITION + 0, 
					TIME_ROW);
	}
	if(dateTime == 1 && elapsedTime == 1)
	{
		drawTextOnLCD("  ", 
					TIME_POSITION + 3, 
					TIME_ROW);
	}
	else
	{
		drawNumberOnLCD(	registers.minutesRegister.tensPlace * 10 + registers.minutesRegister.unitPlace, 
					TIME_POSITION + 3, 
					TIME_ROW);
	}
	
	drawNumberOnLCD(	registers.secondsRegister.tensPlace * 10 + registers.secondsRegister.unitPlace, 
				TIME_POSITION + 6, 
				TIME_ROW);

	if(dateTime == 2 && elapsedTime == 1)
	{
		drawDayOfWeek_Standard(7);
	}
	else
	{
		drawDayOfWeek_Standard(registers.dateBRegister.weekdays);
	}
	
	if(dateTime == 3 && elapsedTime == 1)
	{
		drawTextOnLCD("  ", 
					DATE_POSITION + 0, 
					DATE_ROW);
	}
	else
	{
		drawNumberOnLCD(	registers.dateARegister.dayTensPlace * 10 	+ registers.dateARegister.dayUnitPlace, 
					DATE_POSITION + 0, 
					DATE_ROW);
	}
	if(dateTime == 4 && elapsedTime == 1)
	{
		drawTextOnLCD("  ", 
					DATE_POSITION + 3, 
					DATE_ROW);
	}
	else
	{
		drawNumberOnLCD(	registers.dateBRegister.monthTensPlace * 10 + registers.dateBRegister.monthUnitPlace, 
					DATE_POSITION + 3, 
					DATE_ROW);
	}
	if(dateTime == 5 && elapsedTime == 1)
	{
		drawTextOnLCD("    ", 
					DATE_POSITION + 6, 
					DATE_ROW);
	}
	else
	{
		drawNumberOnLCD(	registers.year.year, 
					DATE_POSITION + 6, 
					DATE_ROW);
	}
	
	drawColonAndSlashForTimeAndDate();
}
//Funkcja s³u¿y do wyœwietlenia na ekranie LCD trybu wyœwietlania listy alarmów oraz edycji alarmów i ich ustawieñ
void drawAlarmMode(void)
{
	drawInformationAboutAlarmChooseControl();
	if(alarmOptionsChoose == 0)
	{
		drawNumberOnLCD(alarmOptions, ALARM_EDIT_POSITION_B - 3, ALARM_EDIT_ROW_B);
		if(alarmRegisters.alarms[alarmOptions].weekly == 1)
		{
			drawTextOnLCD(" TYGODNIOWY", ALARM_EDIT_POSITION_B, ALARM_EDIT_ROW_B);
		}
		else
		{
			drawTextOnLCD("JEDNORAZOWY", ALARM_EDIT_POSITION_B, ALARM_EDIT_ROW_B);
		}
		
		drawDayOfWeek_AlarmEdit(alarmRegisters.alarms[alarmOptions].weekdays);
		drawNumberOnLCD(alarmRegisters.alarms[alarmOptions].hours, 		ALARM_EDIT_POSITION_A + 0, ALARM_EDIT_ROW_A);
		drawNumberOnLCD(alarmRegisters.alarms[alarmOptions].minutes, 	ALARM_EDIT_POSITION_A + 3, ALARM_EDIT_ROW_A);
		
		if(alarmRegisters.alarms[alarmOptions].active == 1)
		{
			drawTextOnLCD(" AKTYW", ALARM_EDIT_POSITION_A + 6, ALARM_EDIT_ROW_A);
		}
		else
		{
			drawTextOnLCD("NAKTYW", ALARM_EDIT_POSITION_A + 6, ALARM_EDIT_ROW_A);
		}
	}
	else if(alarmOptionsChoose >= 1)
	{
		drawNumberOnLCD(alarmOptions, ALARM_EDIT_POSITION_B - 3, ALARM_EDIT_ROW_B);
		if(alarmOptionsB == 0 && elapsedTime == 1)
		{
			drawTextOnLCD("           ", ALARM_EDIT_POSITION_B, ALARM_EDIT_ROW_B);
		}
		else
		{
			if(alarmRegisters.alarms[alarmOptions].weekly == 1)
			{
				drawTextOnLCD(" TYGODNIOWY", ALARM_EDIT_POSITION_B, ALARM_EDIT_ROW_B);
			}
			else
			{
				drawTextOnLCD("JEDNORAZOWY", ALARM_EDIT_POSITION_B, ALARM_EDIT_ROW_B);
			}
		}
		
		if(alarmOptionsB == 1 && elapsedTime == 1)
		{
			drawDayOfWeek_AlarmEdit(7);
		}
		else
		{
			drawDayOfWeek_AlarmEdit(alarmRegisters.alarms[alarmOptions].weekdays);
		}
		
		if(alarmOptionsB == 2 && elapsedTime == 1)
		{
			drawTextOnLCD("  ", ALARM_EDIT_POSITION_A + 0, ALARM_EDIT_ROW_A);
		}
		else
		{
			drawNumberOnLCD(alarmRegisters.alarms[alarmOptions].hours, 		ALARM_EDIT_POSITION_A + 0, ALARM_EDIT_ROW_A);
		}
		
		if(alarmOptionsB == 3 && elapsedTime == 1)
		{
			drawTextOnLCD("  ", ALARM_EDIT_POSITION_A + 3, ALARM_EDIT_ROW_A);
		}
		else
		{
			drawNumberOnLCD(alarmRegisters.alarms[alarmOptions].minutes, 	ALARM_EDIT_POSITION_A + 3, ALARM_EDIT_ROW_A);
		}
		
		if(alarmOptionsB == 4 && elapsedTime == 1)
		{
			drawTextOnLCD("      ", ALARM_EDIT_POSITION_A + 6, ALARM_EDIT_ROW_A);
		}
		else
		{
			if(alarmRegisters.alarms[alarmOptions].active == 1)
			{
				drawTextOnLCD(" AKTYW", ALARM_EDIT_POSITION_A + 6, ALARM_EDIT_ROW_A);
			}
			else
			{
				drawTextOnLCD("NAKTYW", ALARM_EDIT_POSITION_A + 6, ALARM_EDIT_ROW_A);
			}
		}
	}
	drawColonForAlarmEdit();
}


void blinkTimeDateMode(void)
{
	if(elapsedTimeB == 0 || elapsedTimeB == 100000)
	{
		elapsedTimeB = 0;
		elapsedTime = 1;
		drawTimeDateMode();
	}
	else if(elapsedTimeB == 50000)
	{
		elapsedTime = 0;
		drawTimeDateMode();
	}
}
void blinkAlarmMode(void)
{
	if(elapsedTimeB == 0 || elapsedTimeB == 100000)
	{
		elapsedTimeB = 0;
		elapsedTime = 1;
		drawAlarmMode();
	}
	else if(elapsedTimeB == 50000)
	{
		elapsedTime = 0;
		drawAlarmMode();
	}
}
//Funkcja s³u¿y do podœwietlania LCD
void switchHighlightOnLCD(void)
{
	if(globalClockMode == STANDARD || highlightControl.highlightType == 1)
	{
		globalClockMode = HIGHLIGHTED;
		PORT(LCD_HIGHLIGHT_PORT) |= (1<<LCD_HIGHLIGHT_PIN);
	}
	else if(highlightControl.highlightType == 0 && globalClockMode == HIGHLIGHTED)
	{
		globalClockMode = STANDARD;
		PORT(LCD_HIGHLIGHT_PORT) &= ~(1<<LCD_HIGHLIGHT_PIN);
	}
}
//Funkcja slu¿y do wyœwietlenia znaku informuj¹cego, ¿e edytuje siê dan¹ opcjê
void editMode(void)
{
	if(optionChoose == 1)
	{
		LCD_goto(OPTION_TEXT_LAST, OPTION_TEXT_2_ROW);
		LCD_write_text("E");
	}
	else if(optionChoose == 0)
	{
		LCD_goto(OPTION_TEXT_LAST, OPTION_TEXT_2_ROW);
		LCD_write_text(" ");
	}
}
