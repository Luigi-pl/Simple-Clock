#ifndef HD44780_H_
#define HD44780_H_

#include "program.h"

void LCD_out_bits(uint8_t);

uint8_t LCD_in_bits(void);

void LCD_write(uint8_t);

void LCD_just_write(uint8_t);

void LCD_just_write_command(uint8_t);

void LCD_just_write_data(uint8_t);

uint8_t LCD_NotBusy(void);

uint8_t LCD_read(void);

void LCD_write_command(uint8_t);

uint8_t LCD_ReadStatus(void);

void LCD_write_data(uint8_t);

uint8_t LCD_read_data(void);

void LCD_write_text(char *);

void LCD_goto(uint8_t, uint8_t);

void LCD_clear(void);

void LCD_home(void);

void LCD_init(void);


#endif /* HD44780_H_ */