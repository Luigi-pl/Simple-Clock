#include "i2c.h"

void i2c_init(void)
{
	//ZMIENIÆ F_CPU na 16000000, tak aby TWBR = 12 => SCL FREQ = 400kHZ => 400 kbps
  TWSR = 0;
  TWBR = 12;//(F_CPU / 100000UL - 16)/2;	 
  TWCR |= (1<<TWINT);
}

void i2c_start(void)
{
    TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

void i2c_stop(void)
{
    TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);
    while ((TWCR & (1<<TWSTO)) != 0);
}

void i2c_write(uint8_t data)
{
    TWDR = data;
    TWCR = (1<<TWINT) | (1<<TWEN);
    while ((TWCR & (1<<TWINT)) == 0);
}

uint8_t i2c_read(uint8_t ack)
{
    TWCR = ack ? ((1 << TWINT) | (1 << TWEN) | (1 << TWEA)) : ((1 << TWINT) | (1 << TWEN)) ;
    while ((TWCR & (1<<TWINT)) == 0);
    return TWDR;
}
