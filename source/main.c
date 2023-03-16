#include "MKL05Z4.h"
#include "pit.h"
#include "buttons.h"
#include "stepper.h"
#include "lcd1602.h"
#include "uart0.h"

char temp;
uint8_t start = 1;

void UART0_IRQHandler()
{
	if(UART0->S1 & UART0_S1_RDRF_MASK)
	temp=UART0->D;
	LCD1602_SetCursor(0,1);
	LCD1602_Print("jest");
	if(temp == '^') start = 0;
	UART0->D = temp;
	NVIC_EnableIRQ(UART0_IRQn);
}

int main(void) {
	
	UART0_Init();
	
	
	LCD1602_Init();
	LCD1602_ClearAll();
	LCD1602_SetCursor(0,0);
	LCD1602_Print("READY FOR START");
	
	while(start) __NOP();
	
	stepperInitialize();
	pitInitialize((10/2)*20900 +1);       	
	
	startPIT(); 

	
		
	while(1){	
	 __WFI(); // Save energy and wait for interrupt
	}
	
	
}
