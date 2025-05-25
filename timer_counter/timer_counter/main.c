/*
* timer_counter.c
*
* Created: 2023-11-09 오전 9:10:21
* Author : harry
*/

//실습 #1-1
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char SegNum[16]=
{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

int num=0; //전역변수로 사용
ISR(TIMER0_OVF_vect)
{
	static unsigned char count=0;
	count++;
	if(61==count) //인터럽트 61번 발생시 num++
	{
		count=0;
		num=num+1;
		if(10000==num) num=0;
	}
}

int main(void)
{
    DDRC=0xFF;
    DDRG=0x0F;
    
	TCCR0=TCCR0 & 0b00000111; //일반모드로 설정
	TIMSK=TIMSK | 0b00000001; //인터럽트 허용
	sei(); //전체 인터럽트 허용
	TCCR0=TCCR0 | 0b00000111; //분주비를 1024로 설정
	
	unsigned char cnumber[4] = {0,0,0,0}; //cnumber[] 선언 및 초기값 설정
	while(1)
	{
		//FND 출력
		PORTG=0x01<<0;
		PORTC=SegNum[cnumber[3]]; //1의자리
		_delay_ms(1);
		PORTG=0x01<<1;
		PORTC=SegNum[cnumber[2]]; //10의 자리
		_delay_ms(1);
		PORTG=0x01<<2;
		PORTC=SegNum[cnumber[1]]; //100의 자리
		_delay_ms(1);
		PORTG=0x01<<3;
		PORTC=SegNum[cnumber[0]]; //1000자리
		_delay_ms(1);
		
		int t=num;
		for(int i_Digit=0;i_Digit<4;i_Digit++)
		{
			cnumber[3-i_Digit]=t%10; //t=1234 cnumber[3]=4
			t=t/10;
		}
	}
    return 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////
//실습 #1-2
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char SegNum[16]=
{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

int num=0;
ISR(TIMER0_OVF_vect)
{
	static unsigned char count=0;
	count++;
	if(125==count) //인터럽트 125번 발생시 num++
	{
		count=0;
		num=num+1;
		if(10000==num) num=0;
	}
	TCNT0=131; //타이머의 초기값 131로 설정 -> 131+125=256
}

int main(void)
{
	DDRC=0xFF;
	DDRG=0x0F;
	
	TCCR0=TCCR0 & 0b00000111; //일반모드로 설정
	TIMSK=TIMSK | 0b00000001; //인터럽트 허용
	sei();
	TCCR0=TCCR0 | 0b00000111; //분주비를 1024로 설정
	TCNT0=131; //타이머의 초기값 131로 설정 -> 131+125=256
	
	unsigned char cnumber[4] = {0,0,0,0}; //cnumber[] 선언 및 초기값 설정
	while(1)
	{
		//FND 출력
		PORTG=0x01<<0;
		PORTC=SegNum[cnumber[3]]; //1의자리
		_delay_ms(1);
		PORTG=0x01<<1;
		PORTC=SegNum[cnumber[2]]; //10의 자리
		_delay_ms(1);
		PORTG=0x01<<2;
		PORTC=SegNum[cnumber[1]]; //100의 자리
		_delay_ms(1);
		PORTG=0x01<<3;
		PORTC=SegNum[cnumber[0]]; //1000자리
		_delay_ms(1);
		//cnumber 업데이트
		int t=num;
		for(int i_Digit=0;i_Digit<4;i_Digit++)
		{
			cnumber[3-i_Digit]=t%10; //t=1234 cnumber[3]=4
			t=t/10;
		}
	}
	return 0;
}

/////////////////////////////////////////////////////////////////////////////////////////////
//실습 #2
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LEFT 0
#define RIGHT 1

unsigned char direction=LEFT; //초기 방향 LEFT
ISR(TIMER0_OVF_vect)
{
	static unsigned char count=0;
	count++;
	if(125==count) //인터럽트 125번 발생시
	{
		count=0; //count 초기화
		if(LEFT==direction)
		{
			PORTA=PORTA<<1;
		}
		else
		{
			PORTA=PORTA>>1;
		}
	}
	TCNT0=131; //초기값 131로 설정 -> 131+125=256
}

int main(void)
{
	DDRA=0xFF;
	DDRE=DDRE & ~(1<<PINE4) & ~(1<<PINE5);
	PORTA=0b00000001;
	
	TCCR0=TCCR0 & 0b00000111; //일반모드로 설정
	TIMSK=TIMSK | 0b00000001; //인터럽트 허용
	sei(); //전체 인터럽트 허용
	TCCR0=TCCR0 | 0b00000111; //분주비를 1024로 설정
	TCNT0=131; //초기값 131로 설정 -> 131+125=256
	
	while(1)
	{
		if(0x80==PORTA) direction=RIGHT;
		if(0x01==PORTA) direction=LEFT;
		//스위치1,2 구동
		if(!(PINE & (1<<PINE4))) direction=RIGHT;
		if(!(PINE & (1<<PINE5))) direction=LEFT;
	}
	return 0;
}