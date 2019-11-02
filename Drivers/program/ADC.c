/*
 * adcTest.c
 *
 * Created: 11.09.2019 16:13:05
 *  Author: oddiha
 */ 

#include <avr/io.h>
#include <avr/common.h>
#include <avr/builtins.h>
#include <avr/interrupt.h>
#include <avr/iom162.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>

#include "ADC.h"
#include "JOYSTICK.h"

//joystick_data_t joystickCalibration;
	
volatile uint8_t* extADC = (uint8_t*) ADC_ADDRESS;
volatile uint8_t ADCdata;



void adcInit( void )
{
	
	// PortE pin0 as input
	DDRE &= ~(1 << PE0);
	
	// PortB pin0 as input
	DDRB &= ~(1 << PB0);	// USB_RIGHT_TOUCHBUTTON
	
	// PortD pin 2 and 3 as input
	DDRD &= ~(1 << PD2);	// MCP2515 Interrupt
	DDRD &= ~(1 << PD3);	// JOYSTICK PUSHBUTTON - Make interrupt??
	
	// Set pull-up resistor PE0 & PB0
	PORTE |= (1 << PE0);	// LatchEnable Active Low 
	PORTB |= (1 << PB0);	// USB_RIGHT_TOUCHBUTTON -- Does it work?? The PUSH signal is HIGH - Maybe we need pull-down then??
	
	// pull-down
	PORTD &= ~(1 << PD2);	// MCP2515 interrupt
	PORTD &= ~(1 << PD3);	//	Joystick PUSHBUTTON
	
	//disable global interrupt
	//cli();
	
	// INT2 Interrupt on falling edge
	//EMCUCR &= ~(1 << ISC2);
	
	// INT0 Interrupt on rising edge
	//EMCUCR |= (1 << ISC01);
	//EMCUCR |= (1 << ISC00);	
	
	// INT1 Interrupt on rising edge
	//EMCUCR |= (1 << ISC11);
	//EMCUCR |= (1 << ISC10);
	
	// Enable interrupt on PE0
	//GICR |= (1 << INT2);
	
	//Enable interrupt on PD2
	//GICR |= (1<<INT0);
	
	//Enable interrupt on PD3
	//GICR |= (1<<INT1);
	
	//enable global interrupt
	//sei();
	
}



void select_ADC_channel(channel_t channel)
{	
	switch(channel){
		case CHANNEL1:
			*extADC = (uint8_t ) 0x04;
			break;
		case CHANNEL2:
			*extADC = (uint8_t ) 0x05;
			break;
		case CHANNEL3:
			*extADC = (uint8_t ) 0x06;
			break;
		case CHANNEL4:
			*extADC = (uint8_t ) 0x07;
			break;
		default:
			printf("Invalid channel");
	}
	
}

uint8_t getADCdata(channel_t channel)
{
	select_ADC_channel(channel);
	while (PINE & (1<<PE0)); // Wait for ADC data
	ADCdata = *extADC;
	return ADCdata;
}




/*
void sliderDriver()
{
	uint8_t leftSliderData, rightSliderData;
	leftSliderData = getADCdata(CHANNEL1);	//Left slider
	printf("\n\rLeft slider: %d \n\r", leftSliderData);
	
	
	rightSliderData = getADCdata(CHANNEL2);	// Right slider
	printf("Right slider: %d \n\r", rightSliderData);
}

joystick_direction_t get_joystick_direction(uint8_t joyst_coord_X, uint8_t joyst_coord_Y){
	
	int8_t signed_joyst_coord_X, signed_joyst_coord_Y;
	uint8_t abs_signed_joyst_coord_X, abs_signed_joyst_coord_Y;
	
	signed_joyst_coord_X = joyst_coord_X - 128; //joystickCalibration.x_offset;
	signed_joyst_coord_Y = joyst_coord_Y - 128; //joystickCalibration.y_offset;
	abs_signed_joyst_coord_X = abs(signed_joyst_coord_X);
	abs_signed_joyst_coord_Y = abs(signed_joyst_coord_Y);
	
	
	
	if (abs_signed_joyst_coord_X <= JOYSTICK_THRESOLD && abs_signed_joyst_coord_Y <= JOYSTICK_THRESOLD){
		return CENTER;
	}
	
	if (signed_joyst_coord_X > abs_signed_joyst_coord_Y )
	return RIGHT;
	else if (signed_joyst_coord_X < - abs_signed_joyst_coord_Y)
	return LEFT;
	else if (signed_joyst_coord_Y > abs_signed_joyst_coord_X)
	return UP;
	else
	return DOWN;
	
}

// When joystick is centered the measured center value will be the offset
void joystickCalibrate( void )
{
	
	joystickCalibration.x_offset = getADCdata(CHANNEL4); //X axis on channel 4
	joystickCalibration.y_offset = getADCdata(CHANNEL3); //Y axis on channel 3
	
}

void joystickDriver()
{
	uint8_t x_axis;
	uint8_t y_axis;
	x_axis = getADCdata(CHANNEL4); //X axis on channel 4
	y_axis = getADCdata(CHANNEL3); //Y axis on channel 3
	
	
	joystick_direction_t joyst_direction; //The value should be: CENTER/LEFT/RIGHT/DOWN/UP
	joyst_direction = get_joystick_direction(x_axis, y_axis);
	
	
	switch(joyst_direction){
		case CENTER:
		printf("CENTER\n\r");
		break;
		case LEFT:
		printf("LEFT\n\r");
		break;
		case RIGHT:
		printf("RIGHT\n\r");
		break;
		case DOWN:
		printf("DOWN\n\r");
		break;
		case UP:
		printf("UP\n\r");
		break;
		default:
		printf("The function get_joystick_direction is not returning one of these: CENTER/LEFT/RIGHT/DOWN/UP\n\r");
				
	}
	printf("Joystick X = %d \n\r", x_axis);
	printf("Joystick Y = %d \n\r", y_axis);
}
*/


/*
int main(void) 
{
		USART_Init ( MYUBRR );
		fdevopen(&USART_Transmit, &USART_Receive);
		ExernalMemoryInit();
		adcInit();
		
		joystickCalibrate();
		
		printf("Start Test");
		
    while(1)
    {
		if(PIND&0x04)
		{
			printf("Left button WORKS!\n\r");
		}
		if(PIND&0x08)
		{
			printf("Right button WORKS!\n\r");
		}
		if(!PINB&0x01)
		{
			printf("Center button WORKS!\n\r");
		}	
		
		joystickDriver();	
		sliderDriver();

       _delay_ms(1000);
    }
}*/