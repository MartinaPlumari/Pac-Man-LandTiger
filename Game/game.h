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
	uint16_t eaten_pills;
	uint8_t lives;
}game_state_t;

extern volatile game_state_t game_state;

void game_init();
void game_update();
void game_render();
void game_pause();
void game_resume();
void game_over();
void game_victory();
void game_display_life(int lives);
void game_clear_life(int lives);

void pacman_init(uint16_t posX, uint16_t posY, uint16_t speed);
void pacman_display(uint16_t Xpos, uint16_t Ypos);
void pacman_clear(uint16_t Xpos, uint16_t Ypos);
void pacman_change_dir(uint8_t direction);
void pacman_pos_update(uint8_t r, uint8_t c);

void ghost_init(uint16_t posX, uint16_t posY, uint16_t speed);
void ghost_display(uint16_t Xpos, uint16_t Ypos);
void ghost_clear(uint16_t Xpos, uint16_t Ypos);

void counter_update();
void score_update(uint8_t PowerPill);

uint16_t pill_getIndex(uint8_t r, uint8_t c);
void pill_create(uint8_t r, uint8_t c);
void ppill_generate();
void ppills_generate_t();

void map2pixels (uint8_t r, uint8_t c, uint16_t *posX, uint16_t *posY);
void pixels2map (uint16_t posX, uint16_t posY, uint8_t *r, uint8_t *c);
void map_init();

void CAN_send_status(uint16_t counter, uint8_t lives, uint16_t score);

#endif