/*
 * 231010.c
 *
 * Created: 2023-10-10 오전 10:28:03
 * Author : harry
 */ 

#define __DELAY_BACKWARD_COMPATIBLE__
#include <avr/io.h>
#include <util/delay.h>

int main(void)
{	
	//과제#3
	DDRA=0xFF;
	PORTA=0x01;
	DDRE = DDRE & (~(1<<PINE4) & ~(1<<PINE5));
	int i_push=0;//스위치 누른 횟수를 저장하는 변수
	int i_time=1000;//딜레이 할 시간 변수
	
    while (1)
    {
		if (PORTA==0x01)
		{
			for(int j=1;j<8;j++)
			{
				if(!(PINE & (1<<PINE4)))
				{
					_delay_ms(25);
					i_push=i_push-1;
				}
				else if(!(PINE & (1<<PINE5)))
				{
					_delay_ms(25);
					i_push=i_push+1;
				}
				i_time=1000+(100*i_push);
				if(i_time<=100)
				{
					i_push=-9;
					i_time=100;
				}
				else if(i_time>=2000)
				{
					i_push=10;
					i_time=2000;
				}
				_delay_ms(i_time);
				PORTA=PORTA<<1;
			}
		}
		if(PORTA==0x80)
		{
			for(int j=1;j<8;j++)
			{
				if(!(PINE & (1<<PINE4)))
				{
					_delay_ms(25);
					i_push=i_push-1;
				}
				else if(!(PINE & (1<<PINE5)))
				{
					_delay_ms(25);
					i_push=i_push+1;
				}
				i_time=1000+(100*i_push);
				if(i_time<=100)
				{
					i_push=-9;
					i_time=100;
				}
				if(i_time>=2000)
				{
					i_push=10;
					i_time=2000;
				}
				
				_delay_ms(i_time);
				PORTA=PORTA>>1;
			}
		}
    }
	return 0;
	
	//#과제2
	DDRA=0xFF;
	PORTA=0x01;
	int direction=-1;//방향변수 선언(처음은 왼쪽(-1)으로 시작)
	while(1)
	{
		if(!(PINE & (1<<PINE4))) //스위치 1을 누르면 오른쪽(1)으로 방향 전환
		{
			direction=1;
		}
		if(!(PINE & (1<<PINE5)))//스위치2를 누르면 왼쪽(-1)으로 방향전환
		{
			direction=-1;
		}
		if(direction==1)//오른쪽(1)방향이동
		{
			_delay_ms(200);
			PORTA=PORTA>>1;
			if(PORTA==0x01)direction=-1;//끝에 도달하면 방향변수 전환
		}
		else//왼쪽(-1)방향이동
		{
			_delay_ms(200);
			PORTA=PORTA<<1;
			if(PORTA==0x80)direction=1;//끝에 도달하면 방향변수 전환
		}
	}
	return 0;
}

