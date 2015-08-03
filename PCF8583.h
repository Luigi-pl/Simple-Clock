#ifndef PCF8583_H
#define PCF8583_H

#include "program.h"
#include "i2c.h"
void PCF_init(void);

void PCF_setMode(uint8_t);
void PCF_writeMode(uint8_t);
void PCF_readMode(uint8_t);

void PCF_writeRegister(uint8_t, uint8_t);
uint8_t PCF_readRegister(uint8_t);

void PCF_readRegisters(PCF_Registers*);
//////////////////////////////////////////////
void PCF_stopClock(ControlStatusRegister*);
void PCF_startClock(ControlStatusRegister*);

void PCF_loadHighlightSettings(HighlightControl*);
void PCF_saveHighlightSettings(HighlightControl*);
void PCF_saveControlSettings(PCF_Registers*);
void PCF_saveTimeDate(PCF_Registers*);
void PCF_loadAlarmList(Alarm_Registers*);
void PCF_saveAlarmList(Alarm_Registers*);
void PCF_saveAlarmListControl(AlarmListControl*);
#endif /* PCF8583_H*/
