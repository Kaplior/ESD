/*
Hong Jeong
https://ww1.microchip.com/downloads/en/DeviceDoc/doc2467.pdf
*/

#include "config.h"

int main (void)
{
	

	#ifdef ACCELEROMETER
	main_accelerometer();
	#elif defined(ADC_INTERRUPT)
	main_adc_interrupt();
	#elif defined(ADC_INTERRUPT_UART_INTERRUPT)
	main_adc_interrupt_uart_interrupt();
	#elif defined(ADC_INTERRUPT_UART_POLLING)
	main_adc_interrupt_uart_polling();
	#elif defined(ADC_POLLING)
	main_adc_polling();
	#elif defined(ADC_POLLING_UART_POLLING)
	main_adc_polling_uart_polling();
	#elif defined(CDS)
	main_cds();
	#elif defined(GAME_OBSTACLE)
	main_game_obstacle();
	#elif defined(GRAPHICS_BASICS)
	main_graphics_basics();
	#elif defined(GRAPHICS_MOVEMENT)
	main_graphics_movement();
	#elif defined(GRAPHICS_RANDOM)
	main_graphics_random();
	#elif defined(INTERRUPT_EXTERNAL)
	main_interrupt_external();
	#elif defined(INTERRUPT_LAB)
	main_interrupt_lab();
	#elif defined(INTERRUPT_TIMER)
	main_interrupt_timer();
	#elif defined(INTERRUPT_TIMER_CTC)
	main_interrupt_timer_ctc();
	#elif defined(JOYSTICK)
	main_joystick();
	#elif defined(MEMORY_EEPROM)
	main_memory_eeprom();
	#elif defined(MEMORY_PROGRAM)
	main_memory_program();
	#elif defined(PORT_BLINKING)
	main_port_blinking();
	#elif defined(PORT_ROTATION)
	main_port_rotation();
	#elif defined(SERIAL_BUFFER)
	main_serial_buffer();
	#elif defined(SERIAL_INTERRUPT)
	main_serial_interrupt();
	#elif defined(SERIAL_INTERRUPT_LAB)
	main_serial_interrupt_lab();
	#elif defined(SERIAL_POLLING)
	main_serial_polling();
	#elif defined(SERIAL_POLLING_SIMULIDE)
	main_serial_polling_simulide();
	#elif defined(SERIAL_PUZZLE)
	main_serial_puzzle();
	#elif defined(SERIAL_WORD)
	main_serial_word();
	#elif defined(TIMER_COUNTER)
	main_timer_counter();
	#elif defined(TIMER_CTC)
	main_timer_ctc();
	#elif defined(TIMER_FASTPWM)
	main_timer_fastpwm();
	#elif defined(TIMER_NORMAL)
	main_timer_normal();
	// Add more conditions here if needed
	#else
	// Code to execute when none of the above conditions are met
	#endif
	// The rest of your main function
	return 0;
}
