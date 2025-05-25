/*
 * serial.h
 *
 * Created: 2023-12-05 오전 11:29:02
 *  Author: harry
 */ 


#ifndef SERIAL_H_
#define SERIAL_H_

void InitSerial(unsigned long baudrate);
void SendChar(char data);
char ReceiveChar(void);
void SendString(char* str);
int ReceiveString(char* str);

#endif /* SERIAL_H_ */