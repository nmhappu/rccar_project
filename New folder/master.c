#include <lpc214x.h>
#define clk 60000000



void pllInit(){
	PLL0CFG = 0X24;
	PLL0CON = 0X01;
	PLL0FEED = 0XAA;
	PLL0FEED = 0X55;
	while(PLL0STAT & 0X400 == 0);
	PLL0CON = 0X03;
	VPBDIV = 0X01;
	PLL0FEED = 0XAA;
	PLL0FEED = 0X55;
}

void uartInit(int baudrate){
	unsigned int DL = (clk / 16) / baudrate;
	//PINSEL0 = 0X00000005;
	U0LCR = 0X83;
	U0DLM = DL >> 8; // to take only the msb part
	U0DLL = DL & 0XFF; // to take only the lsb part 
	U0LCR = 0X03;
	
}
void spiSend(char data){
	//IODIR0 = 0X000000D0;
	IOCLR0 = 0X00000080;
	S0SPCR = 0X00000020;
	S0SPCCR = 0X0A;
	S0SPDR = data;
	while(!(S0SPSR & (0X01 << 7)));
}


int main(){
	char ch = 'm';
	PINSEL0 = 0X00005505;
	pllInit();
	uartInit(9600);
	while(1){
	spiSend(ch);
	
	U0THR = ch;
	while(!(U0LSR & 0x20));
	}
	
}
