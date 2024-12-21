#include "game.h"
#include "../GLCD/GLCD.h"

#define N 13

typedef struct{
	uint16_t posX;
	uint16_t posY;
	uint8_t speed;
	uint8_t dir;
}state_t;

state_t pacman;

//farne una versione in cui si renderizza la linea intera
    int circle[N][N] = {
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
        {0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
        {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}
    };

void pacman_init(uint16_t posX, uint16_t posY, uint16_t speed){
	
		pacman.posX = posX;
		pacman.posY = posY;
		pacman.speed = speed;
		pacman.dir = G_LEFT;
	
		pacman_display(pacman.posX, pacman.posY);
	
}

void game_init(){
		LCD_Clear(Blue);
		pacman_init(100, 100, 5);
}

void game_update(){
	
	//aggiungere caso per muro -> non serve rirenderizzare pacman
	
	pacman_clear(pacman.posX, pacman.posY);
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
	
	//falso perchè pacman si teletrasporta -> usare una versione modificata di questo codice per fare il teletrasporto
	/*if(X<0 || Y<0 || X+N>MAX_X || Y+N>MAX_Y){
		//valutare se rirenderizzare nella posizione precedente -> sarebbe meglio NON cancellare quello di prima
		return;
	}*/
	
	for(i=0; i<N; i++){
		for(j=0;j<N;j++){
			if(circle[i][j]){
				LCD_SetPoint(X+i, Y+j, Yellow);
			}
		}
	}
}

void pacman_clear(uint16_t Xpos, uint16_t Ypos){
	int i, j;
	int X, Y;
	
	X = Xpos - N/2;
	Y = Ypos - N/2;
	
	for(i=0; i<N; i++){
		for(j=0;j<N;j++){
			if(circle[i][j]){
				LCD_SetPoint(X+i, Y+j, Blue);
			}
		}
	}
}