#ifndef DEFINES_H_
#define DEFINES_H_

/////////////////////////////
#define PORT(x) XPORT(x)
#define XPORT(x) (PORT##x)

#define PIN(x) XPIN(x)
#define XPIN(x) (PIN##x)

#define DDR(x) XDDR(x)
#define XDDR(x) (DDR##x)
/////////////////////////////
#define LCD_PORTS A
#define LCD_DATA_PORT A
#define LCD_RS_PORT A
#define LCD_RW_PORT A
#define LCD_E_PORT A

#define LCD_HIGHLIGHT_PORT C
#define I2C_PORT C

#define INTERRUPT_PORT D
#define BUTTON_PORT D
#define BUTTON_PIN D
#define BUZZER_PORT D

#define LCD_D4_PIN 3
#define LCD_D5_PIN 4
#define LCD_D6_PIN 5
#define LCD_D7_PIN 6
#define LCD_RS_PIN 0
#define LCD_RW_PIN 1
#define LCD_E_PIN 2

#define LCD_HIGHLIGHT_PIN 7
#define I2C_SDA_PIN 1
#define I2C_SCL_PIN 0

#define BUTTON_LEFT_PIN 7
#define BUTTON_CHOOSE_PIN 6
#define BUTTON_RIGHT_PIN 5
#define BUTTON_MODE_PIN 4
#define BUZZER_PIN 3
#define MODE_INTERRUPT_PIN 3
#define TIME_INTERRUPT_PIN 2
///////////////////////////// I2C access data to RTC - PCF8583
#define RFC8583_WRITE_ADDRESS 0xA2
#define RFC8583_READ_ADDRESS 0xA3
///////////////////////////// TIME, DATE, ALRAM positions on LCD DISPLAY
#define TIME_POSITION 4
#define DATE_POSITION 5
#define ALARM_POSITION 0
#define ALARM_EDIT_POSITION_A 3
#define ALARM_EDIT_POSITION_B 5

#define OPTION_TEXT_FIRST 0
#define OPTION_TEXT_LAST 15
#define OPTION_LEFT 0
#define OPTION_RIGHT 11

#define TIME_ROW 0
#define DATE_ROW 1
#define ALARM_ROW 3
#define ALARM_EDIT_ROW_A 1
#define ALARM_EDIT_ROW_B 0

#define OPTION_TEXT_1_ROW 0
#define OPTION_TEXT_2_ROW 1
#define OPTION_1_ROW 2
#define OPTION_2_ROW 3
///////////////////////////// LCD DISPLAY - HD44780
#define HD44780_CLEAR					0x01

#define HD44780_HOME					0x02

#define HD44780_ENTRY_MODE				0x04
	#define HD44780_EM_SHIFT_CURSOR		0
	#define HD44780_EM_SHIFT_DISPLAY	1
	#define HD44780_EM_DECREMENT		0
	#define HD44780_EM_INCREMENT		2

#define HD44780_DISPLAY_ONOFF			0x08
	#define HD44780_DISPLAY_OFF			0
	#define HD44780_DISPLAY_ON			4
	#define HD44780_CURSOR_OFF			0
	#define HD44780_CURSOR_ON			2
	#define HD44780_CURSOR_NOBLINK		0
	#define HD44780_CURSOR_BLINK		1

#define HD44780_DISPLAY_CURSOR_SHIFT	0x10
	#define HD44780_SHIFT_CURSOR		0
	#define HD44780_SHIFT_DISPLAY		8
	#define HD44780_SHIFT_LEFT			0
	#define HD44780_SHIFT_RIGHT			4

#define HD44780_FUNCTION_SET			0x20
	#define HD44780_FONT5x7				0
	#define HD44780_FONT5x10			4
	#define HD44780_ONE_LINE			0
	#define HD44780_TWO_LINE			8
	#define HD44780_SHIFT				0x10
	#define HD44780_4_BIT				0
	#define HD44780_8_BIT				16

#define HD44780_CGRAM_SET				0x40

#define HD44780_DDRAM_SET				0x80
///////////////////////////// I2C codes
#define ACK 1
#define NOACK 0
///////////////////////////// RTC PCF8583 registers
#define START_REGISTER 0x00

#define CONTROL_STATUS_REGISTER 0x00
#define SECONDS_REGISTER 0x02
#define MINUTES_REGISTER 0x03
#define HOURS_REGISTER 0x04
#define DATEA_REGISTER 0x05
#define DATEB_REGISTER 0x06
#define ALARM_REGISTER 0x08
#define YEAR_REGISTER 0x10
#define HIGHLIGHT_REGISTER 0x12
#define ALARM_CONTROL_LIST_REGISTER 0x13
#define ALARM_LIST_REGISTER 0x15
/////////////////////////////

#endif /* DEFINES_H_ */