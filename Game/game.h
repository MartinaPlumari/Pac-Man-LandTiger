#ifndef __GAME_H 
#define __GAME_H

#include "LPC17xx.h"

enum directions{
	G_UP,
	G_DOWN,
	G_LEFT,
	G_RIGHT
};

//valutare se metterlo nel .c
enum gameState{
	PAUSE,
	GAME_OVER,
	PLAY,
	VICTORY
};

typedef struct{
	uint16_t counter;
	uint8_t curr_state;
	uint16_t score;
	uint8_t lives;
}game_state_t;

extern volatile game_state_t game_state;

void game_init();
void game_update();
void game_render();
void game_pause();
void game_resume();
void game_over();
void game_gain_life();
void game_lose_life();

void pacman_init(uint16_t posX, uint16_t posY, uint16_t speed);
void pacman_display(uint16_t Xpos, uint16_t Ypos);
void pacman_clear(uint16_t Xpos, uint16_t Ypos);
void pacman_change_dir(uint8_t direction);

void counter_update();
void score_update(uint8_t PowerPill);

void print_number(uint16_t value, uint16_t posX, uint16_t posY, uint16_t textColor, uint16_t bgColor);
void print_circle(uint8_t radius, uint16_t posX, uint16_t posY, uint16_t color);


#endif