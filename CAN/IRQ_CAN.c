/*----------------------------------------------------------------------------
 * Name:    Can.c
 * Purpose: CAN interface for for LPC17xx with MCB1700
 * Note(s): see also http://www.port.de/engl/canprod/sv_req_form.html
 *----------------------------------------------------------------------------
 * This file is part of the uVision/ARM development tools.
 * This software may only be used under the terms of a valid, current,
 * end user licence from KEIL for a compatible version of KEIL software
 * development tools. Nothing else gives you the right to use this software.
 *
 * This software is supplied "AS IS" without warranties of any kind.
 *
 * Copyright (c) 2009 Keil - An ARM Company. All rights reserved.
 *----------------------------------------------------------------------------*/

#include <LPC17xx.h>                  /* LPC17xx definitions */
#include "CAN.h"                      /* LPC17xx CAN adaption layer */
#include <stdio.h>
#include "../Game/game.h"
#include "../GLCD/GLCD.h"

extern uint8_t icr ; 								//icr and result must be global in order to work with both real and simulated landtiger.
extern uint32_t result;
extern CAN_msg       CAN_TxMsg;    /* CAN message for sending */
extern CAN_msg       CAN_RxMsg;    /* CAN message for receiving */

int msg_score;
int msg_counter;
int msg_lives;

/*----------------------------------------------------------------------------
  CAN interrupt handler
 *----------------------------------------------------------------------------*/
void CAN_IRQHandler (void)  {
	static int prev_score;
	static int prev_counter;
	static int prev_lives = 0;

  /* check CAN controller 1 */
	icr = 0;
  icr = (LPC_CAN1->ICR | icr) & 0xFF;               /* clear interrupts */
	
  if (icr & (1 << 0)) {                          		/* CAN Controller #1 meassage is received */
		CAN_rdMsg (1, &CAN_RxMsg);	                		/* Read the message */
    LPC_CAN1->CMR = (1 << 2);                    		/* Release receive buffer */
	}
		
	/* check CAN controller 2 */
	icr = 0;
	icr = (LPC_CAN2->ICR | icr) & 0xFF;             /* clear interrupts */

	if (icr & (1 << 0)) {                          	/* CAN Controller #2 meassage is received */
		CAN_rdMsg (2, &CAN_RxMsg);	                		/* Read the message */
		LPC_CAN2->CMR = (1 << 2);                    		/* Release receive buffer */
		
		//	CAN_TxMsg.data[0] = d0_score;
		//	CAN_TxMsg.data[1] = d1_score;
		//	CAN_TxMsg.data[2] = d2_remaining_lives;
		//	CAN_TxMsg.data[3] = d3_remaining_time;	
		msg_score = (CAN_RxMsg.data[1] << 8) | CAN_RxMsg.data[0];
		msg_lives = CAN_RxMsg.data[2];
		msg_counter = CAN_RxMsg.data[3]; 
		
		if (msg_score != prev_score){
				print_number(msg_score, MAX_X-80, 17, White, Black);
				prev_score = msg_score;
		}
		if (msg_lives != prev_lives){
				game_display_life(msg_lives);
				prev_lives = msg_lives;
		}
		/*if (msg_lives < prev_lives){
				game_lose_life(msg_lives);
				prev_lives = msg_lives;
		}*/
		if (msg_counter != prev_counter){
				print_number(msg_counter, 0, 17, White, Black);
				prev_counter = msg_counter;
		}
	}
}
