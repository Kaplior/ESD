
#include "config.h"

#ifdef ADC_POLLING
/* 1. ADC Polling  */
/*ADC Registers:
 ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0
 ADMUX: REFS1 REFS0 ADLAR _ MUX3 MUX2 MUX1 MUX0
*/

void main_adc_polling (void)
{
	init_devices();
	ADCSRA = 0x00;	// disable adc
	ADMUX  = (1<<REFS1)|(1<<REFS0); 	// select adc input 0 and Internal 2.56V Voltage Reference with external capacitor at AREF pin
	ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);	// Conversion time:  Fosc/8
	unsigned int adc = 0;
	while (1){
		adc = 0;	
		
		ADCSRA |= (1<<ADSC);                            // Wait for the AD conversion to complete
		while(!(ADCSRA & (1<<ADIF)));
		ADCSRA |= (1<<ADIF);					//manual reset in polling (auto reset in interrupt)
		
		adc = (ADCL + (ADCH * 256))/10;			
		_delay_ms(100);
		lcd_clear();							// clear LCD
		ScreenBuffer_clear();					// clear screen buffer
		lcd_string(0,0, " 12345 Hong Jeong");
		lcd_string(2,0,"ADC0 Potentiometer");	// output ADC0 Potentiometer
		GLCD_Rectangle(50,0,60,adc);		// output line gage
		lcd_xy(4,0); GLCD_4DigitDecimal(adc);	// output ADC0
	}
}
#endif

#ifdef ADC_INTERRUPT
/* 2. ADC Interrupt */
/*ADC Registers:
 ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0
 ADMUX: REFS1 REFS0 ADLAR _ MUX3 MUX2 MUX1 MUX0
*/

void main_adc_interrupt (void)
{
	init_devices();
	ADCSRA = 0x00;	// disable adc
	ADMUX  = (1<<REFS0); 	// select adc input 0 and Aref
	ADCSRA = (1<<ADSC)|(1<<ADIE)|(1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);	// Conversion time:  Fosc/8
	sei();
	while (1);		
}

SIGNAL (ADC_vect){
	static unsigned int adc = 0;
	adc = (ADCL + (ADCH * 256))/10;
	_delay_ms(100);
	lcd_clear();							// clear LCD
	ScreenBuffer_clear();					// clear screen buffer
	lcd_string(0,0, " 12345 Hong Jeong");
	lcd_string(2,0,"ADC0 Potentiometer");	// output ADC0 Potentiometer
	GLCD_Rectangle(50,0,60,adc);		// output line gage
	lcd_xy(4,0); GLCD_4DigitDecimal(adc);	// output ADC0
	ADCSRA |= (1<<ADSC);					//ADSC reset when conversion completed
}
#endif

#ifdef ADC_POLLING_UART_POLLING
/* 3. ADC Polling UART Polling*/
/* UART and ADC Registers ('-' means default 0)
UART Registers:
UCSRnA: RXCn TXCn UDREn - - - - -
UCSRnB: RXCIEn TXCIEn UDRIEn RXENn TXENn - - -
UCSRnC: - - - - - UCSZn1 UCSZn0 -

ADC Registers:
ADMUX: REFS1 REFS0 - - MUX3 MUX2 MUX1 MUX0
ADCSA: ADEN ADSC - ADIF ADIE ADPS2 ADPS1 ADPS0
*/

#define F_CPU 16000000UL
#define BAUD 9600
void usart_send (unsigned char ch);
void main_adc_polling_uart_polling (void)
{
// UART1: 9600 8N1
UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
UBRR1L = F_CPU/16/BAUD-1;
UCSR1B = (1<<TXEN1);				//Transmission enable
UCSR1C = (1<< UCSZ11)|(1<<UCSZ10);	// 8-bit

// ADC Initialization
ADMUX = (1<<REFS1)|(1<<REFS0);  // 0xC0, 2.56V Vref, right-justified, channel 0
ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // 0x87

while (1)
{
ADCSRA|=(1<<ADSC);				// start conversion
while((ADCSRA&(1<<ADIF))==0);	// polling conversion to finish
ADCSRA |= (1<<ADIF);			// clear ADIF not auto cleared in polling, cleared auto in interrupt

int a = ADCL;
a = a | (ADCH<<8);

if(a < 0)
{
usart_send('-');
a *= -1;
}

usart_send((a/1000)+'0');
a = a % 1000;
usart_send((a/100)+'0');
a = a % 100;
usart_send((a/10)+'0');
a = a % 10;
usart_send((a)+'0');
usart_send('\r');

_delay_ms(100);
}

}

void usart_send (unsigned char ch)
{
while (!(UCSR1A & (1<<UDRE1))); 	// transmission complete?
UDR1 = ch;							// transmit ch
}
#endif

#ifdef ADC_INTERRUPT_UART_POLLING
/* 4. ADC Interrupt UART Polling */
/* UART and ADC Registers ('-' means default 0)
UART Registers:
UCSRnA: RXCn TXCn UDREn - - - - -
UCSRnB: RXCIEn TXCIEn UDRIEn RXENn TXENn - - -
UCSRnC: - - - - - UCSZn1 UCSZn0 -

ADC Registers:
ADMUX: REFS1 REFS0 - - MUX3 MUX2 MUX1 MUX0
ADCSA: ADEN ADSC - ADIF ADIE ADPS2 ADPS1 ADPS0
*/

#define F_CPU 16000000UL
#define BAUD 9600
void main_adc_interrupt_uart_polling (void)
{
	cli();
	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;
	UCSR1B = (1<<TXEN1);				// Transmission enable
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);	// 8-bit

	// ADC Initialization
	ADMUX = (1<<REFS1)|(1<<REFS0);  // 0xC0, 2.56V Vref, right-justified, channel 0
	ADCSRA = (1<<ADSC)|(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // 0x8F

	sei();

	while (1);
}

void usart_send (unsigned char ch)
{
	while (! (UCSR1A & (1<<UDRE1))); 	// polling transmission complete?
	UDR1 = ch;							// transmit ch
}

SIGNAL(ADC_vect)					// ADC interrupt
{
	int a = ADCL;					// ADC register
	a = a | (ADCH<<8);

	if(a < 0)
	{
		usart_send('-');
		a *= -1;
	}

	usart_send((a/1000)+'0');
	a = a % 1000;
	usart_send((a/100)+'0');
	a = a % 100;
	usart_send((a/10)+'0');
	a = a % 10;
	usart_send((a)+'0');
	usart_send('\r');

	ADCSRA |=(1<<ADSC);				// start conversion
}
#endif

#ifdef ADC_INTERRUPT_UART_INTERRUPT
/* Lab: ADC Interrupt UART Interrupt */

unsigned char ch ='x';
int a;
void main_adc_interrupt_uart_interrupt (void)
{
	init_devices();
	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;
	UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1);				// Transmission enable
	UCSR1C = (1<< UCSZ11)|(1<<UCSZ10);	// 8-bit

	// ADC Initialization
	ADMUX = (1<<REFS1)|(1<<REFS0);  // 0xC0, 2.56V Vref, right-justified, channel 0
	ADCSRA = (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // 0x8F

	sei();
	//ADCSRA |=(1<<ADSC);				// start conversion

	DDRB = 0xFF;
	PORTB = 0xFF;

	while (1);
}

void usart_send (unsigned char ch)
{
	while (!(UCSR1A & (1<<UDRE1))); 	// polling transmission complete?
	UDR1 = ch;							// transmit ch
	PORTB ^= (1<<7);
}

SIGNAL(ADC_vect)					// ADC interrupt
{
	a = ADCL;					// ADC register
	a = a | (ADCH<<8);
	
	lcd_clear();	
	lcd_string(0,0, " 12345 Hong Jeong");
	lcd_string(2,0,"ADC0 Potentiometer");	// output ADC0 Potentiometer
		
	ScreenBuffer_clear();					// clear screen buffer
	GLCD_Rectangle(50,0,60,a/10);		// output line gage
	lcd_xy(4,0); GLCD_4DigitDecimal(a);	// output ADC0
	
	if(a < 0)
	{
	ch = '-';
	a *= -1;
	}

	ch = (a/1000)+'0'; 	usart_send(ch);
	a = a % 1000;
	ch = (a/100)+'0'; 	usart_send(ch);
	a = a % 100;
	ch = (a/10)+'0'; 	usart_send(ch);
	a = a % 10;
	ch = (a)+'0'; 		usart_send(ch);
	ch = '\r'; 			usart_send(ch);
}

SIGNAL (USART1_RX_vect){
	unsigned char control = UDR1;
	if (control == 's'){
		PORTB ^= (1<<0);
		ADCSRA |=(1<<ADSC);				// start conversion
	}
}
#endif


