/*
 * adc_control.c
 *
 * Created: 2023-11-28 오전 2:07:51
 * Author : harry
 */ 

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

unsigned char SegNum[16]=
{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

unsigned char cnumber[4]={0,}; // FND 출력할 자리수 배열

ISR(TIMER0_OVF_vect) //타이머 오버플로우 인터럽트
{
	PORTA=0xFF;
}

ISR(TIMER0_COMP_vect) //출력비교 인터럽트
{
	PORTA=0x00;
}

unsigned int bright;
ISR(ADC_vect)
{
	unsigned int ADC_Data;
	ADC_Data=ADCW;
	
	//밝기 업데이트
	bright=255-((float)255/1023)*ADC_Data;
	
	//cnumber 업데이트
	for (int i_Digit=0;i_Digit<4;i_Digit++)
	{
		cnumber[i_Digit]=ADC_Data%10;
		ADC_Data/=10;
	}
}

int main(void)
{
	DDRA=0xFF; //LED	
	DDRC=0xFF; //FND
	DDRG=0x0F; //FNDselect
	DDRF=DDRF & 0b11111110; //PORTF를 입력으로 설정
	
	ADMUX=0x00; //ADC0싱글엔드, AREF, 우측정렬
	ADCSRA=ADCSRA | 0b10001111; //단일변환모드, 인터럽트 활성화, 분주비 128
	
	TCCR0=TCCR0 | 0b01001000; //고속 PWM모드설정, OC0출력사용안함
	TIMSK=TIMSK | 0b00000011; //타이머_카운터, 오버플로우 인터럽트 허용
	sei();

	TCCR0=TCCR0 | 0b00000111; //분주비 1024
	ADCSRA=ADCSRA | 0b01000000; //ADC 시작
	
    while(1)
    {
		ADCSRA=ADCSRA | 0b01000000;
		//FND 출력
		for (int i=0;i<4;i++)
		{
			PORTG=0x01<<i;
			PORTC=SegNum[cnumber[i]];
			_delay_ms(1);
		}
		OCR0=bright;
	}
	return 0;
}


//#include <avr/io.h>
//#include <util/delay.h>
//#include <avr/interrupt.h>
//
//unsigned char SegNum[16]=
//{0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F,
//0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
//
//unsigned char cnumber[4]={0,}; // FND 출력할 자리수 배열
//unsigned int bright;
//
//ISR(TIMER0_COMP_vect) //출력비교 인터럽트 서비스루틴
//{
	//static unsigned int count=0;
	//
	//count++;
	//if(bright>=1003) //강한 빛 감지시
	//{
		//if(count==3000) //3초 카운트
		//{
			//PORTA=0xFF; //LED on
		//}
	//}
	//else //평상시에는 LED off, count=0
	//{
		//PORTA=0x00;
		//count=0;
	//}
//}
//
//ISR(ADC_vect)
//{
	//unsigned int ADC_Data;
	//ADC_Data=ADCW;
	//
	//bright=ADC_Data;
	//
	//// ADC값 FND 업데이트
	//for(int i_Digit=0;i_Digit<4;i_Digit++)
	//{
		//cnumber[i_Digit]=ADC_Data%10;
		//ADC_Data/=10;
	//}
//}
//
//int main(void)
//{
	//DDRA=0xFF; //LED
	//PORTA=0b00000000;
	//
	//DDRC=0xFF; //FND
	//DDRG=0x0F; //FND Select
	//DDRF=DDRF | 0b11111110; //PORTF 출력설정
	//
	//ADMUX=0x00; //ADC0싱글엔드, AREF, 우측정렬
	//ADCSRA=ADCSRA | 0b10001111; //단일변환모드, 인터럽트 활성화, 분주비 128
	//
	//TCCR0=TCCR0 | 0b00001000; //CTC모드로 설정 3번비트 1
	//TIMSK=TIMSK | 0b00000010; //인터럽트 허용
	//sei();
	//
	//OCR0=249; //출력 비교값 설정
	//TCCR0=TCCR0 | 0b00000100; //분주비 64
	//ADCSRA=ADCSRA | 0b01000000; //ADC 시작
	//
	//while(1)
	//{
		//ADCSRA=ADCSRA | 0b01000000; //ADC 시작
		//for (int i=0;i<4;i++)
		//{
			//PORTG=0x01<<i;
			//PORTC=SegNum[cnumber[i]];
			//_delay_ms(1);
		//}
	//}
	//
	//return 0;
//}