#ifndef DISPLAY_H
#define DISPLAY_H

#ifndef F_CPU
/* Definiere F_CPU, wenn F_CPU nicht bereits vorher definiert 
   (z.&nbsp;B. durch Übergabe als Parameter zum Compiler innerhalb 
   des Makefiles). Zusätzlich Ausgabe einer Warnung, die auf die
   "nachträgliche" Definition hinweist */
//#warning "F_CPU war noch nicht definiert, wird nun mit 1000000 definiert"*/
#define F_CPU 12000000UL     /* 12 Mhz */
#endif
#define MAX_DELAY (262UL / (F_CPU / 1000000UL))

#include <avr/io.h>
#include <util/delay.h>  


/* HD44780 compatible controllers only */

/* Constants */
#define LCD_PORT PORTC	//D4..D7 on PC0..PC3
#define LCD_RS PC6	//RS Low means command, high means data
#define LCD_EN PC4	
#define LCD_DDR DDRC
#define LCD_CTRL PD4
#define LCD_CTRL_PORT PORTD


/* LCD commands */
#define CLEAR 							0x01
#define HOME  							0x02
#define ENTRYMODE_CURSOR_SHIFT_RIGHT 	0x06
#define ENTRYMODE_CURSOR_SHIFT_LEFT  	0x04
#define ENTRYMODE_DISPLAY_SHIFT_RIGHT 	0x07
#define ENTRYMODE_DISPLAY_SHIFT_LEFT	0x05
#define OFF								0x08
#define ON								0x0C
#define SETSYSTEMPROP					0x28


void lcd_data( char data);	//Transmits data to the display
void lcd_command( char command);	//Sends a command to the display
void lcd_enable();	//Enables the lcd
void lcd_init();
void lcd_clear();
void lcd_home();
void lcd_string( char *str);
void set_cursor(uint8_t x, uint8_t y);
void newline();
void lcd_poweron();
void lcd_poweroff();

#endif
