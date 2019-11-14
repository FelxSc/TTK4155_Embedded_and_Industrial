/*
 * CAN.h
 *
 * Created: 05.10.2019 18:18:32
 *  Author: oddiha
 */ 


#ifndef CAN_H_
#define CAN_H_


typedef struct {
	uint16_t ID;
	uint8_t length;
	uint8_t msg[8];
}CAN_message_t;


CAN_message_t game_data;


void sendCANmessage(CAN_message_t* data);
void receiveCANmesssage( CAN_message_t* data, uint8_t reg );



#endif /* CAN_H_ */