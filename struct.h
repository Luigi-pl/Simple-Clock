#ifndef STRUCT_H
#define STRUCT_H

typedef union {
	uint8_t value;
	struct {
		uint8_t highlightType:1;
		uint8_t highlightTime:7;
	};
} HighlightControl;

typedef union {
	uint8_t value;
	struct {
		uint8_t timerFlag:1;
		uint8_t alarmFlag:1;
		uint8_t alarmEnableBit:1;
		uint8_t maskFlag:1;
		uint8_t functionMode:2;
		uint8_t holdLastCountFlag:1;
		uint8_t stopCountingFlag:1;
	};
} ControlStatusRegister;

typedef union {
	uint8_t value;
	struct {
		uint8_t unitPlace:4;
		uint8_t tensPlace:4;
	};
} SimpleRegister;

typedef union {
	uint8_t value;
	struct {
		uint8_t unitPlace:4;
		uint8_t tensPlace:2;
		uint8_t AMPMStatus:1;
		uint8_t format:1;
	};
} HoursRegister;

typedef union {
	uint8_t value;
	struct {
		uint8_t dayUnitPlace:4;
		uint8_t dayTensPlace:2;
		uint8_t year:2;
	};
} DateARegister;

typedef union {
	uint8_t value;
	struct {
		uint8_t monthUnitPlace:4;
		uint8_t monthTensPlace:1;
		uint8_t weekdays:3;
	};
} DateBRegister;

typedef union {
	uint8_t value;
	struct {
		uint8_t timerFunction:3;
		uint8_t timerInterruptEnable:1;
		uint8_t clockAlarmFunction:2;
		uint8_t timerAlarmEnable:1;
		uint8_t alarmInterruptEnable:1;
	};
} AlarmControl;

typedef union {
	uint16_t year;
	struct {
		uint8_t valueA;
		uint8_t valueB;
	};
} Year;

typedef union {
	uint16_t value;
	struct {
		uint8_t minutes:6;
		uint8_t hours:5;
		uint8_t weekdays:3;
		uint8_t active:1;
		uint8_t weekly:1;
	};
} AlarmList;

typedef union {
	uint16_t value;
	struct {
		uint8_t nextActiveAlarm:8;
		uint8_t numberOfAlarm:8;
	};
} AlarmListControl;

typedef union {
	uint16_t valueMain;
	struct {
		uint8_t valueA:8;
		uint8_t valueB:8;
	};
} Divider;

typedef struct {
	ControlStatusRegister controlStatusRegister;		//#0
	SimpleRegister secondsRegister;						//#2
	SimpleRegister minutesRegister;						//#3
	HoursRegister hoursRegister;						//#4
	DateARegister dateARegister;						//#5
	DateBRegister dateBRegister;						//#6
	AlarmControl alarmControl;							//#8
	Year year;											//#16
} PCF_Registers;

typedef struct {
	AlarmListControl control;
	AlarmList alarms[20];
} Alarm_Registers;



#endif /* STRUCT_H */