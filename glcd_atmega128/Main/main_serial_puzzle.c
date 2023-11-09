
#include "config.h"

/******** 20 questions quiz ******************/
#ifdef SERIAL_PUZZLE

#define MYUBRR F_CPU/16/BAUD-1 // Calculate UBRR value

// UART buffer sizes
#define TX_BUFFER_SIZE 64

// UART buffers
volatile char tx_buffer[TX_BUFFER_SIZE];
volatile uint8_t tx_write_pos = 0;
volatile uint8_t tx_read_pos = 0;

// Function prototypes
void init_UART(uint16_t ubrr_value);
void uart_transmit(char data);
void send_string(const char* str);
uint8_t is_tx_buffer_empty();
void write_tx_buffer(char c);

// Guessing game data structure
typedef struct {
	int lower_bound;
	int upper_bound;
} GuessRange;

// Initialize the guessing range
GuessRange currentRange = {1, 100};

// UART initialization function
void init_UART(uint16_t ubrr_value) {
	// Set baud rate
	UBRR1H = (unsigned char)(ubrr_value>>8);
	UBRR1L = (unsigned char)ubrr_value;
	// Enable receiver, transmitter and receiver interrupt
	UCSR1B = (1<<RXEN1)|(1<<TXEN1)|(1<<RXCIE1);
	// Set frame format: 8 data, 1 stop bit
	UCSR1C = (3<<UCSZ10);
}

// Function to check if the transmission buffer is empty
uint8_t is_tx_buffer_empty() {
	return tx_read_pos == tx_write_pos;
}

// Function to write data to the transmission buffer
void write_tx_buffer(char c) {
	if ((tx_write_pos + 1) % TX_BUFFER_SIZE != tx_read_pos) {
		tx_buffer[tx_write_pos] = c;
		tx_write_pos = (tx_write_pos + 1) % TX_BUFFER_SIZE;
	}
	// Enable data register empty interrupt
	UCSR1B |= (1<<UDRIE1);
}

// ISR for UART data register empty
ISR(USART1_UDRE_vect) {
	if (!is_tx_buffer_empty()) {
		UDR1 = tx_buffer[tx_read_pos];
		tx_read_pos = (tx_read_pos + 1) % TX_BUFFER_SIZE;
		} else {
		// Disable the interrupt if nothing to send
		UCSR1B &= ~(1<<UDRIE1);
	}
}

// Function to send a string through UART
void send_string(const char* str) {
	while (*str) {
		write_tx_buffer(*str++);
	}
}

// Function to ask the next question
void ask_question(GuessRange range) {
	char question[64];
	int midpoint = (range.lower_bound + range.upper_bound) / 2;
	snprintf(question, sizeof(question), "Is the number greater than %d? (y/n)\r\n", midpoint);
	send_string(question);
}

// Function to process the user's answer
void process_answer(char answer) {
	int midpoint = (currentRange.lower_bound + currentRange.upper_bound) / 2;
	if (answer == 'y') {
		currentRange.lower_bound = midpoint + 1;
		} else {
		currentRange.upper_bound = midpoint;
	}

	if (currentRange.lower_bound == currentRange.upper_bound) {
		char final_guess[64];
		snprintf(final_guess, sizeof(final_guess), "The number is %d!\r\n", currentRange.lower_bound);
		send_string(final_guess);
		// Reset the range for a new game
		currentRange.lower_bound = 1;
		currentRange.upper_bound = 100;
		send_string("Think of another number between 1 and 100.\r\n");
		} else {
		ask_question(currentRange);
	}
}

// ISR for UART receive complete
ISR(USART1_RX_vect) {
	static char buffer[10];
	static uint8_t i = 0;

	char data = UDR1;

	// Check for end of line (\r\n)
	if (data == '\r' || data == '\n') {
		if (i > 0) { // Check if buffer is not empty
			buffer[i] = '\0'; // Null-terminate the string
			process_answer(buffer[0]); // Process the first character
			i = 0; // Reset the index
		}
		} else if (i < sizeof(buffer) - 1) { // Check buffer overflow
		buffer[i++] = data; // Store the data
	}
}

// Main function
void main_serial_puzzle(void) {
	// Initialize UART
	init_UART(MYUBRR);
	sei(); // Enable global interrupts

	// Welcome message
	send_string("Guess the Number Game!\r\nThink of a number between 1 and 100.\r\n");

	// Start by asking the first question
	ask_question(currentRange);

	// Main loop
	while (1) {
		// Main loop does nothing, just waits for interrupts
	}

}
#endif
