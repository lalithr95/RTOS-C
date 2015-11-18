#include <reg51.h>
#define LCD P2
#define ADC P0
sbit A0 = P1^0;
sbit A1 = P1^1;
sbit A2 = P1^2;
sbit START = P1^3;
sbit ALE = P1^4;
sbit EOC = P1^5;
#define VREF 4.96
#define MAX_REF 0xff

void init_lcd();
void cmd_lcd(unsigned char c);
void write_lcd(unsigned char c);
void display_lcd(unsigned char *c);
void integer_lcd(int n);
void float_lcd(float f);
void delay_ms(unsigned int i);

void main(void) {
	init_lcd();
	A0 = 0;
	A1 = 0;
	A2 = 0;
	START = 0;
	ALE = 0;
	display_lcd("Data ");
	while(1) {
		cmd_lcd(0xc0);
		START = 1;
		ALE = 0;
		START = 0;
		while(EOC == 0);
		float_lcd(VREF * ADC/MAX_REF);
		display_lcd(" volts");
		delay_ms(1000);
	}
}

void init_lcd(void) {
	delay_ms(10);
	cmd_lcd(0x28);
	cmd_lcd(0x0e);
	cmd_lcd(0x06);
	cmd_lcd(0x01);
}

void cmd_lcd(unsigned char c) {
	unsigned char temp;
	temp = c>>4;
	LCD = temp<<4 | 0x02;
	LCD = 0;
	LCD = c<<4 | 0x02;
	LCD = 0;
	delay_ms(20);
}

void write_lcd(unsigned char c) {
	unsigned char temp;
	temp = c>>4;
	LCD = temp<<4 | 0x03;
	LCD = 0;
	LCD = c<<4 | 0x03;
	LCD = 0;
	delay_ms(20);
}

void display_lcd(unsigned char *s) {
	while(*s) {
		write_lcd(*s++);
	}
}

void integer_lcd(int n) {
	unsigned char c[6];
	unsigned int i=0;
	if (n<0) {
		write_lcd('-');
		n = -n;
	}
	if (n==0) {
		write_lcd('0');
	}
	while(n>0) {
		c[i++] = n%10;
		n = n/10;
	}
	while(i-->=1) {
		write_lcd(c[i]);
	}
}

void float_lcd(float f) {
	int n;
	float temp;
	n = f;
	integer_lcd(n);
	write_lcd('.');
	temp = f-n;
	if(temp >= 0.00 && temp <= 0.09) {
		write_lcd('0');
	}
	f = temp*100;
	n = f;
	integer_lcd(n);
}

void delay_ms(unsigned int i) {
	unsigned int j;
	while(i-->0) {
		for(j=0;j<500;j++) {
			
		}
	}
}
