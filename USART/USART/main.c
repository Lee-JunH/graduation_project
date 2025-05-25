/*
 * USART.c
 *
 * Created: 2023-12-05 오전 11:07:26
 * Author : harry
 */ 

#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include "serial.h"

unsigned char SegNum[16]=
{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

unsigned char cnumber[4]={0,}; // FND 출력할 자리수 배열

unsigned int fndon=0; //fnd의 on/off 여부

ISR(INT4_vect) //sw1 클릭 시
{
	SendString("sw1 clicked");
}
ISR(INT5_vect) //sw2 클릭 시
{
	SendString("sw2 clicked");
}

ISR(TIMER0_COMP_vect) //출력비교 인터럽트
{
	//fnd 출력
	if(fndon)
	{
		static unsigned char digit=0;
		PORTG=0x01<<digit;
		PORTC=SegNum[cnumber[digit]];
		digit++;
		if(4==digit) digit=0;
	}
	else
	{
		PORTC=PORTC<<4;
	}
	//2초 감지
	static unsigned int count = 0;
	count++; //1ms마다 ++
	if(2000==count)
	{
		ADCSRA=ADCSRA | 0b01000000; // ADC 시작
		count=0;
	}
}

ISR(ADC_vect) //광 센서 감지
{
	unsigned int ADC_Data;
	ADC_Data=ADCW;
	
	char fndString[10]={0,}; //변환한 문자열 공간
	itoa(ADC_Data,fndString,10); //정수형을 문자열로 변환
	SendString(fndString);
}

int main(void)
{
	//포트 설정
	DDRA=0xFF; //LED
	DDRC=0xFF; //FND
	DDRG=0x0F; //FNDselect
	DDRE=DDRE & ~(1<<PINE4) & ~(1<<PINE5); //sw1,sw2
	DDRF=DDRF & 0b11111110; //ADC
	
	//1. 스위치
	EICRB=EICRB & ~(1<<ISC40) & ~(1<<ISC50); //하강에지 사용
	EICRB=EICRB | (1<<ISC41) | (1<<ISC51);
	EIMSK=EIMSK | (1<<INT4) | (1<<INT5); //스위치인터럽트 허용
	sei();
	
	//2. CTC모드
	TCCR0=TCCR0 | 0b00001000; //CTC모드로 설정 3번비트 1
	TIMSK=TIMSK | 0b00000010; //인터럽트 허용 //sei();
	OCR0=249; //출력 비교값 설정
	TCCR0=TCCR0 | 0b00000100; //분주비 64
	
	//3. ADC
	ADMUX=0x00; //ADC0싱글엔드, AREF, 우측정렬
	ADCSRA=ADCSRA | 0b10001111; //단일변환모드, 인터럽트 활성화, 분주비 128
	ADCSRA=ADCSRA | 0b01000000; //ADC 시작
	
	//변수 선언
	char string[128] = {0,}; //수신할 문자열
    unsigned int cnum;
	
	InitSerial(9600); //Baud Rate 9600으로 USART 초기화
	
    while (1) 
    {
		ReceiveString(string);
		cnum=atoi(string); //문자열을 정수형으로
		
		//on,off
		if(!strcmp(string,"on")){
			PORTA=0xFF;
			fndon=0;
		}
		else if(!strcmp(string,"off")){
			PORTA=0x00;
			fndon=0;
		}
		//LED
		else if(cnum==1){
			PORTA=0x01;
			fndon=0;
		}
		else if(cnum==2)
		{
			PORTA=0x03;
			fndon=0;
		}
		else if(cnum==3)
		{
			PORTA=0x07;
			fndon=0;
		}
		else if(cnum==4)
		{
			PORTA=0x0F;
			fndon=0;
		}
		else if(cnum==5){
			PORTA=0x1F;
			fndon=0;
		}
		else if(cnum==6){
			PORTA=0x3F;
			fndon=0;
		}
		else if(cnum==7){
			PORTA=0x7F;
			fndon=0;
		}
		else if(cnum==8){
			PORTA=0xFF;
			fndon=0;
		}
		//FND
		else if(cnum>=1000 && cnum<=9999)
		{
			//cnumber 업데이트
			for (int i_Digit=0;i_Digit<4;i_Digit++)
			{
				cnumber[i_Digit]=cnum%10;
				cnum/=10;
			}
			fndon=1;
		}
		//이외
		else
		{
			fndon=0;
			for(int i=0;i<3;i++)
			{
				PORTA=0xFF;
				_delay_ms(50);
				PORTA=0x00;
				_delay_ms(50);
			}
		}
    }
	return 0;
}



//연습 모음
//ReceiveString(string); //문자열 수신
//SendString(string); //수신한 문자열 송신(echo)

//SendString("Hello"); //문자열 송신
//_delay_ms(2000);

//ch=ReceiveChar();
//if('1'==ch)PORTA=0x01;
//else if('2'==ch)PORTA=0x03;
//else PORTA=0x00;

//ch=ReceiveChar(); //문자 한개를 Serial 로 수신
//SendChar(ch); //받은 문자를 Serial 로 송신

//SendChar('a');
//_delay_ms(2000);