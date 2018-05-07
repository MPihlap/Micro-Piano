/*
 * Klaver.c
 *
 * Created: 07/05/2018 14:19:52
 * Author : meelis
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
uint8_t keyIsHeld = 0;
volatile uint8_t heldKeys[] = {0, 0, 0, 0};
volatile uint8_t key = 0;
ISR(USART1_RX_vect){
	char value = UDR1;
	if (value == '0') {
		//heldKeys[0] = 0;
		key = 1;
	}
	else {
		//heldKeys[0] = UDR1;
		key = value;
	}
	//PORTA = UDR1;
}
void init_uart(){
	UBRR1 = 12;
	UCSR1B = (1 << TXEN1)|(1 << RXEN1)|(1 << RXCIE1);
	UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
}

int main(void)
{
	init_uart();
	sei();
    DDRA = 0xFF;
	PORTA = 0xFF;
    while (1) {
		//PORTA = heldKeys[0];
		PORTA = key;
	}
}

