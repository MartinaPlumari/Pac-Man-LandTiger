#ifndef __GAME_H 
#define __GAME_H

#include "LPC17xx.h"

enum Directions{
	G_UP,
	G_DOWN,
	G_LEFT,
	G_RIGHT
};

void game_init();
void game_update();
void game_render();

void pacman_init(uint16_t posX, uint16_t posY, uint16_t speed);
void pacman_display(uint16_t Xpos, uint16_t Ypos);
void pacman_clear(uint16_t Xpos, uint16_t Ypos);
void pacman_change_dir(uint8_t direction);


#endif