#ifndef PROGRAM_H_
#define PROGRAM_H_

#include <avr/io.h>
#include <inttypes.h>
#include <util/delay.h>
#include "defines.h"
#include "struct.h"

enum mode {STANDARD = 0, HIGHLIGHTED = 1, EDIT_OPTIONS = 2, EDIT_DATETIME = 3, ALARMS = 4};

//////CONFIGURABLE//////
extern volatile PCF_Registers registers;
extern volatile HighlightControl highlightControl;
extern volatile Alarm_Registers alarmRegisters;
extern volatile uint8_t alarmEnableBit;
//////UNCONFIGURABLE//////
extern volatile enum mode globalClockMode;

extern volatile uint8_t buzzerActivated;

extern volatile uint8_t elapsedTime;
extern volatile uint32_t elapsedTimeB;

extern volatile uint8_t alarm;
extern volatile uint8_t alarmChoose;

extern volatile uint8_t option;
extern volatile uint8_t optionChoose;

extern volatile uint8_t dateTime;
extern volatile uint8_t dateTimeChoose;

extern volatile uint8_t alarmOptions;
extern volatile uint8_t alarmOptionsB;
extern volatile uint8_t alarmOptionsChoose;
////////////////////////// LCD DRAW
void drawNumberOnLCD(int, int, int);
void drawTextOnLCD(char*, int, int);

void drawColonAndSlashForTimeAndDate(void);
void drawColonForAlarmEdit(void);
void drawInformationAboutOptionControl(void);
void drawInformationAboutTimeDateControl(void);
void drawInformationAboutAlarmChooseControl(void);
void drawInformationAboutAlarmEditControl(void);
void drawDayOfWeek_Standard(uint8_t);
void drawDayOfWeek_Alarm(uint8_t);
void drawDayOfWeek_AlarmEdit(uint8_t);
void drawStandardMode(void);
void drawOptionsMode(void);
void drawTimeDateMode(void);
void drawAlarmMode(void);
void blinkTimeDateMode(void);
void blinkAlarmMode(void);
void switchHighlightOnLCD(void);
void editMode(void);
////////////////////////// TIME and DATE
void changeHours(uint8_t, int8_t);
void changeMinutes(uint8_t, int8_t);
void changeWeekdays(uint8_t, int8_t);
void changeDays(uint8_t, int8_t);
void changeMonths(uint8_t, int8_t);
void changeYears(uint8_t, int8_t);
////////////////////////// MODES
void changeMode(void);

void changeAlarm(int8_t);
void alarmManager(int8_t, int8_t, int8_t);

void changeOptions(int8_t);
void changeOptionsValue(int8_t);
void optionsManager(int8_t, int8_t, int8_t);

void changeDateTime(int8_t);
void changeDateTimeValue(int8_t);
void dateTimeManager(int8_t, int8_t, int8_t);

void changeAlarmOptions(int8_t);
void changeAlarmOptionsValue(int8_t);
void alarmOptionsManager(int8_t, int8_t, int8_t);
///////////////////////// BUZZER
void stopBuzzer(void);
void startBuzzer(void);
void checkAlarm(void);
#endif /* PROGRAM_H_ */