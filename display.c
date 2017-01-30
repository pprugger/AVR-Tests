#include "display.h"

void lcd_data( char data)
{
	// Set RS to 1 for data
	LCD_PORT |= (1<<LCD_RS);
	//Copy
	 char temp = data;
	//Shift high nibble left
	data = data >> 4;
	//Mask upper nibble
	data &= 0b00001111;
	//Data out
	LCD_PORT &= 0b11110000;
	LCD_PORT |= data;
	lcd_enable();

	//Send lower nibble
	temp &= 0b00001111;
	LCD_PORT &= 0b11110000;
	LCD_PORT |= temp;
	lcd_enable();
	_delay_us(50);
}
 
// sendet einen Befehl an das LCD

void lcd_command( char command)
{
	// Set RS to 0 for command
	LCD_PORT &= ~(1<<LCD_RS);
	//Copy
	 char temp = command;
	//Shift high nibble left
	command = command >> 4;
	//Mask upper nibble
	command &= 0b00001111;
	//Data out
	LCD_PORT &= 0b11110000;
	LCD_PORT |= command;
	lcd_enable();

	//Send lower nibble
	temp &= 0b00001111;
	LCD_PORT &= 0b11110000;
	LCD_PORT |= temp;
	lcd_enable();
	_delay_us(50);
}


void lcd_clear()
{
	lcd_command(CLEAR);
	_delay_ms(5);
}

void lcd_home()
{
	lcd_command(HOME);
	_delay_ms(5);
}

void lcd_init(void)
{
   LCD_DDR = LCD_DDR | 0x0F | (1<<LCD_RS) | (1<<LCD_EN);   
 
   _delay_ms(15);
   LCD_PORT &= 0xF0;
   LCD_PORT |= 0x03;            
   LCD_PORT &= ~(1<<LCD_RS);      
   lcd_enable();
 
   _delay_ms(5);
   lcd_enable();
 
   _delay_ms(1);
   lcd_enable();
   _delay_ms(1);
 
   // 4 Bit Modus aktivieren 
   LCD_PORT &= 0xF0;
   LCD_PORT |= 0x02;
   lcd_enable();
   _delay_ms(1);
 
   // 4Bit / 2 Zeilen / 5x7
   lcd_command(0x28);
    
   // Display ein / Cursor aus / kein Blinken
   lcd_command(0x0C); 
 
   // inkrement / kein Scrollen
   lcd_command(0x06);
 
   lcd_clear();
   _delay_ms(200);
}

void lcd_enable()
{
	//Set enable pin
	LCD_PORT |= (1<<LCD_EN);
	//Wait
	//_delay_us(2);	
	_delay_us(100);
	//Clear bit
	LCD_PORT &= ~(1<<LCD_EN);
}


void set_cursor(uint8_t x, uint8_t y)
{
	uint8_t tmp = 0;
 
  	switch (y) 
  	{
		case 1: tmp=0x80+0x00+x; break;    // 1. Zeile
		case 2: tmp=0x80+0x40+x; break;    // 2. Zeile
	}
	lcd_command(tmp);
}

void newline()
{
	lcd_command(0x80+0x40);	
}


void lcd_string( char *str)
{
	while(*str)
	{
		lcd_data(*str);
		str++;
	}
}


void lcd_poweron()
{
	LCD_CTRL_PORT &= ~(1<<LCD_CTRL);
	lcd_init();
}

void lcd_poweroff()
{
	LCD_DDR = 0x00;
	LCD_CTRL_PORT |= (1<<LCD_CTRL);
}