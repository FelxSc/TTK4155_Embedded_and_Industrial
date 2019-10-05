/*
 * CAN.h
 *
 * Created: 05.10.2019 18:18:32
 *  Author: oddiha
 */ 


#ifndef CAN_H_
#define CAN_H_

void sendCANmessage(CAN_message_t* data);
void receiveCANmesssage( CAN_message_t* );



#endif /* CAN_H_ */