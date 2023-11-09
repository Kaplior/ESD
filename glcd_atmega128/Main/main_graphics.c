
#include "config.h"

/* Graphics: basics */
#ifdef GRAPHICS_BASICS
 
/* Graphics: Characters  */
char Dis_Scr_IO_ON1[]={"O"};
char Dis_Scr[]={"#$%&'()*+,-./0123456"};
void main_graphics_basics(void)
{
	init_devices();								// initialize LCD
	S_Start();									// sound
	lcd_clear();
	lcd_string(0,0,"====================");       
	lcd_string(1,0,"   ATmega128 GLCD   ");		
	lcd_string(2,0,Dis_Scr);
	GLCD_Line(30,10,40,20);				GLCD_Rectangle(30,31,40,41);    	
	lcd_string(4,5,Dis_Scr_IO_ON1);  	GLCD_Circle(35,55,5);
	lcd_xy(4,12); GLCD_4DigitDecimal(1235);
	lcd_string(6,0, "SOC3050 Hong Jeong");
	
}
#endif




/* Graphics: Random circles 	*/
#ifdef GRAPHICS_RANDOM

void main_graphics_random (void)
{
	// random numbers and seed.
	uint16_t u_rand_x = 0;
	uint16_t u_rand_y = 0;
	uint16_t u_rand_r = 0;
	time_t t;
	init_devices();
	lcd_clear();
	srand((unsigned) time(&t));
	while(1){
		u_rand_x = rand()%64;
		u_rand_y = rand()%128;
		u_rand_r = rand()%10;
		GLCD_Circle(u_rand_x, u_rand_y, u_rand_r);
		_delay_ms(100);
	}
}
#endif
	
/* 	Graphics: Random movement 	*/
#ifdef GRAPHICS_MOVEMENT

void main_graphics_movement (void)
{
	//random numbers and seed.
	uint16_t u_rand_x = 0, old_x = 0;
	uint16_t u_rand_y = 0, old_y = 0;
	time_t t;
		
	init_devices();
	lcd_clear();
	srand((unsigned) time(&t));
	while(1){
		//S_Star();
		u_rand_x = rand()%64;
		u_rand_y = rand()%128;
		GLCD_Line(old_x, old_y, u_rand_x, u_rand_y);
		old_x = u_rand_x; old_y = u_rand_y;
		_delay_ms(100);
	}
}
#endif