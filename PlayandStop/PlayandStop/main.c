/*
 * PlayandStop.c
 *
 * Created: 2023-10-28 오후 1:04:21
 * Author : harry
 */ 

//과제 6주차
#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	DDRA=0xFF;
	DDRC=0xFF;
	DDRE=DDRE & ~(1<<PINE4) & ~(1<<PINE5);
	DDRG=0xFF;
	PORTA=0x01;
	unsigned char play_pattern[]={0x73, 0x38, 0x77, 0x66, 0x6D, 0x78, 0x3F, 0x73};
	unsigned char fnd_pattern[]={0x08, 0x04,0x02, 0x01};
	int direc=-1;
	int count=0;
	
	while(1)
	{
		//스위치1 누르면 PLAY 출력 P:a,b,e,f,g(0x73) | L:d,e,f(0x38) | A:a,b,c,e,f,g(0x77) | Y:b,c,f,g(0x66)
		if(!(PINE & (1<<PINE4)))
		{
			count=0;
			if(direc==-1)
			{
				_delay_ms(1);
				PORTA=PORTA<<1;
			}
			else
			{
				_delay_ms(1);
				PORTA=PORTA>>1;
			}
			if(PORTA==0x80)direc=1;
			if(PORTA==0x01)direc=-1;
			
			while(!(PINE & (1<<PINE4)))
			{
				for(int i=0;i<4;i++)
				{
					PORTG=fnd_pattern[i];
					PORTC=play_pattern[i];
					_delay_ms(1);
				}
			}
		}
		//스위치2 누르면 STOP S:a,c,d,f,g(0x6D) | t:d,e,f,g(0x78) | O:a,b,c,d,e,f(0x3F) | P:a,b,e,f,g(0x73)
		else if(!(PINE & (1<<PINE5)))
		{
			count=0;
			for(int i=4;i<8;i++)
			{
				PORTG=fnd_pattern[i-4];
				PORTC=play_pattern[i];
				_delay_ms(1);
			}
			while(count!=2)
			{
				for(int i=4;i<8;i++)
				{
					PORTG=fnd_pattern[i-4];
					PORTC=play_pattern[i];
					_delay_ms(1);
				}
				if(!(PINE & (1<<PINE5)))
				{
					count=count+1;
					while(!(PINE & (1<<PINE5)))
					{
						for(int i=4;i<8;i++)
						{
							PORTG=fnd_pattern[i-4];
							PORTC=play_pattern[i];
							_delay_ms(1);
						}
					}
				}
			}
		}
		//버튼을 누르지 않을 때
		else
		{
			for(int i=0;i<4;i++)
			{
				PORTG=fnd_pattern[i];
				PORTC=play_pattern[i];
				_delay_ms(1);
			}
		}
	}
	return 0;
}

