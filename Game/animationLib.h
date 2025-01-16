#ifndef __ANIM_H 
#define __ANIM_H

#include "LPC17xx.h"
#define N 8
#define M 13
#define MAP_N 10
#define MAP_C 21
#define MAP_R 24

extern uint8_t eat_anim[4][N][N];
extern uint8_t ghost_anim [3][N][N];
extern uint8_t map[MAP_R][MAP_C];
extern uint8_t life_sym[M][M];

void print_number(uint16_t value, uint16_t posX, uint16_t posY, uint16_t textColor, uint16_t bgColor);
void print_circle(uint8_t radius, uint16_t posX, uint16_t posY, uint16_t color);
void print_tile(uint16_t r, uint16_t c, uint16_t x_margin, uint16_t y_margin);
void print_gate(uint16_t r, uint16_t c, uint16_t x_margin, uint16_t y_margin);

#endif