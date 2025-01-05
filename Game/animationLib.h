#ifndef __ANIM_H 
#define __ANIM_H

#include "LPC17xx.h"
#define N 8
#define M 13
#define MAP_N 10
#define MAP_C 21
#define MAP_R 24

extern uint8_t eat_anim[4][N][N];
extern uint8_t map[MAP_R][MAP_C];
extern uint8_t life_sym[M][M];

#endif