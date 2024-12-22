#include "game.h"
#include "../GLCD/GLCD.h"
#include "animationLib.h"

#define START_X 100
#define START_Y 100
#define SPEED 5

/*ENTITY TYPE*/
/* - posX and posY: current position												*/
/* - speed: current speed																		*/
/* - dir: current direction 															  */
/* - anim_frame: index of the next animation frame to render*/
typedef struct{
	uint16_t posX;
	uint16_t posY;
	uint8_t speed;
	uint8_t dir;
	uint8_t anim_frame;
}entity_state_t;

volatile entity_state_t pacman;
volatile uint8_t game_state;

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
		pacman_init(START_X, START_Y, SPEED);
	
	  game_state = PLAY;
}

void game_update(){
	
	//aggiungere caso per muro -> non serve rirenderizzare pacman
	
	/*PACMAN RENDERING*/
	pacman_clear(pacman.posX, pacman.posY);
	
	//check if we have to teleport (only horizontal since we don't have it vertically)
	if(pacman.posX<=0)
		pacman.posX = MAX_X;
	if(pacman.posX>MAX_X)
		pacman.posX = 0;
	
	//position update
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

void game_pause(){
	
	game_state = PAUSE;
	
	GUI_Text(MAX_X/2-40, MAX_Y/2, (uint8_t *) "         ", Blue, Blue);
	GUI_Text(MAX_X/2-40, MAX_Y/2, (uint8_t *) "  PAUSE  ", Black, White);
	
	//bloccare timer
}

void game_resume(){
	
	game_state = PLAY;
	
	GUI_Text(MAX_X/2-40, MAX_Y/2, (uint8_t *) "         ", Blue, Blue);
	
	//far riprendere il timer
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
			if(eat_anim[0][i][j]){
						LCD_SetPoint(X+j, Y+i, Blue);
					}
		}
	}
	
}