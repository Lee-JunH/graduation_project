/*
 * serial.c
 *
 * Created: 2023-12-05 오전 11:30:09
 *  Author: harry
 */ 

#include <avr/io.h>
#include "serial.h"

//USART 초기화 함수 정의
void InitSerial(unsigned long baudrate)
{
	unsigned int ubrr = (unsigned int)(F_CPU/(16*baudrate)-1); //UBRR획득
	UBRR0H=(unsigned char)(ubrr >> 8); //상위 비트 값 입력
	UBRR0L=(unsigned char)(ubrr);
	
	UCSR0A=0b00100000; //(생략가능) 송신준비 완료상태
	UCSR0B=0b00011000; //인터럽트 사용안함, USART송수신부 사용
	UCSR0C=0b00000110; //(생략가능)비동기모드, 정지비트 1, 전송 문자길이 8비트
}

//문자 한개를 Serial 로 송신하는 함수 정의
void SendChar(char data)
{
	while(!(UCSR0A & 0b00100000)); //송신준비 완료 플래그가 세트되면 탈출
	UDR0=data;
}

//문자 한개를 Serial 로 수신하는 함수 정의
char ReceiveChar(void)
{
	while(!(UCSR0A & 0b10000000)); //수신완료 플래그가 세트되면 탈출
	return UDR0; //수신 버퍼의 값을 반환
}

//문자열을 Serial 로 송신하는 함수 정의
void SendString(char* str)
{
	while(*str != '\0')
	{
		SendChar(*str);
		str++;
	}
	SendChar('\r');
	SendChar('\n');
}

//문자열을 Serial 로 수신하는 함수 정의
int ReceiveString(char* str)
{
	int count=0;
	char data; //문자 한개 수신받을 변수
	
	while(1)
	{
		data=ReceiveChar(); //문자 한개 수신
		count++;
		
		if('\r'==data) //데이터의 끝도달시
		{
			*str='\0';
			break; //수신한 문자가 CR이면 NULL저장 후 수신 종료
		}
		else
		{
			*str=data; //수신한 문자 str에 저장
			str++; //다음 문자 저장할 배열 주소 증가
		}
	}
	return count; //NULL 포함한 문자 개수 반환
}