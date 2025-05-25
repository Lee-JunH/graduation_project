/*
 * 230919_Test.c
 *
 * Created: 2023-09-19 오후 12:59:49
 * Author : harry
 */ 


#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define LEFT 0
#define RIGHT 1

unsigned char SegNum[16] =
{
	0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07,
	0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71
};

unsigned char direction = LEFT;

ISR(TIMER0_OVF_vect)
{
	static unsigned char count_t = 0;
	
	count_t++;
	if(count_t==125)
	{
		if(direction)
		{
			PORTA = PORTA >> 1;
			if(PORTA == 0x01)
			{
				direction=LEFT;
			}
		}
		else
		{
			PORTA = PORTA << 1;
			if(PORTA == 0x80)
			{
				direction=RIGHT;
			}
		}
		count_t=0;
	}
	TCNT0=130;
}

int main(void)
{
	DDRA=0xFF;
	DDRE=DDRE & ~(1<<PINE4) & ~(1<<PINE5);
	PORTA=0b00000001;
	
	TCCR0 = TCCR0 & 0b10000111; // 일반 모드로 설정/ 출력비교 사용 안함
	TIMSK = TIMSK | 0b00000001; // 타이머/카운터0 오버플로우 인터럽트 허용
	sei(); // 전체 인터럽트 허용
	TCCR0 = TCCR0 | 0b00000111; // 분주기 1024 설정
	TCNT0 = 130; // TCNT0 초기값 설정

	while (1)
	{
		if(PORTA!=0x01)
		{
			if(!(PINE & (1<<PINE4)))
			{
				direction = RIGHT;
			}
		}
		if(PORTA!=0x80)
		{
			if(!(PINE & (1<<PINE5)))
			{
				direction = LEFT;
			}
		}
	}
	return 0;
}


//#define __DELAY_BACKWARD_COMPATIBLE__
//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
//
//int stopwatch=0;
//int pause=0;
//
//ISR(INT4_vect)
//{
	//stopwatch=1;
	//pause=0;
//}
//ISR(INT5_vect)
//{
	//pause=1;
	//stopwatch=0;
//}
//
//unsigned char SegNum[16]=
//{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
//0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
//
//int main(void)
//{
	//DDRC=0xFF;
	//DDRG=0x0F;
	//DDRE=DDRE & ~(1<<PINE4) & ~(1<<PINE5);
	//
	//int t=0;
	//unsigned char cnumber[4] = {0,0,0,0};
	//
	//EICRB = EICRB & ~(1<<ISC40);
	//EICRB = EICRB | (1<<ISC41);
	//EIMSK = EIMSK | (1<<INT4);
	//sei();
//
	//EICRB = EICRB & ~(1<<ISC50);
	//EICRB = EICRB | (1<<ISC51);
	//EIMSK = EIMSK | (1<<INT5);
	//sei();
	//
	//while(1)
	//{
		//for(int i_4ms = 0;i_4ms <25;i_4ms++)
		//{
			//PORTG=0x01<<0;
			//PORTC=SegNum[cnumber[0]]; //1의자리
			//_delay_ms(1);
			//PORTG=0x01<<1;
			//PORTC=SegNum[cnumber[1]]; //10의 자리
			//_delay_ms(1);
			//PORTG=0x01<<2;
			//PORTC=SegNum[cnumber[2]]; //100의 자리
			//_delay_ms(1);
			//PORTG=0x01<<3;
			//PORTC=SegNum[cnumber[3]]; //1000자리
			//_delay_ms(1);
		//}
		//if(!stopwatch || pause) t=t;
		//else if(stopwatch) t++;
		//
		//if(10000==t) t=0;
		//
		//int num=t; // t와 같은 num 변수 선언
		//for(int i_Digit=0;i_Digit<4;i_Digit++)
		//{
			//cnumber[i_Digit]=num%10; //ex) 1234 일때 4는 cnumber[0]에 넣기
			//num=num/10;//나눠서 123만 남도록
		//}
	//}
	//return 0;
//}
	
	
	
	//과제 7주차
	//unsigned char prev, now;
	//
	//DDRA=0xFF;
	//DDRE=DDRE & ~(1<<PINE4);
	//PORTA=0b00000001;
	//
	//
	//if((PINE & (1<<PINE4)))
	//{
		//prev=HIGH;
		//now=HIGH;
	//}
	//else
	//{
		//prev=LOW;
		//now=LOW;
	//}
	//
	//while(1)
	//{
		//if(PINE & (1<<PINE4)) now=HIGH;
		//else now=LOW;
		//
		//if((prev==HIGH) && (now==LOW))
		//{
			//PORTA=PORTA<<1;
			//if(0==PORTA) PORTA=0b00000001;
			//
			//_delay_ms(100);
			//while(!(PINE & (1<<PINE4)));
			//_delay_ms(100);
		//}
		//prev=now;
	//}
	//return 0;