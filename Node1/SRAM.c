/*
 * SRAM.c
 *
 * Created: 8/28/2019 7:32:57 PM
 *  Author: Odd Inge Halsos & Felix Schöpe
 */ 


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



void SRAM_write(uint16_t address, char data){
	
	volatile char *ext_ram = (char *) SRAM_FIRST_ADDRESS;
	ext_ram[address] = data;
}

uint8_t SRAM_read(uint16_t address){
	

	uint8_t value = 0;
	volatile char *ext_ram = (char *) SRAM_FIRST_ADDRESS;
	value = ext_ram[address];
	return value;
}
