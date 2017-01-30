/*
 * GccApplication2.c
 *
 * Created: 29.11.2016 21:54:49
 *  Author: pprugger
 */ 

#ifndef F_CPU
/* Definiere F_CPU, wenn F_CPU nicht bereits vorher definiert 
   (z.&nbsp;B. durch Übergabe als Parameter zum Compiler innerhalb 
   des Makefiles). Zusätzlich Ausgabe einer Warnung, die auf die
   "nachträgliche" Definition hinweist */
//#warning "F_CPU war noch nicht definiert, wird nun mit 1000000 definiert"*/
#define F_CPU 12000000UL     /* 12 Mhz */
#endif
#define MAX_DELAY (262UL / (F_CPU / 1000000UL))
#define BAUD 9600UL
#define UBRR_VAL ((F_CPU+BAUD*8)/(BAUD*16)-1)
#define BUF_LEN 128
#define CMD_NAME_LEN 16
#include <avr/io.h>
#include <util/delay.h>  
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include "display.h"
#include "stdio.h"
#include "string.h"

enum CMD_FLAG
{
	CMD_IDLE,
	CMD_RECEIVED
};

volatile uint8_t cmd_flag = CMD_IDLE;
const static uint8_t buf_len = BUF_LEN;
volatile char cmd_buffer[BUF_LEN];
volatile uint8_t buf_ptr = 0;
volatile uint16_t nsecs = 0;
volatile uint16_t hour = 0;
volatile uint16_t min = 0;
char cmdline[BUF_LEN];

void delay_var_ms(uint16_t ms)
{
		uint16_t nFullDelays = ms / MAX_DELAY;
		uint8_t remainder = ms % MAX_DELAY;
		
		for(uint16_t nDelay = 0; nDelay < nFullDelays; nDelay++)
		{
			_delay_ms(MAX_DELAY);
		}
		
		//delay remainder 1ms wise
		for(uint8_t nMillisec = 0; nMillisec < remainder; nMillisec++)
		{
			_delay_ms(1);
		}
}


void uart_init(void)
{
	UBRR0H = UBRR_VAL >> 8;
	UBRR0L = UBRR_VAL & 0xFF;
	UCSR0B |= (1<<TXEN0) | (1<<RXEN0);
	UCSR0B |=  (1<<RXCIE0);
	UCSR0C = (1<< UCSZ00) | (1<<UCSZ01);
	DDRD |= (1<<PD1);
	_delay_ms(50);
}

void uart_putc(char c)
{
	while(!(UCSR0A & (1<<UDRE0)))
	{
		;
	}

	UDR0 = c;
}

void uart_puts(char *s)
{
	while(*s)
	{
		uart_putc(*s);
		s++;
	}
}

void proc_command(char *str)
{
	strcpy((char*)cmd_buffer, str);
}

ISR(USART0_RX_vect)
{
	char c = UDR0;

	if(c == '\r')
	{
		cmd_flag = CMD_RECEIVED;
		cmd_buffer[buf_ptr] = 0x00;
	}
	//else if(c == '\b')
	//{
		//if(buf_ptr > 0)
		//{
			//buf_ptr--;
			//uart_putc('\b');
			//uart_putc(' ');
			//uart_putc('\b');
		//}
	//}
	else
	{
		 if(buf_ptr < (buf_len-1))
		 {
			cmd_buffer[buf_ptr] = c;
			buf_ptr++;
			uart_putc(c);
		 }
	}
}

void timer_init(void)
{	
	const uint16_t timer_constant = 46875;
	TCCR1B |= (1<<CS12);
	TCCR1B |= (1<<WGM12);
	OCR1AH = timer_constant >> 8;
	OCR1AL = timer_constant & 0xFF;
	TIMSK1 |= (1<<OCIE1A);
	_delay_ms(50);
}

ISR(TIMER1_COMPA_vect)
{
	nsecs++;
	if (nsecs > 59)
	{
		nsecs = 0;
		min++;
	}
	
	if (min > 59)
	{
		min = 0;
		hour++;
	}
	
	if (hour == 24)
	{
		hour = 0;
	}
}

void led_init(void)
{
	PORTD &= ~((1<<PD2) | (1<<PD3));
	DDRD |= ((1<<PD2) | (1<<PD3));
	PORTD |= ((1<<PD2) | (1<<PD3));
}

void lcd_on_off_ctrl_init(void)
{
	PORTD &= ~(1<<PD4);
	DDRD |= (1<<PD4);
}

int main(void)
{	
	char buffer[16];

	led_init();
	uart_init();
	_delay_ms(200);
	lcd_on_off_ctrl_init();
	lcd_init();
	timer_init();
	sei();

	uart_puts("Test Test\n\r");
//
	//while(1)
	//{
		//uart_putc('O');
	//}

	 while(1)
	 {
			if(cmd_flag == CMD_RECEIVED)
			{
				cmd_flag = CMD_IDLE;
				buf_ptr = 0;
				uart_putc('\n');
				uart_putc('\r');
			
				if(strcmp((char*)cmd_buffer, "Led on") == 0)
				{
					lcd_clear();
					lcd_string("Led on");
					PORTD |= ((1<<PD2) | (1<<PD3));
					_delay_ms(5000);
				}

				if(strcmp((char*)cmd_buffer, "Led off") == 0)
				{
					lcd_clear();
					lcd_string("Led off");
					PORTD &= ~((1<<PD2) | (1<<PD3));
					_delay_ms(5000);
				}

				if(strcmp((char*)cmd_buffer, "Lcd on") == 0)
				{
					lcd_poweron();
				}

				if(strcmp((char*)cmd_buffer, "Lcd off") == 0)
				{
					lcd_poweroff();
				}

				if(strcmp((char*)cmd_buffer, "sleep") == 0)
				{	
					_delay_ms(10);
					set_sleep_mode(SLEEP_MODE_PWR_DOWN);
					sleep_mode();
				}
			}
		 lcd_clear();
		 sprintf(buffer, "%02d:%02d:%02d", hour, min, nsecs);
		 lcd_string(buffer);
		 _delay_ms(1000);
	 }

	return 0;
}