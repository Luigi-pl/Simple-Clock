#include "PCF8583.h"
#include "struct.h"
void PCF_init(void)
{
	i2c_init();
}

//Ustawianie trybu dzia³ania
void PCF_setMode(uint8_t mode)
{
	i2c_write(mode);
}

//Ustawienie trybu dzia³ania na zapis i wybranie rejestru reg
void PCF_writeMode(uint8_t reg)
{
	i2c_start();
	PCF_setMode(RFC8583_WRITE_ADDRESS);
	i2c_write(reg);
}

//Ustawienie trybu dzia³ania na odczyt, zaczynaj¹c od rejestru reg 
void PCF_readMode(uint8_t reg)
{
	i2c_start();
	//Wybranie rejestru do odczytu
	PCF_setMode(RFC8583_WRITE_ADDRESS);
	i2c_write(reg);
	i2c_start();
	PCF_setMode(RFC8583_READ_ADDRESS);
}

//Zapisanie do aktualnie wybranego rejestru wartoœci value
//ack odpowiada za zakoñczenie nadawania (NOACK) lub jego kontynuace (ACK)
void PCF_writeRegister(uint8_t value, uint8_t ack)
{
	i2c_write(value);
	//Fragment zwi¹zany z koñczeniem wysy³ania
	if(ack == NOACK)
	{
		i2c_stop();
	}
}
//Odczytanie aktualnie wybranego rejestru
//ack odpowiada za zakoñczenie odbierania (NOACK) lub jego kontynuace (ACK)
uint8_t PCF_readRegister(uint8_t ack)
{
	uint8_t registerValue = i2c_read(ack);
	if(ack == NOACK)
	{
		i2c_stop();
	}
	return registerValue;
}

//Funkcja s³u¿y do odczytania wybranych rejestrów, przechowuj¹cych dane zwi¹zane z czasem, dat¹ i ustawieniami.
void PCF_readRegisters(PCF_Registers* data)
{
	int i = 0;
	int last = 10;
	uint8_t registerValue[11];
	
	PCF_readMode(START_REGISTER);
	
	for(; i < last - 2; i++)
	{
		registerValue[i] = PCF_readRegister(ACK);
	}
	registerValue[last - 2] = PCF_readRegister(NOACK);
	
	////////////////////////////////////////
	
	PCF_readMode(YEAR_REGISTER);

	registerValue[last - 1] = PCF_readRegister(ACK);
	registerValue[last] = PCF_readRegister(NOACK);
	
	////////////////////////////////////////	
	data->controlStatusRegister.value	= registerValue[0];
	data->secondsRegister.value			= registerValue[2];
	data->minutesRegister.value			= registerValue[3];
	data->hoursRegister.value			= registerValue[4];
	data->dateARegister.value			= registerValue[5];
	data->dateBRegister.value 			= registerValue[6];
	data->alarmControl.value 			= registerValue[8];
	data->year.valueA 					= registerValue[9];
	data->year.valueB					= registerValue[10];
}

//Funkcja wy³¹cza zegar w module PCF8583
void PCF_stopClock(ControlStatusRegister* data)
{
	data->holdLastCountFlag = 1;
	data->stopCountingFlag = 1;

	PCF_writeMode(CONTROL_STATUS_REGISTER);
	PCF_writeRegister(data->value, NOACK);
}

//Funkcja w³¹cza zegar w module PCF8583
void PCF_startClock(ControlStatusRegister* data)
{
	data->holdLastCountFlag = 0;
	data->stopCountingFlag = 0;

	PCF_writeMode(CONTROL_STATUS_REGISTER);
	PCF_writeRegister(data->value, NOACK);
}

//Funkcja wczytuje ustawienia podœwietlenia dla zegara
void PCF_loadHighlightSettings(HighlightControl* data)
{
	PCF_readMode(HIGHLIGHT_REGISTER);
	data->value = PCF_readRegister(NOACK);
}

//Funkcja zapisuje ustawienia podœwietlenia dla zegara
void PCF_saveHighlightSettings(HighlightControl* data)
{
	PCF_writeMode(HIGHLIGHT_REGISTER);
	PCF_writeRegister(data->value, NOACK);
}

//Funkcja zapisuje ustawienia kontrolne zegara
void PCF_saveControlSettings(PCF_Registers* data)
{
	PCF_writeMode(CONTROL_STATUS_REGISTER);
	PCF_writeRegister(data->controlStatusRegister.value, NOACK);
}

//Funkcja s³u¿y do zapisywania zmian w rejestrach zwi¹zanych z czasem na module PCF8583
void PCF_saveTimeDate(PCF_Registers* data)
{
	PCF_stopClock(&(data->controlStatusRegister));
	
	PCF_writeMode(SECONDS_REGISTER);
	PCF_writeRegister(data->secondsRegister.value, ACK);
	PCF_writeRegister(data->minutesRegister.value, ACK);
	PCF_writeRegister(data->hoursRegister.value, ACK);
	PCF_writeRegister(data->dateARegister.value, ACK);
	PCF_writeRegister(data->dateBRegister.value, ACK);
	
	PCF_writeMode(ALARM_REGISTER);
	PCF_writeRegister(data->alarmControl.value, ACK);
	
	PCF_writeMode(YEAR_REGISTER);
	PCF_writeRegister(data->year.valueA, ACK);
	PCF_writeRegister(data->year.valueB, NOACK);
	
	PCF_startClock(&(data->controlStatusRegister));
}
//Funkcja wczytuje liste alarmów
void PCF_loadAlarmList(Alarm_Registers* alarms)
{
	uint8_t i = 0;
	PCF_readMode(ALARM_CONTROL_LIST_REGISTER);
	alarms->control.value = (PCF_readRegister(ACK) << 8) | (PCF_readRegister(ACK));
	for(; i < 19; i++)
	{
		alarms->alarms[i].value = (PCF_readRegister(ACK) << 8) | (PCF_readRegister(ACK));
	}
	
	alarms->alarms[19].value = (PCF_readRegister(ACK) << 8) | (PCF_readRegister(NOACK));
}
//Funkcja zapisuje liste alarmów
void PCF_saveAlarmList(Alarm_Registers* alarms)
{
	Divider divider;
	uint8_t i = 0;
	PCF_writeMode(ALARM_CONTROL_LIST_REGISTER);
	
	divider.valueMain = alarms->control.value;
	PCF_writeRegister(divider.valueB, ACK);
	PCF_writeRegister(divider.valueA, ACK);
	
	for(; i < 19; i++)
	{
		divider.valueMain= alarms->alarms[i].value;
		PCF_writeRegister(divider.valueB, ACK);
		PCF_writeRegister(divider.valueA, ACK);
	}
	divider.valueMain = alarms->alarms[19].value;
	PCF_writeRegister(divider.valueB, ACK);
	PCF_writeRegister(divider.valueA, NOACK);
}
//Funkcja zapisuje ustawienia alarmów
void PCF_saveAlarmListControl(AlarmListControl* alarmListControl)
{
	Divider divider;
	PCF_writeMode(ALARM_CONTROL_LIST_REGISTER);
	divider.valueMain = alarmListControl->value;
	
	PCF_writeRegister(divider.valueB, ACK);
	PCF_writeRegister(divider.valueA, NOACK);
}