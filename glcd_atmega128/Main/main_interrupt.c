#include "config.h"

#ifdef INTERRUPT_EXTERNAL
/* External Interrupt 
   1. External INT0~3: PD0~PD3, INT4~7: PE4~PE7
   2. EICRA: ISC31 ISC30 ISC21 ISC20 ISC11 ISC10 ISC01 ISC00
	  EICRB: ISC71 ISC70 ISC61 ISC60 ISC51 ISC50 ISC41 ISC40
   ISCn1 ISCn0 Description
	0 0 The low level of INTn generates an interrupt request.
	0 1 Reserved
	1 0 The falling edge of INTn generates asynchronously an interrupt request.
	1 1 The rising edge of INTn generates asynchronously an interrupt request.
   3. EIMSK: INT7 INT6 INT5 INT4 INT3 INT2 INT1 INT0
   4. SREG: SEI 
*/

void main_interrupt_external (void)
{
	//apps
	DDRB = 0xFF;
	PORTB = 0xAA;
	
	//interrupt
	EICRA = 1<<ISC01;		//INT0 falling
	EICRB = 0x00;
	EIMSK = 1<<INT0;	//INT0 enabled
	sei();				//Interrupt enabled
	//apps
	while (1);	

}

SIGNAL (INT0_vect){
	PORTB = ~PORTB;
}
#endif

/* Timer Interrupt 
   1. TCNT0
   2. TCCR0: FOC0 WGM00 COM01 CON00 WGM01 CS02 CS01 CS00
   3. TIFR:  OCF2 TOV2  ICF1  OCF1B TOV1  OCF0 TOV0
      TIMSK: OCIE2 TOIE2 TICIE1 OCIE1A OCIE1B TOIE1 OCIE0 TOIE0
   4. SREG: SEI 
*/
#ifdef INTERRUPT_TIMER

void main_interrupt_timer (void)
{
	//apps
	DDRB = 0xFF;
	PORTB = 0xAA;
	//timer interrupt
	TCNT0 = 0x00;					// 0~255
	TCCR0 = (1<<CS02) | (1<< CS01) | (1<< CS00); // clock/1024
	TIMSK |= 1<<TOIE0;
	sei();
	
	while (1);	

}

SIGNAL (TIMER0_OVF_vect){
	static int i = 0;
	if (!(i%10)){
		PORTB = ~PORTB;
	}
	i++;
	TCNT0 = 0x00;
}
#endif

/* Timer Interrupt: CTC mode
   1. TCNT0, OCR0
   2. TCCR0: FOC0 WGM00 COM01 CON00 WGM01 CS02 CS01 CS00
   3. TIFR:  OCF2 TOV2  ICF1  OCF1B TOV1  OCF0 TOV0
      TIMSK: OCIE2 TOIE2 TICIE1 OCIE1A OCIE1B TOIE1 OCIE0 TOIE0
   4. SREG: SEI 
*/
#ifdef INTERRUPT_TIMER_CTC

void main_interrupt_timer_ctc (void)
{
	//apps
	DDRB = 0xFF;
	PORTB = 0xAA;
	//timer interrupt
	TCNT0 = 0x00;
	OCR0 = 0xFF;					// 0~255
	TCCR0 = (1<<WGM01) | (1<<CS02) | (1<< CS01) | (1<< CS00); // CTC and Clock/1024
	TIMSK = 1<<OCIE0;
	sei();
	while (1) {}	
}

SIGNAL (TIMER0_COMP_vect){
	static int i = 0;
	if (!(i%100)) PORTB = ~PORTB;
	i++;
}
#endif

/* Lab: Button PIN0 toggles LED between 0xAA and 0x55, displaying it on LCD
   1. External INT0~3: PD0~PD3, INT4~7: PE4~PE7
   2. EICRA: ISC31 ISC30 ISC21 ISC20 ISC11 ISC10 ISC01 ISC00
	  EICRB: ISC71 ISC70 ISC61 ISC60 ISC51 ISC50 ISC41 ISC40
   ISCn1 ISCn0 Description
	0 0 The low level of INTn generates an interrupt request.
	0 1 Reserved
	1 0 The falling edge of INTn generates asynchronously an interrupt request.
	1 1 The rising edge of INTn generates asynchronously an interrupt request.
   3. EIMSK: INT7 INT6 INT5 INT4 INT3 INT2 INT1 INT0
   4. SREG: SEI 
*/

#ifdef INTERRUPT_LAB
	unsigned char num1;
	unsigned char num2;
	unsigned char num3;
	char result[30];
void main_interrupt_lab (void)
{
	init_devices();								// initialize LCD
	lcd_clear();
	lcd_string(0,0, "12345 Hong Jeong");
	
	//apps
	DDRB = 0xFF;
	PORTB = 0xAA;

	
	// INT0 interrupt
	EICRA = 1<<ISC01;		//INT0 falling
	EICRB = 0x00;
	EIMSK = 1<<INT0;	//INT0 enabled
	sei();				//Interrupt enabled
	
	
	//apps
	int i = 0;
	int s = 0;
	int m = 0;
	int h = 0;
	char clock[20];
	while (1) {

		_delay_ms(1000);
		i++;
		h = i/3600;
		m = (i%3600)/60;
		s = (i%3600)%60;
		sprintf(clock,"%d:%d:%d",h,m,s);
		lcd_string(3,0,clock);		
	}	

}

SIGNAL (INT0_vect){
	//multiplier
	num1 = rand();
	num2 = rand();

	sprintf(result,"%d * %d = %d",num1,num2,num1 * num2);
	lcd_string(6,0,result);
	PORTB = ~PORTB;
}
#endif
