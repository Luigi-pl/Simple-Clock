#include <avr/interrupt.h>
#include <stdlib.h>
#include "program.h"
#include "HD44780.h"
#include "i2c.h"
#include "pcf8583.h"

//////CONFIGURABLE//////
volatile PCF_Registers registers;
volatile Alarm_Registers alarmRegisters;
volatile HighlightControl highlightControl;
volatile uint8_t alarmEnableBit = 0;
//////UNCONFIGURABLE//////
volatile enum mode globalClockMode = STANDARD;

volatile uint8_t buzzerActivated = 0;

volatile uint8_t elapsedTime = 0;
volatile uint32_t elapsedTimeB = 0;

volatile uint8_t alarm = 0;
volatile uint8_t alarmChoose = 0;

volatile uint8_t option = 0;
volatile uint8_t optionChoose = 0;

volatile uint8_t dateTime = 0;
volatile uint8_t dateTimeChoose = 0;

volatile uint8_t alarmOptions = 0;
volatile uint8_t alarmOptionsB = 0;
volatile uint8_t alarmOptionsChoose = 0;
/////////////////////////////////////////////////////////////////////////
ISR (INT0_vect)
{
	if(globalClockMode == HIGHLIGHTED || globalClockMode == STANDARD)
	{ 
		PCF_readRegisters(&registers);
		drawStandardMode();
	}
	//Wylaczanie podswietlenia
	if(highlightControl.highlightType == 0 && globalClockMode == HIGHLIGHTED && elapsedTime < highlightControl.highlightTime)
	{
		elapsedTime += 1;
	}
	else if(highlightControl.highlightType == 0 && globalClockMode == HIGHLIGHTED)
	{
		elapsedTime = 0;
		switchHighlightOnLCD();
	}
	checkAlarm();
}

/////////////////////////////////////////////////////////////////////////
void prepareModules(void)
{

	//Przerwania
	PORT(INTERRUPT_PORT) |= ((1<<TIME_INTERRUPT_PIN));
	
	//Przyciski
	PORT(BUTTON_PORT) |= ((1<<BUTTON_LEFT_PIN)|(1<<BUTTON_CHOOSE_PIN)|(1<<BUTTON_RIGHT_PIN)|(1<<BUTTON_MODE_PIN));
	
	//Buzzer
	DDR(BUZZER_PORT)  |= ((1 << BUZZER_PIN));
	PORT(BUZZER_PORT) &= ~((1 << BUZZER_PIN));

	//LCD
	//wyjscia sterujace LCD
	DDR(LCD_PORTS) = (	(1<<LCD_RS_PIN)|(1<<LCD_RW_PIN)|(1<<LCD_E_PIN )|
						(1<<LCD_D4_PIN)|(1<<LCD_D5_PIN)|(1<<LCD_D6_PIN)|(1<<LCD_D7_PIN));
	//wyjscia sterujace podswietleniem LCD
	DDR(LCD_HIGHLIGHT_PORT) = (1<<LCD_HIGHLIGHT_PIN);
	PORT(LCD_HIGHLIGHT_PORT) |= (0<<LCD_HIGHLIGHT_PIN);
	LCD_init();
	LCD_home();
	LCD_write_text("WELCOME");

	//PCF8583
	PCF_init();
	PCF_loadHighlightSettings(&highlightControl);
	PCF_loadAlarmList(&alarmRegisters);
	PCF_readRegisters(&registers);
	
	drawStandardMode();
	switchHighlightOnLCD();
	
	//konfiguracja przerwan
	cli(); //blokujemy dzialanie przerwan
	MCUCR |= ((1<<ISC01) | (0<<ISC00)); //przerwania INT0 reagujace na zbocze opadajace (tick zegara)	
	GICR |= (1<<INT0); //wlaczamy przerwania INT0
	sei(); //odblokowujemy dzialanie przerwan
}
/////////////////////////////////////////////////////////////////////////
int main(void)
{
	//Przygotowanie mikrokontrolera i podzespo³ów
	prepareModules();

	
	uint8_t przyciskLewo = 0;
	uint8_t przyciskPrawo = 0;
	uint8_t przyciskWyboru = 0;
	uint8_t przyciskTrybu = 0;
	
	while(1)
	{	
		
		przyciskTrybu = ((PIND >> PIND4) & 0x1);
		
		if(przyciskTrybu == 0)
		{
			changeMode();
		}
		
		//Obs³uga ró¿nych trybów dzia³ania
		if(globalClockMode == HIGHLIGHTED || globalClockMode == STANDARD)
		{
			//Tryb standardowy (domyœlny) i podœwietlony
			przyciskLewo = ((PIND >> PIND7) & 0x1);
			przyciskPrawo = ((PIND >> PIND5) & 0x1);
			przyciskWyboru = ((PIND >> PIND6) & 0x1);
			alarmManager(przyciskLewo, przyciskPrawo, przyciskWyboru);
		}
		else if(globalClockMode == EDIT_OPTIONS)
		{
			//Tryb edycji opcji
			przyciskLewo = ((PIND >> PIND7) & 0x1);
			przyciskPrawo = ((PIND >> PIND5) & 0x1);
			przyciskWyboru = ((PIND >> PIND6) & 0x1);
			optionsManager(przyciskLewo, przyciskPrawo, przyciskWyboru);
		}
		else if(globalClockMode == EDIT_DATETIME)
		{
			//Tryb edycji czasu i daty
			elapsedTimeB += 1;
			blinkTimeDateMode();
			przyciskLewo = ((PIND >> PIND7) & 0x1);
			przyciskPrawo = ((PIND >> PIND5) & 0x1);
			przyciskWyboru = ((PIND >> PIND6) & 0x1);
			dateTimeManager(przyciskLewo, przyciskPrawo, przyciskWyboru);
		}
		else if(globalClockMode == ALARMS)
		{
			//Tryb edycji alarmów i ich ustawieñ
			//Tryb wyœwietlania listy alarmów
			elapsedTimeB += 1;
			blinkAlarmMode();
			przyciskLewo = ((PIND >> PIND7) & 0x1);
			przyciskPrawo = ((PIND >> PIND5) & 0x1);
			przyciskWyboru = ((PIND >> PIND6) & 0x1);
			alarmOptionsManager(przyciskLewo, przyciskPrawo, przyciskWyboru);
		}
	}

	return 0;
}
