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

extern volatile uint8_t game_state;

void game_init();
void game_update();
void game_render();
void game_pause();
void game_resume();

void pacman_init(uint16_t posX, uint16_t posY, uint16_t speed);
void pacman_display(uint16_t Xpos, uint16_t Ypos);
void pacman_clear(uint16_t Xpos, uint16_t Ypos);
void pacman_change_dir(uint8_t direction);


#endif