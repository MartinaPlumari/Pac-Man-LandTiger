#include "music.h"
#include "../timer/timer.h"

#define N 4

typedef struct{
	int curr;
	int len;
	int priority;
	NOTE *sfx;
}sfx_t;

//considerare di spostare in un file
NOTE theme[] = 
{
	// 1
	{c4, time_semicroma},
	{c5, time_semicroma},
	{g4, time_semicroma},
	{e4, time_semicroma},
	{c5, time_semicroma},
	{g4, time_semicroma},
	{e4, time_semicroma},
	{pause, time_semicroma},
	//2
	{d4b, time_semicroma},
	{d5b, time_semicroma},
	{a4b, time_semicroma},
	{f4, time_semicroma},
	{d5b, time_semicroma},
	{a4b, time_semicroma},
	{f4, time_semicroma},
	{pause, time_semicroma},
	//3
	{c4, time_semicroma},
	{c5, time_semicroma},
	{g4, time_semicroma},
	{e4, time_semicroma},
	{c5, time_semicroma},
	{g4, time_semicroma},
	{e4, time_semicroma},
	{pause, time_semicroma},
	// 4
	{e4, time_biscroma},
	{f4, time_biscroma},
	{g4, time_biscroma},
	{pause, time_biscroma},
	{g4, time_biscroma},
	{a4b, time_biscroma},
	{a4, time_biscroma},
	{pause, time_biscroma},
	{a4, time_biscroma},
	{b4b, time_biscroma},
	{b4, time_biscroma},
	{pause, time_biscroma},
	{c5, time_semicroma},
	{pause, time_semicroma}
	
};

NOTE sm_life[] = {
	{e5, time_semicroma},
	{g5, time_semicroma},
	{e6, time_semicroma},
	{c6, time_semicroma},
	{d6, time_semicroma},
	{g6, time_semicroma},
};

NOTE chomp[] = {
	{c4b, time_semibiscroma},
	{c4, time_semibiscroma},
	{pause, time_biscroma}
};

NOTE loose[] = {
  {e5, time_semibiscroma},
  {g5b, time_semibiscroma},
  {e5, time_semibiscroma},
  {e5b, time_semibiscroma},
  {e5, time_semibiscroma},
  {e5b, time_semibiscroma},
  {d5, time_semibiscroma},
  {e5b, time_semibiscroma},
  {d5, time_semibiscroma},
  {d5b, time_semibiscroma},
  {d5, time_semibiscroma},
  {d5b, time_semibiscroma},
  {c5, time_semibiscroma},
  {b4, time_semibiscroma},
  {b4b, time_semibiscroma},
  {f4b, time_semibiscroma},
  {g4b, time_semibiscroma},
  {g4b, time_semibiscroma},
};

volatile SOUND_CODE current_sfx;
volatile BOOL silence;

sfx_t sfx[N];

void sfx_init(){
	int i;
	
	silence = FALSE;

	sfx[THEME].len = 38;
	sfx[THEME].curr = 0;
	sfx[THEME].priority = 2;
	sfx[THEME].sfx = theme;
	
	sfx[CHOMP].len = 3;
	sfx[CHOMP].curr = 0;
	sfx[CHOMP].priority = 1;
	sfx[CHOMP].sfx = chomp;
	
	sfx[NEW_LIFE].len = 6;
	sfx[NEW_LIFE].curr = 0;
	sfx[NEW_LIFE].priority = 3;
	sfx[NEW_LIFE].sfx = sm_life;
	
	sfx[LOSE].len = 18;
	sfx[LOSE].curr = 0;
	sfx[LOSE].priority = 4;
	sfx[LOSE].sfx = loose;
	
	current_sfx = THEME;
}

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

void playSound(){
	
	if(silence)
		return;
	
	playNote(sfx[current_sfx].sfx[sfx[current_sfx].curr++]);
	if(sfx[current_sfx].curr >= sfx[current_sfx].len)
		silence = TRUE;
	
	
	/*if(sfx[current_sfx].curr < sfx[current_sfx].len){
		playNote(sfx[current_sfx].sfx[sfx[current_sfx].curr++]);
		previous = current_sfx;
	}else{
		silence = TRUE;
	}*/
	
}

void changeSound(SOUND_CODE code){
	//if(current_sfx != SILENCE || current_sfx == code)
	if(silence || sfx[current_sfx].priority <= sfx[code].priority){
		sfx[current_sfx].curr = 0;
		current_sfx = code;
		silence = FALSE;
	}
}
