#include <compiler_defs.h>
#include <C8051F020_defs.h>


void ADC0_function(void){
    // reference control reg
	REF0CN = 0x03;   //internal reference voltage + internal bias genertaor w b zero ba2y el register

	// configuration reg for ADC0
	ADC0CF = 0x00; // awel 3 bits equal zero to make gain=0, other 5 to make conv. clk= sys clk

    //mux 0 configuration
	AMX0CF = 0x00;			 // set zeros 3alshan independent
	// selection
	AMX0SL = 0x02;			 // 3alshan a5tar el AIN2
	ADC0CN = 0x81; 			 // ADC Control // el 8 3alshan a5ly el mode TH=ta5od 8 bit w el TL = a5od 4 bit
}

void DAC0_function(void){
	DAC0CN = 0x84; // enable the 7th bit and data into high then low 
	
}

unsigned int input[3] = {0,0,0};


void init_intClock(void)
{
	OSCXCN = 0x00;
	OSCICN = 0x14; // should be put as 0x14 to make it work	 but originally is it 0x04
	while (!(OSCICN & 0x10)); // Wait till IFRDY ( bit no 4 ) pin is set	, not working ,why if ^4 is 0
	/* Missing Clock Enable Bit(7) is 0 ( disabled) 
	  bits [ 1 : 0] are 10 for 2MHz
	  bit (2) is 1 to enable internal clock
	  bit (3) is 0 to disable external clock
	  */
}


void main(void){

WDTCN = 0xde;
WDTCN = 0xad;
init_intClock();
ADC0_function();
DAC0_function();

XBR2 = 0x40; // to take input from ADC0  

	while(1){
	AD0BUSY = 1; //start AD conversion
	while(!AD0INT);
	AD0INT = 0;  //clr flag
	input[0] = input[1];
	input[1] = input[2];
	input[2]= ADC0;
	DAC0 = (input[2] + input[1]); // --> LPF
	//DAC0 = (input[2] + input [0]); --> Notch Filter
	//DAC0 = (input[2] - input [1]); --> HPF 

	}
}