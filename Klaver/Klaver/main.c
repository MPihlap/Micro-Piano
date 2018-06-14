/*
 * Klaver.c
 *
 * Created: 07/05/2018 14:19:52
 * Author : meelis
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#define SPEAKER PB6
#define SYSTEM_CLOCK 2000000
#define PRESCALER 1
#define	C4 261
#define D4b 277
#define D4 293
#define E4b 311
#define E4 329
#define F4 349
#define Gb4 370
#define G4 392
#define A4b 415
#define A4 440
#define B4b 466
#define B4 493
#define C5 523
#define Db5 554
#define D5 587
#define Eb5 622
#define E5 659

void start_timer();
void stop_timer();
volatile uint16_t activeNote = E4;
volatile uint8_t heldKeys[] = {0, 0, 0, 0};
volatile uint8_t key = 0;
volatile uint8_t lastKey = 0;

ISR(TIMER1_COMPA_vect){
	PORTB = PINB ^ (1 << SPEAKER);
}
ISR(USART1_RX_vect){
	char value = UDR1;
	if (value == '0') {
		//heldKeys[0] = 0;
		key = 1;
		stop_timer();
	}
	else {
		//heldKeys[0] = UDR1;
		lastKey = key;
		key = value;
		switch (key){
			case 'a' :
				activeNote = C4;
				break;
			case  'w' :
				activeNote = D4b;
				break;
			case 's' :
				activeNote = D4;
				break;
			case 'e' :
				activeNote = E4b;
				break;
			case 'd' :
				activeNote = E4;
				break;
			case 'f' :
				activeNote = F4;
				break;
			case 't' :
				activeNote = Gb4;
				break;
			case 'g' :
				activeNote = G4;
				break;
			case 'y' :
				activeNote = A4b;
				break;
			case 'h' :
				activeNote = A4;
				break;
			case 'u' :
				activeNote = B4b;
				break;
			case 'j' :
				activeNote = B4;
				break;
			case 'k' :
				activeNote = C5;
				break;
			case 'o' :
				activeNote = Db5;
				break;
			case 'l' :
				activeNote = D5;
				break;
			case 'p' :
				activeNote = Eb5;
				break;
			case 'ö' :
				activeNote = E5;
				break;
			default :
				key = 1;
				stop_timer();
				break;
			
		}
		if (key != 1) {
			start_timer();
		}
		
	}
}
uint16_t getCounterLimit(int frequency) {
	return SYSTEM_CLOCK/(frequency*2);
}
void send_uart(char word){
	while (!(UCSR1A & (1 << UDRE1))) {
		asm("nop");
	}
	UDR1 = word;
}
void init_uart(){
	UBRR1 = 12;
	UCSR1B = (1 << TXEN1) | (1 << RXEN1) | (1 << RXCIE1);
	UCSR1C = (1 << UCSZ10) | (1 << UCSZ11);
}
void init_timer() {		// NB! DOES NOT START TIMER
	TCCR1A = (1 << COM1A1); // CTC TOP = OCR1A
	TCCR1B = (1 << WGM12);
	TIMSK1 = 1 << OCIE1A;
}
void start_timer() {
	TCCR1B |= 1 << CS10;
}
void stop_timer() {
	TCCR1B &= ~(1<<CS10);
}

int main(void)
{
	init_uart();
	init_timer();
	OCR1A = getCounterLimit(activeNote);
	//start_timer();
	sei();
    DDRA = 0xFF;
	PORTA = 0xFF;
	DDRB = 1 << SPEAKER;
    while (1) {
		PORTA = key;
		OCR1A = getCounterLimit(activeNote);
	}
}

