#include <avr/interrupt.h>
#include <stdlib.h>
#include "program.h"
#include "HD44780.h"
#include "i2c.h"
#include "pcf8583.h"

//Wy³¹czanie dŸwiêku alarmu
void stopBuzzer(void)
{
	uint8_t i = 0;
	uint8_t min = 0;
	uint8_t max = 19;
	
	buzzerActivated = 0;
	PORT(BUZZER_PORT) &= ~((1 << BUZZER_PIN));
	
	if(alarmRegisters.alarms[alarmRegisters.control.nextActiveAlarm].weekly == 0)
	{
		alarmRegisters.alarms[alarmRegisters.control.nextActiveAlarm].active = 0;
	}
	
	for(; i <= max; i++)
	{
		alarm += 1;
		if(alarm > max)
		{
			alarm = min;
		}
		
		if(alarmRegisters.alarms[alarm].active == 1)
		{
			alarmRegisters.control.nextActiveAlarm = alarm;
			PCF_saveAlarmListControl(&(alarmRegisters.control));
			break;
		}
	}
}

//W³¹czanie dŸwiêku alarmu
void startBuzzer(void)
{
	buzzerActivated = 1;
	PORT(BUZZER_PORT) |= ((1 << BUZZER_PIN));
}

//Funkcja s³u¿y do uruchomienia alarmu we w³aœciwym momencie
void checkAlarm(void)
{
	uint8_t sec = registers.secondsRegister.tensPlace * 10 + registers.secondsRegister.unitPlace;
	if(buzzerActivated == 0 && alarmEnableBit == 1 && sec == 0)
	{
		uint8_t minutes = registers.minutesRegister.tensPlace * 10 + registers.minutesRegister.unitPlace;
		uint8_t hours = registers.hoursRegister.tensPlace * 10 + registers.hoursRegister.unitPlace;
		uint8_t weekdays = registers.dateBRegister.weekdays;
		AlarmList alarm = alarmRegisters.alarms[alarmRegisters.control.nextActiveAlarm];
		if(alarm.active == 1 && alarm.hours == hours && alarm.minutes == minutes && alarm.weekdays == weekdays)
		{
			startBuzzer();
		}
	}
}