void leds_on_start_left (void)
{
	PORTD |= (1<<PD0);
	delay_var_ms(100);
	
	PORTD |= (1<<PD1);
	delay_var_ms(100);
	
	PORTD |= (1<<PD2);
	delay_var_ms(100);
	
	PORTD |= (1<<PD3);
	delay_var_ms(100);
}

void leds_on_start_right (void)
{
	PORTD |= (1<<PD3);
	delay_var_ms(100);
	
	PORTD |= (1<<PD2);
	delay_var_ms(100);
	
	PORTD |= (1<<PD1);
	delay_var_ms(100);
	
	PORTD |= (1<<PD0);
	delay_var_ms(100);
}

void leds_off_start_left (void)
{
	PORTD &= ~(1<<PD0);
	delay_var_ms(100);
	
	PORTD &= ~(1<<PD1);
	delay_var_ms(100);
	
	PORTD &= ~(1<<PD2);
	delay_var_ms(100);
	
	PORTD &= ~(1<<PD3);
	delay_var_ms(100);
}

void leds_off_start_right (void)
{
	PORTD &= ~(1<<PD3);
	delay_var_ms(100);
	
	PORTD &= ~(1<<PD2);
	delay_var_ms(100);
	
	PORTD &= ~(1<<PD1);
	delay_var_ms(100);
	
	PORTD &= ~(1<<PD0);
	delay_var_ms(100);
}