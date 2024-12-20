/*********************************************************************************************************
**--------------File Info---------------------------------------------------------------------------------
** File name:           IRQ_RIT.c
** Last modified Date:  2014-09-25
** Last Version:        V1.00
** Descriptions:        functions to manage T0 and T1 interrupts
** Correlated files:    RIT.h
**--------------------------------------------------------------------------------------------------------
*********************************************************************************************************/
#include "LPC17xx.h"
#include "RIT.h"
#include "../led/led.h"

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

volatile int B_down=0;

void RIT_IRQHandler (void)
{					
	
	static int J_up = 0;
	static int J_down = 0;
	static int J_right = 0;
	static int J_left = 0;
	
	/*joystick behavior*/
	if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
		/* Joystick UP pressed */
		J_up++;
		switch(J_up){
			case 1:
				/*pacman moves up*/
				break;
			default:
				break;
		}
	}else if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
		/* Joystick RIGHT pressed */
		J_right++;
		switch(J_right){
			case 1:
				/*pacman moves right*/
				break;
			default:
				break;
		}
	}else if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
		/* Joystick LEFT pressed */
		J_left++;
		switch(J_left){
			case 1:
				/*pacman moves left*/
				break;
			default:
				break;
		}
	}else if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
		/* Joystick DOWN pressed */
		J_down++;
		switch(J_down){
			case 1:
				/*pacman moves down*/
				break;
			default:
				break;
		}
	}else{
		J_up = 0;
		J_down = 0;
		J_left = 0;
		J_right = 0;
	}
	
	/* button management */
	if(B_down>=1){ 
		if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
			switch(B_down){				
				case 2:				/* pay attention here: please see slides to understand value 2 */
				/*INT0 behavior*/
				
					break;
				default:
					break;
			}
			B_down++;
		}
		else {	/* button released */
			B_down=0;			
			NVIC_EnableIRQ(EINT1_IRQn);							 /* enable Button interrupts			*/
			LPC_PINCON->PINSEL4    |= (1 << 22);     /* External interrupt 0 pin selection */
		}
	}
/*	else{
			if(down==1)
				down++;
	} */
	
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
