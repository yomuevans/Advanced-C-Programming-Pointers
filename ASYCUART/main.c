#include <atmel_start.h>
#include <string.h>

//Serial Receiving & Complete flags
volatile uint8_t serial_recieving = 0;
volatile uint8_t serial_complete = 0;

// Bytes Received Counters
volatile uint8_t serial_recieved_bytes_counter = 0;
volatile uint8_t total_bytes = 0;

// Size Of Receive Buffer
#define SERIAL_BUFFER_SIZE 200

//Receive and Transmit Buffers
 uint8_t rx_buffer[SERIAL_BUFFER_SIZE] = { 0x00 };
 uint8_t tx_buffer[SERIAL_BUFFER_SIZE + 14] = "Your message";


/*
Virtual COM port receive callback Function, used for receiving data
*/
static void serial_rx_cb(const struct usart_async_descriptor *const io_descr)
{
	//counters
	uint8_t ch, count;
	// Read a character
	count = io_read(&SERIAL.io, &ch,1);
	
	// Check if we're receiving
	if(serial_recieving == 0)
	{
		//Check for new line of Carriage return
		if(ch != '\r' && ch != '\n')
		{
			//Set the receiving Flag
			serial_recieving = 1;
			
			//Reset Byte Counter
			serial_recieved_bytes_counter = 0;
			
			//Start Filling the buffer
			rx_buffer[serial_recieved_bytes_counter] = ch;
			
			//Increment our byte counter
			serial_recieved_bytes_counter += count;
				
		}
		
	}
	else
	{
		//Continue filling in the buffer
		rx_buffer[serial_recieved_bytes_counter] = ch;
		
		//Increment the byte counter
		
		serial_recieved_bytes_counter += count;
		
		//Check for new carriage return
		if (ch == '\r' || ch == '\n')
		{
			//Set the completion flag
			serial_complete =1;
			
			//Total Bytes
			total_bytes = serial_recieved_bytes_counter -2;
			
		}
		//Check the buffer Overflow
		if (serial_recieved_bytes_counter >= SERIAL_BUFFER_SIZE)
		{
			//Reset the buffer counter
			serial_recieved_bytes_counter = 0;
		}
	}
}
//Virtual Com Port Transmit callback function.
static void serial_tx_cb(const struct usart_async_descriptor *const io_descr)
{
	// Do nothing
}


int main(void)
{
	/* Initializes MCU, drivers and middleware */
	atmel_start_init();
	
	// Initialize ANSYNC Driver
	usart_async_register_callback(&SERIAL, USART_ASYNC_TXC_CB, serial_tx_cb);
	usart_async_register_callback(&SERIAL, USART_ASYNC_RXC_CB, serial_rx_cb );
	usart_async_enable(&SERIAL);

	/* Replace with your application code */
	while (1) 
	{
		// Check if we're complete
		if (serial_recieving == 1)
		{
			// Check if we're complete
			if (serial_complete == 1)
			{
				//Reset flags
				serial_recieving = 0;
				serial_complete =0;
				
				//Copy Message to TX  Buffer
				memcpy(&tx_buffer[14], &rx_buffer[0], SERIAL_BUFFER_SIZE);
				
				//print a message
				io_write(&SERIAL.io, tx_buffer, total_bytes + 16);
				
				//Clear memory
				memset(&rx_buffer, 0x00, SERIAL_BUFFER_SIZE);
			}
		}
	}
}
