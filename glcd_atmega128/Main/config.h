/* config.h
 *
 * Created: 11/28/2021 9:36:39 AM
 * Author: hjeong
 *
 * Configuration file for AVR microcontroller project.
 * Defines system clock frequency, baud rate for serial communication,
 * includes necessary headers, and declares function prototypes
 * for various modules and functionalities.
 */

#ifndef CONFIG_H_
#define CONFIG_H_

// System clock frequency
#define F_CPU 16000000UL // 16 MHz

// Baud rate for UART
#define BAUD 9600

// AVR system libraries
#include <avr/io.h>
#include <avr/interrupt.h> 
#include <util/delay.h>

// Standard libraries (ensure compatibility with AVR)
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

// Project-specific module headers
#include "_main.h"
#include "_buzzer.h"
#include "_adc.h"
#include "_eeprom.h"
#include "_init.h"
#include "_interrupt.h"
#include "_port.h"
#include "_timer2.h"
#include "_uart.h"
#include "_glcd.h"

// Alphabetically ordered feature toggle macros (uncomment to enable) and function prototypes
//#define ACCELEROMETER
void main_accelerometer(void);

//#define ADC_INTERRUPT
void main_adc_interrupt(void);

//#define ADC_INTERRUPT_UART_INTERRUPT
void main_adc_interrupt_uart_interrupt(void);

//#define ADC_INTERRUPT_UART_POLLING
void main_adc_interrupt_uart_polling(void);

//#define ADC_POLLING
void main_adc_polling(void);

//#define ADC_POLLING_UART_POLLING
void main_adc_polling_uart_polling(void);

//#define CDS
void main_cds(void);

#define GAME_OBSTACLE
void main_game_obstacle(void);

//#define GRAPHICS_BASICS
void main_graphics_basics(void);

//#define GRAPHICS_MOVEMENT
void main_graphics_movement(void);

//#define GRAPHICS_RANDOM
void main_graphics_random(void);

//#define INLINE
void main_inline(void); // Ensure this function exists or is used

//#define INTERRUPT_EXTERNAL
void main_interrupt_external(void);

//#define INTERRUPT_LAB
void main_interrupt_lab(void);

//#define INTERRUPT_TIMER
void main_interrupt_timer(void);

//#define INTERRUPT_TIMER_CTC
void main_interrupt_timer_ctc(void);

//#define JOYSTICK
void main_joystick(void);

//#define MEMORY_EEPROM
void main_memory_eeprom(void);

//#define MEMORY_PROGRAM
void main_memory_program(void);

//#define PORT_BLINKING
void main_port_blinking(void);

//#define PORT_ROTATION
void main_port_rotation(void);

//#define SERIAL_BUFFER
void main_serial_buffer(void);

//#define SERIAL_INTERRUPT
void main_serial_interrupt(void);

//#define SERIAL_INTERRUPT_LAB
void main_serial_interrupt_lab(void);

//#define SERIAL_POLLING
void main_serial_polling(void);

//#define SERIAL_POLLING_SIMULIDE
void main_serial_polling_simulide(void);

//#define SERIAL_PUZZLE
void main_serial_puzzle(void);

//#define SERIAL_WORD
void main_serial_word(void);

//#define TIMER_COUNTER
void main_timer_counter(void);

//#define TIMER_CTC
void main_timer_ctc(void);

//#define TIMER_FASTPWM
void main_timer_fastpwm(void);

//#define TIMER_NORMAL
void main_timer_normal(void);

#endif /* CONFIG_H_ */
