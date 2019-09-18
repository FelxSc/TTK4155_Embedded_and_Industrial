#define ADC_ADDRESS 0x1400

typedef enum {CHANNEL1, CHANNEL2, CHANNEL3, CHANNEL4} channel_t;		// data type to select adc channels
typedef enum {CENTER, LEFT, RIGHT, DOWN, UP} joystick_direction_t;		// Joystick direction



typedef struct joystick_data
{
	uint8_t x_offset;
	uint8_t y_offset;
}joystick_data_t;