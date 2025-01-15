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
#include "../GLCD/GLCD.h"
#include "../Game/game.h"
#include "../music/music.h"

/*comment the define if you want to use emulator's default scaling*/
#define CUSTOM_SCALING

/******************************************************************************
** Function name:		RIT_IRQHandler
**
** Descriptions:		REPETITIVE INTERRUPT TIMER handler
**
** parameters:			None
** Returned value:		None
**
******************************************************************************/

//1 s = 20 chiamate a RIT_IRQ
volatile int B_down=0;


void RIT_IRQHandler (void)
{					
	
	static int J_up = 0;
	static int J_down = 0;
	static int J_right = 0;
	static int J_left = 0;
	
	static int currentNote = 0;
	static int ticks = 0;
	
#ifndef CUSTOM_SCALING
	disable_RIT();
	reset_RIT();
#endif
	

	if(game_state.curr_state == PLAY){
		/*joystick behavior*/
		if((LPC_GPIO1->FIOPIN & (1<<29)) == 0){	
			/* Joystick UP pressed */
			J_up++;
			if(J_up >= 1)
				pacman_change_dir(G_UP);
			
		}else{
			J_up = 0;
		}
		
		if((LPC_GPIO1->FIOPIN & (1<<28)) == 0){
			/* Joystick RIGHT pressed */
			J_right++;
			if(J_right >= 1)
				pacman_change_dir(G_RIGHT);

		}else{
			J_right = 0;
		}
		
		if((LPC_GPIO1->FIOPIN & (1<<27)) == 0){
			/* Joystick LEFT pressed */
			J_left++;
			if(J_left >= 1)
					pacman_change_dir(G_LEFT);
			
		}else{
			J_left = 0;
		}
		
		if((LPC_GPIO1->FIOPIN & (1<<26)) == 0){
			/* Joystick DOWN pressed */
			J_down++;
			if(J_down >= 1)
				pacman_change_dir(G_DOWN);
			
		}else{
			J_down = 0;
		}
		
		game_update();
	}
		
		/* button management */
		if(B_down>=1){ 
			if((LPC_GPIO2->FIOPIN & (1<<10)) == 0){	/* INT0 pressed */
				switch(B_down){				
					case 2:		
					/*INT0 behavior*/
					
					//pause and restart game when pressing the button
						if(game_state.curr_state == PLAY){
							game_pause();
						}
						else if(game_state.curr_state == PAUSE){
							game_resume();
						}
						
						break;
					default:
						break;
				}
				B_down++;
			}
			else {	/* button released */
				B_down=0;			
				NVIC_EnableIRQ(EINT0_IRQn);							 /* enable Button interrupts			*/
				LPC_PINCON->PINSEL4    |= (1 << 20);     /* External interrupt 0 pin selection */
			}
		}
		
		/*music management*/
		if(!isNotePlaying()){
			++ticks;
			if(ticks == UPTICKS){
				ticks = 0;
				playSound();
			}
		}
	
		
		/* capire a che serve
		(currentNote == (sizeof(theme) / sizeof(theme[0])))
	  {
			disable_RIT();
	  }*/
		
#ifndef CUSTOM_SCALING
	enable_RIT();
#endif		
		
  LPC_RIT->RICTRL |= 0x1;	/* clear interrupt flag */
	
  return;
}

/******************************************************************************
**                            End Of File
******************************************************************************/
