/*
 * Klaver.c
 *
 * Created: 07/05/2018 14:19:52
 * Author : meelis
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "piano.h"

#define SPEAKER PB6
#define SYSTEM_CLOCK 2000000

void start_timer();
void stop_timer();
volatile uint8_t key = 0;	// Last key that was pressed on keyboard

ISR(TIMER1_COMPA_vect){
	PORTB = PINB ^ (1 << SPEAKER);	// Toggle speaker
}
ISR(USART1_RX_vect){
	char value = UDR1;
	if (value == '0') {	// If silence requested, set key to 0
		key = 0;
	}
	else {	// If a note is to be played, set key to the key pressed
		key = value;
	}
}
uint16_t selectNote(uint8_t key){	// Select musical frequency based on key that was pressed
	switch (key){
		case 'a' :
			return C4;
		case  'w' :
			return D4b;
		case 's' :
			return D4;
		case 'e' :
			return E4b;
		case 'd' :
			return E4;
		case 'f' :
			return F4;
		case 't' :
			return Gb4;
		case 'g' :
			return G4;
		case 'y' :
			return A4b;
		case 'h' :
			return A4;
		case 'u' :
			return B4b;
		case 'j' :
			return B4;
		case 'k' :
			return C5;
		case 'o' :
			return Db5;
		case 'l' :
			return D5;
		case 'p' :
			return Eb5;
		default :
			return SILENCE;
	}
}
uint16_t getCounterLimit(int frequency) {	// Calculate timer limit based on sys clock speed and frequency of note to be played
	return SYSTEM_CLOCK/(frequency*2);
}

void init_uart(){	// 9600 baud, no parity
	UBRR1 = 12;
	UCSR1B = (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1);
	UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
}
void init_timer() {			// NB! DOES NOT START TIMER
	TCCR1A = (1 << COM1A1); // CTC TOP = OCR1A
	TCCR1B = (1 << WGM12);	
	TIMSK1 = 1 << OCIE1A;
}
void start_timer() {	// Start timer by enabling clock
	TCCR1B |= 1 << CS10;
}
void stop_timer() {		// Pause timer by disabling clock
	TCCR1B &= ~(1<<CS10);
}

int main(void)
{
	init_uart();
	init_timer();
	uint16_t activeNote;	// Note currently being played

	sei();					// Enable interrupts
    DDRA = 0xFF;			// Set PORTA leds as output
	DDRB = 1 << SPEAKER;	// Set speaker pin as output
    while (1) {
		PORTA = key;
		activeNote = selectNote(key);	// decipher what note is to be played
		if (activeNote == SILENCE) {	// if no note is to be played, stop timer
			stop_timer();
		}
		else {							// If a sound is to be played, start timer and set limit accordingly
			start_timer();
			OCR1A = getCounterLimit(activeNote);
		}
	}
}

