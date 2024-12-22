#include "game.h"
#include "../GLCD/GLCD.h"
#include "animationLib.h"

typedef struct{
	uint16_t posX;
	uint16_t posY;
	uint8_t speed;
	uint8_t dir;
	uint8_t anim_frame;
}entity_state_t;

volatile entity_state_t pacman;


void pacman_init(uint16_t posX, uint16_t posY, uint16_t speed){
	
		pacman.posX = posX;
		pacman.posY = posY;
		pacman.speed = speed;
		pacman.dir = G_LEFT;
	  pacman.anim_frame = 0;
	
		pacman_display(pacman.posX, pacman.posY);
	
}

void game_init(){
		LCD_Clear(Blue);
		pacman_init(100, 100, 5);
}

void game_update(){
	
	//aggiungere caso per muro -> non serve rirenderizzare pacman
	
	//render pacman
	pacman_clear(pacman.posX, pacman.posY);
	
	if(pacman.posX<=0)
		pacman.posX = MAX_X;
	if(pacman.posX>MAX_X)
		pacman.posX = 0;
	
	switch(pacman.dir){
		case G_UP:
		  pacman.posY -= pacman.speed;
			break;
		case G_DOWN:
		  pacman.posY += pacman.speed;
			break;
		case G_LEFT:
			pacman.posX -= pacman.speed;
			break;
		case G_RIGHT:
			pacman.posX += pacman.speed;
			break;
		default:
			break;
	}
		
	pacman_display(pacman.posX, pacman.posY);
	
}

void pacman_change_dir(uint8_t direction){
	pacman.dir = direction;
}

void pacman_display(uint16_t Xpos, uint16_t Ypos){
	int i, j;
	int X, Y;
	
	X = Xpos - N/2;
	Y = Ypos - N/2;

	
	switch(pacman.dir){
		case(G_DOWN):
			for(i=0; i<N; i++){
				for(j=0;j<N;j++){
					if(eat_anim[pacman.anim_frame][j][i]){
						LCD_SetPoint(X+j, Y+i, Yellow);
					}
				}
			}
			break;
		case(G_UP):
			for(i=0; i<N; i++){
				for(j=0;j<N;j++){
					if(eat_anim[pacman.anim_frame][j][N-i-1]){
						LCD_SetPoint(X+j, Y+i, Yellow);
					}
				}
			}
			break;
		case(G_LEFT):
			for(i=0; i<N; i++){
				for(j=0;j<N;j++){
					if(eat_anim[pacman.anim_frame][i][N-j-1]){
						LCD_SetPoint(X+j, Y+i, Yellow);
					}
				}
			}
			break;
		case(G_RIGHT):
			for(i=0; i<N; i++){
				for(j=0;j<N;j++){
					if(eat_anim[pacman.anim_frame][i][j]){
						LCD_SetPoint(X+j, Y+i, Yellow);
					}
				}
			}
			break;
	}
	
	pacman.anim_frame ++;
	if(pacman.anim_frame >= 4)
		pacman.anim_frame = 0;
}

void pacman_clear(uint16_t Xpos, uint16_t Ypos){
	int i, j;
	int X, Y;
	
	X = Xpos - N/2;
	Y = Ypos - N/2;
	
	for(i=0; i<N; i++){
		for(j=0;j<N;j++){
			LCD_SetPoint(X+j, Y+i, Blue);
		}
	}
	
/*		switch(pacman.dir){
		case(G_DOWN):
			for(i=0; i<N; i++){
				for(j=0;j<N;j++){
					if(eat_anim[pacman.anim_frame][i][j]){
						LCD_SetPoint(X+j, Y+i, Blue);
					}
				}
			}
			break;
		case(G_UP):
			for(i=0; i<N; i++){
				for(j=0;j<N;j++){
					if(eat_anim[pacman.anim_frame][i][N-j]){
						LCD_SetPoint(X+j, Y+i, Blue);
					}
				}
			}
			break;
		case(G_LEFT):
			break;
		case(G_RIGHT):
			break;
	}*/
	
	
}