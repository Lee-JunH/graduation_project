/*
 * timer_CTCmode.c
 *
 * Created: 2023-11-16 오전 12:24:06
 * Author : harry
 */ 

//#실습1

//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
//
//ISR(TIMER0_COMP_vect) //출력비교 인터럽트 서비스루틴
//{
	//static unsigned int count=0;
	//count++;
	//if(1000==count)
	//{
		//PORTA=~PORTA;
		//count=0;
	//}
//}
//
//int main(void)
//{
    //DDRA=0xFF;
	//PORTA=0b00000000;
	//
	//TCCR0=TCCR0 | 0b00001000; //CTC모드로 설정 3번비트 1
	//TIMSK=TIMSK | 0b00000010; //
	//sei();
	//
	//OCR0=249; //출력 비교값 설정
	//TCCR0=TCCR0 | 0b00000100; //분주비 64
	//
    //while (1);
	//
	//return 0;
//}


// 실습#2
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

ISR(TIMER0_OVF_vect) //타이머 오버플로우 서비스루틴
{
	PORTA=0xFF;
}

ISR(TIMER0_COMP_vect) //출력비교 인터럽트 서비스루틴
{
	PORTA=0x00;
}

int main(void)
{
	DDRA=0xFF;
	PORTA=0x00; //LED 초기상태 꺼짐
	
	TCCR0=TCCR0 | 0b01001000; //고속 PWM모드로 설정 6,3번비트 11, OC출력사용 안함 5,4비트 00
	TIMSK=TIMSK | 0b00000011; //오버플로우와 출력비교 인터럽트 둘다 허용
	sei();
	TCCR0=TCCR0 | 0b00000111; //분주비 1024
	while (1)
	{
		for(int i=0; i<255;i++)
		{
			OCR0=i;
			_delay_ms(10);
		}
		for(int i=255; i>0;i--)
		{
			OCR0=i;
			_delay_ms(10);
		}		
	}
	return 0;
}