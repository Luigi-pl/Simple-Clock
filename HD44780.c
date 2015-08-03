#include "HD44780.h"

void LCD_out_bits(uint8_t bits_to_write)
{
	if(bits_to_write & 0x01)
		PORT(LCD_DATA_PORT) |= (1<<LCD_D4_PIN);
	else
		PORT(LCD_DATA_PORT)  &= ~(1<<LCD_D4_PIN);

	if(bits_to_write & 0x02)
		PORT(LCD_DATA_PORT) |= (1<<LCD_D5_PIN);
	else
		PORT(LCD_DATA_PORT)  &= ~(1<<LCD_D5_PIN);

	if(bits_to_write & 0x04)
		PORT(LCD_DATA_PORT) |= (1<<LCD_D6_PIN);
	else
		PORT(LCD_DATA_PORT)  &= ~(1<<LCD_D6_PIN);

	if(bits_to_write & 0x08)
		PORT(LCD_DATA_PORT) |= (1<<LCD_D7_PIN);
	else
		PORT(LCD_DATA_PORT)  &= ~(1<<LCD_D7_PIN);
}

uint8_t LCD_in_bits(void)
{
	uint8_t tmp = 0;

	if(((PIN(LCD_DATA_PORT) >> LCD_D4_PIN) & 0x01) != 0)
		tmp |= (1 << 0);
	if(((PIN(LCD_DATA_PORT) >> LCD_D5_PIN) & 0x01) != 0)
		tmp |= (1 << 1);
	if(((PIN(LCD_DATA_PORT) >> LCD_D6_PIN) & 0x01) != 0)
		tmp |= (1 << 2);
	if(((PIN(LCD_DATA_PORT) >> LCD_D7_PIN) & 0x01) != 0)
		tmp |= (1 << 3);
		
	return tmp;
}

void LCD_write(uint8_t data_to_write)
{
	DDR(LCD_DATA_PORT) |= ((1<<LCD_D4_PIN)|(1<<LCD_D5_PIN)|(1<<LCD_D6_PIN)|(1<<LCD_D7_PIN));

	PORT(LCD_RW_PORT) &= ~(1<<LCD_RW_PIN);
	PORT(LCD_E_PORT) |= (1<<LCD_E_PIN);
	LCD_out_bits(data_to_write >> 4);
	PORT(LCD_E_PORT) &= ~(1<<LCD_E_PIN);
	_delay_ms(1);
	PORT(LCD_E_PORT) |= (1<<LCD_E_PIN);
	LCD_out_bits(data_to_write);
	PORT(LCD_E_PORT) &= ~(1<<LCD_E_PIN);
	_delay_ms(1);
	
	while(LCD_ReadStatus() & 0x80);
}

void LCD_just_write(uint8_t data_to_write)
{
	DDR(LCD_DATA_PORT) |= ((1<<LCD_D4_PIN)|(1<<LCD_D5_PIN)|(1<<LCD_D6_PIN)|(1<<LCD_D7_PIN));

	PORT(LCD_RW_PORT) &= ~(1<<LCD_RW_PIN);
	PORT(LCD_E_PORT) |= (1<<LCD_E_PIN);
	LCD_out_bits(data_to_write >> 4);
	PORT(LCD_E_PORT) &= ~(1<<LCD_E_PIN);
	_delay_ms(1);
	PORT(LCD_E_PORT) |= (1<<LCD_E_PIN);
	LCD_out_bits(data_to_write);
	PORT(LCD_E_PORT) &= ~(1<<LCD_E_PIN);
	_delay_ms(1);
}

void LCD_just_write_command(uint8_t commandToWrite)
{
	PORT(LCD_RS_PORT) &= ~(1<<LCD_RS_PIN);
	LCD_just_write(commandToWrite);
}

void LCD_just_write_data(uint8_t data_to_write)
{
	PORT(LCD_RS_PORT) |= (1<<LCD_RS_PIN);
	LCD_just_write(data_to_write);
}

uint8_t LCD_NotBusy(void)
{
	if(LCD_ReadStatus() != 0x80)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

uint8_t LCD_read(void)
{
	uint8_t tmp = 0;

	DDR(LCD_DATA_PORT) &= ~((1<<LCD_D4_PIN)|(1<<LCD_D5_PIN)|(1<<LCD_D6_PIN)|(1<<LCD_D7_PIN));

	PORT(LCD_RW_PORT) |= (1<<LCD_RW_PIN);
	PORT(LCD_E_PORT) |= (1<<LCD_E_PIN);
	tmp |= (LCD_in_bits() << 4);
	PORT(LCD_E_PORT) &= ~(1<<LCD_E_PIN);
	_delay_ms(1);
	PORT(LCD_E_PORT) |= (1<<LCD_E_PIN);
	tmp |= LCD_in_bits();
	PORT(LCD_E_PORT) &= ~(1<<LCD_E_PIN);
	
	return tmp;
}

void LCD_write_command(uint8_t commandToWrite)
{
	PORT(LCD_RS_PORT) &= ~(1<<LCD_RS_PIN);
	LCD_write(commandToWrite);
}

uint8_t LCD_ReadStatus(void)
{
	PORT(LCD_RS_PORT) &= ~(1<<LCD_RS_PIN);
	
	return LCD_read();
}

void LCD_write_data(uint8_t data_to_write)
{
	PORT(LCD_RS_PORT) |= (1<<LCD_RS_PIN);
	LCD_write(data_to_write);
}

uint8_t LCD_read_data(void)
{
	PORT(LCD_RS_PORT) |= (1<<LCD_RS_PIN);
	
	return LCD_read();
}

void LCD_write_text(char * text)
{
	while(*text != '\0' && *text != 0)
	{
		LCD_write_data(*text++);
	}
}

void LCD_goto(uint8_t x, uint8_t y)
{
	uint8_t shift = 0;
	if(y >= 2)
	{
		shift = HD44780_SHIFT;
	}
	LCD_write_command(HD44780_DDRAM_SET | (shift + x + (0x40 * (y % 2))));
}

void LCD_clear(void)
{
	LCD_write_command(HD44780_CLEAR);
	_delay_ms(2);
}

void LCD_home(void)
{
	LCD_write_command(HD44780_HOME);
	_delay_ms(2);
}

void LCD_init(void)
{
	uint8_t i;

	DDR(LCD_DATA_PORT) |= ((1<<LCD_D4_PIN)|(1<<LCD_D5_PIN)|(1<<LCD_D6_PIN)|(1<<LCD_D7_PIN));
	DDR(LCD_RS_PORT) |= (1<<LCD_RS_PIN);
	DDR(LCD_RW_PORT) |= (1<<LCD_RW_PIN);
	DDR(LCD_E_PORT) |= (1<<LCD_E_PIN);

	_delay_ms(15);
	PORT(LCD_RS_PORT) &= ~(1<<LCD_RS_PIN);
	PORT(LCD_E_PORT) &= ~(1<<LCD_E_PIN);
	PORT(LCD_RW_PORT) &= ~(1<<LCD_RW_PIN);
	
	for(i = 0; i < 3; i++)
	{
		PORT(LCD_E_PORT) |= (1<<LCD_E_PIN);
		LCD_out_bits(0x03);
		PORT(LCD_E_PORT) &= ~(1<<LCD_E_PIN);
		_delay_ms(5);
	}

	PORT(LCD_E_PORT) |= (1<<LCD_E_PIN);
	LCD_out_bits(0x02);
	PORT(LCD_E_PORT) &= ~(1<<LCD_E_PIN);

	_delay_ms(1);
	LCD_write_command(HD44780_FUNCTION_SET | HD44780_FONT5x7 | HD44780_TWO_LINE | HD44780_4_BIT);
	LCD_write_command(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_OFF);
	LCD_write_command(HD44780_CLEAR);
	LCD_write_command(HD44780_ENTRY_MODE | HD44780_EM_SHIFT_CURSOR | HD44780_EM_INCREMENT);
	LCD_write_command(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_OFF | HD44780_CURSOR_NOBLINK);
}
