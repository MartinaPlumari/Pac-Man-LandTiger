#include "music.h"

#include "../timer/timer.h"

NOTE theme[] = 
{
	// 1
	{c2, time_croma},
	{c3, time_croma},
	{g2, time_croma},
	{e2, time_croma},
	{c3, time_croma},
	{g2, time_croma},
	{e2, time_semiminima},
	//{pause, time_croma},
	//2
	{d2b, time_croma},
	{d3b, time_croma},
	{a2b, time_croma},
	{f2, time_croma},
	{d3b, time_croma},
	{a2b, time_croma},
	{f2, time_semiminima},
	//{pause, time_croma},
	//3
	{c2, time_croma},
	{c3, time_croma},
	{g2, time_croma},
	{e2, time_croma},
	{c3, time_croma},
	{g2, time_croma},
	{e2, time_croma},
	{pause, time_croma},
	// 4
	{e2, time_semicroma},
	{f2, time_semicroma},
	{g2, time_semicroma},
	{pause, time_semicroma},
	{g2, time_semicroma},
	{a2b, time_semicroma},
	{a2, time_semicroma},
	{pause, time_semicroma},
	//5
	{a2, time_semicroma},
	{b2b, time_semicroma},
	{b2, time_semicroma},
	{pause, time_semicroma},
	{c3, time_croma},
	{pause, time_croma}
	
};

void playNote(NOTE note)
{
	if(note.freq != pause)
	{
		reset_timer(1);
		init_timer(1, 0, 0, 3, note.freq);
		enable_timer(1);
	}
	reset_timer(2);
	init_timer(2, 0, 0, 7, note.duration);
	enable_timer(2);
}

BOOL isNotePlaying(void)
{
	return ((LPC_TIM1->TCR != 0) || (LPC_TIM2->TCR != 0));
}
