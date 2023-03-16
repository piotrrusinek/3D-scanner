#include "MKL05Z4.h"                   
#include "stepper.h"
#include "lcd1602.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

static uint16_t period = 1000;
static uint8_t steps = 0;
static uint8_t rounds = 0;


/*
first stepper motor step -  PTA6
second stepper motor step -  PTA5
first stepper motor dir - PTA7
*/

void stepperInitialize(void)
{
	SIM->SCGC5 |=  SIM_SCGC5_PORTA_MASK; //enable clock for portA
	PORTA->PCR[6] = PORT_PCR_MUX(1); //set PTA6 as GPIO
	PORTA->PCR[7] = PORT_PCR_MUX(1); //set PTA7 as GPIO
	PORTA->PCR[5] = PORT_PCR_MUX(1); //set PTA5 as GPIO
	FPTA->PDDR |= (1 << 6) | (1 << 7) | (1 << 8); //set PTA6, PTA7, PTA5 as output
	
	FPTA->PCOR |= (1 << 6);
	FPTA->PCOR |= (1 << 7);
	FPTA->PCOR |= (1 << 5);
	
	LCD1602_Init();
	LCD1602_ClearAll();
	LCD1602_SetCursor(0,0);
	LCD1602_Print("status:");
	
}

void stepperPulse(void)
{
	
	if(rounds<100)
	{
		FPTA->PTOR |= (1 << 6);
		FPTA->PTOR |= (1 << 6);
		steps++;
		
		//send measured value
		char rx_buf[7] = "123456";
		
		int i;
		
		for(i=0;rx_buf[i]!=0;i++)	
		{
			while(!(UART0->S1 & UART0_S1_TDRE_MASK));	
			UART0->D = rx_buf[i];
		}
		while(!(UART0->S1 & UART0_S1_TDRE_MASK));	
		UART0->D = 0xa;	
		
		if(steps >= 200)
		{
			stepperDir();
			stepperPulseSecond();
			rounds++;
			steps = 0;
			if(rounds%10 == 0) updateLCD();
			
			LCD1602_SetCursor(11,1);
			LCD1602_PrintNum(rounds);
			LCD1602_SetCursor(14,1);
			LCD1602_Print("%");
			
			
			
		}
	}
		
}

void stepperPulseSecond(void)
{
	FPTA->PTOR |= (1 << 5);
	FPTA->PTOR |= (1 << 5);
}

void stepperDir(void)
{
	FPTA->PTOR |= (1 << 7);
}

void stepperFaster(void)
{
	period+=1;
	
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV((period/2)*20900 +1);
}

void stepperSlower(void)
{
	if(period>1) period-=1;
	
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV((period/2)*20900 +1);
}

void  updateLCD(void)
{
	if((rounds/10)>0)
	{
		
		LCD1602_SetCursor((uint8_t)(rounds/10)-1,1);
		//LCD1602_Write8(124,1);
		LCD1602_Print("|");
		
	}
}

