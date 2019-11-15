/*
 * USART.c
 *
 * Created: 8/28/2019 7:32:57 PM
 * Author : oddingehalsos
 */ 
//#define F_CPU 4915200 // 4.9152 Mhz
#define FOSC 4915200// Clock Speed
#define BAUD 9600
#define MYUBRR FOSC/16/BAUD-1

#include <avr/io.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/iom162.h>
#include <util/delay.h>
#include <stdlib.h>

#define SRAM_FIRST_ADDRESS 0x1800


void SRAM_test(void)
{
	printf("ENTERING SRAM TEST\n\r");
	volatile char *ext_ram = (char *) 0x1800; // Start address for the SRAM
	printf("ext ram\n\r");
	uint16_t ext_ram_size= 0x800;
	uint16_t write_errors= 0;
	uint16_t retrieval_errors= 0;
	
	printf("Starting SRAM test...\n\r");
	
	// rand() stores some internal state, so calling this function in a loop will
	// yield different seeds each time (unless srand() is called before this function)
	uint16_t seed = rand();// Write phase: Immediately check that the correct value was stored
	srand(seed);
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		ext_ram[i] = some_value;
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Write phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			write_errors++;
		}
	}
	
	// Retrieval phase: Check that no values were changed during or after the write phase
	srand(seed);	// reset the PRNG to the state it had before the write phase
	for (uint16_t i = 0; i < ext_ram_size; i++) {
		uint8_t some_value = rand();
		uint8_t retreived_value = ext_ram[i];
		if (retreived_value != some_value) {
			printf("Retrieval phase error: ext_ram[%4d] = %02X (should be %02X)\n\r", i, retreived_value, some_value);
			retrieval_errors++;
		}
	}
	printf("SRAM test completed with \n\r%4d errors in write phase and \n\r%4d errors in retrieval phase\n\n\r", write_errors, retrieval_errors);
}

void DecoderTest( void )
{
	
	unsigned char data = 'b';
	printf("SRAM test\n\r");
	PORTC = 0x0a;
	scanf("%s",&data);
	printf("ADC test\n\r");
	PORTC = 0x04;
	scanf("%s",&data);
	printf("OLED cmd test\n\r");
	PORTC = 0x00;
	scanf("%s",&data);
	printf("OLED data test\n\r");
	PORTC = 0x02;
	scanf("%s",&data);
	
}

void LatchTest( void )
{
	unsigned char data = 0x0;
	printf("0x01\n\r");
	PORTA = 0x1;
	scanf("%s",&data);
	printf("0x02\n\r");
	PORTA = 0x2;
	scanf("%s",&data);
	printf("0x04\n\r");
	PORTA = 0x4;
	scanf("%s",&data);
	printf("0x08\n\r");
	PORTA = 0x8;
	scanf("%s",&data);
	
}



void SRAM_write(uint16_t address, char data){
	
	volatile char *ext_ram = (char *) SRAM_FIRST_ADDRESS;	//Create a pointer to the array of all addresses we will write to. SRAM starting at 0x1800. ext_ram[0x7FF] is maximum because 0x1800 + 0x7FF = 0x1FFF!
	ext_ram[address] = data;
}

uint8_t SRAM_read(uint16_t address){
	
	//if (address > 0x7FF){
	//	printf("SRAM error, trying to read an address that is too big\n");
	//	return EXIT_FAILURE;
	//}
	uint8_t value = 0;
	volatile char *ext_ram = (char *) SRAM_FIRST_ADDRESS;
	value = ext_ram[address];
	return value;
}
/*
int main(void)
{
	USART_Init ( MYUBRR );
	fdevopen(&USART_Transmit, &USART_Receive);
	ExernalMemoryInit();
	
	//SRAM_test();
	
	char data;
	char send = 'a';
	

	
    while (1) 
    {
		SRAM_write(0x600, send);	
		data = SRAM_read(0x600);
		printf("%c",data);
		_delay_ms(10);
		//send++;
		
    }
}*/