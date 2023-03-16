#include "MKL05Z4.h"                    
#include "pit.h"												
#include "stepper.h"

static const IRQn_Type myPIT_IRQn = PIT_IRQn;  



void PIT_IRQHandler() {
		
	// check to see which channel triggered interrupt 
	if (PIT->CHANNEL[0].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 0
		PIT->CHANNEL[0].TFLG &= PIT_TFLG_TIF_MASK;
		
		
		
		stepperPulse();
		
		
		
		
	} else if (PIT->CHANNEL[1].TFLG & PIT_TFLG_TIF_MASK) {
		// clear status flag for timer channel 1
		PIT->CHANNEL[1].TFLG &= PIT_TFLG_TIF_MASK;
	}

	//clear pending IRQ
	NVIC_ClearPendingIRQ(myPIT_IRQn);
	
}

 /*----------------------------------------------------------------------------
  Function that initializes PIT timer
 *----------------------------------------------------------------------------*/
void pitInitialize(unsigned period) {

	// Enable clock to PIT module
	SIM->SCGC6 |= SIM_SCGC6_PIT_MASK; 
	
	// Enable module, freeze timers in debug mode
	PIT->MCR &= ~PIT_MCR_MDIS_MASK; 
	PIT->MCR |= PIT_MCR_FRZ_MASK;   
	
	//calculate proper ldval here:
	
	// Initialize PIT0 to count down from argument 
	PIT->CHANNEL[0].LDVAL = PIT_LDVAL_TSV(period);

	// No chaining
	PIT->CHANNEL[0].TCTRL &= PIT_TCTRL_CHN_MASK;
	
	// Generate interrupts
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TIE_MASK;

	/* Enable Interrupts */
	NVIC_SetPriority(myPIT_IRQn, 2); 

	NVIC_ClearPendingIRQ(myPIT_IRQn); 
	NVIC_EnableIRQ(myPIT_IRQn);	

}



void startPIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL |= PIT_TCTRL_TEN_MASK;
}

void stopPIT(void) {
// Enable counter
	PIT->CHANNEL[0].TCTRL &= ~PIT_TCTRL_TEN_MASK;
}
