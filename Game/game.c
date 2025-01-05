#include "game.h"
#include <stdio.h>
#include "../GLCD/GLCD.h"
#include "animationLib.h"
#include "../timer/timer.h"


#define START_X 100
#define START_Y 100
#define SPEED 5
#define MAX_TIME 60

#define PILL_DIM 3
#define PPILL_DIM 5

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

typedef struct{
	uint16_t posX;
	uint16_t posY;
	uint8_t PowerPill;
	uint8_t isEaten;
}pill_t;

volatile entity_state_t pacman;
volatile game_state_t game_state;
//valutare se mettere le pill dentro una struttura mappa. Di sicuro vanno inizializzate con la mappa
volatile pill_t pills[240];

void pacman_init(uint16_t posX, uint16_t posY, uint16_t speed){
	
		pacman.posX = posX;
		pacman.posY = posY;
		pacman.speed = speed;
		pacman.dir = G_LEFT;
	  pacman.anim_frame = 0;
	
		pacman_display(pacman.posX, pacman.posY);
	
}

void game_init(){

		LCD_Clear(Black);
		//pacman_init(START_X, START_Y, SPEED);
	  
		/*set and print counter*/
		game_state.counter = MAX_TIME;
	  GUI_Text(0,0,(uint8_t *) "  TIME  ", White, Black);
		print_number(game_state.counter, 0, 12, White, Black);

		/*set and print score*/
		game_state.score = 0;
	  GUI_Text(MAX_X-80, 0, (uint8_t *) "  SCORE  ", White, Black); 
	  print_number(game_state.score, MAX_X-80, 12, White, Black);
	
		/*set and print lives*/
		game_state.lives = 0;
		game_gain_life();
	  
	
	//provvisorio: sostituire con init mappa
		/*pills[0].posX = 200;
		pills[0].posY = 200;
	  pills[0].PowerPill = 0;
		pills[0].isEaten = 0;
	  print_circle(PILL_DIM, pills[0].posX, pills[0].posY, White); */
		
		map_init();
		
	  game_pause();
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
	
	//fare questo check solo per intervalli più grossi di un pixel !!!
	if(pacman.posX == pills[0].posX && pacman.posY == pills[0].posY && pills[0].isEaten == 0)
			score_update(pills[0].PowerPill);
	
}

void game_pause(){
	
	game_state.curr_state = PAUSE;
	
	//rendere questo più carino e generalizzare in una funzione
	GUI_Text(MAX_X/2-35, MAX_Y/2, (uint8_t *) "         ", Black, Black);
	GUI_Text(MAX_X/2-35, MAX_Y/2, (uint8_t *) "  PAUSE  ", Black, White);
	
	//block timer
	disable_timer(0);
}

void game_resume(){
	
	game_state.curr_state = PLAY;
	
	GUI_Text(MAX_X/2-35, MAX_Y/2, (uint8_t *) "         ", Black, Black);
	
	//resume timer
	enable_timer(0);
}

void game_over(){
	
	game_state.curr_state = GAME_OVER;
	disable_timer(0);
	
	LCD_Clear(Red);
	
	//rendere questo più carino e generalizzare in una funzione
	GUI_Text(MAX_X/2-55, MAX_Y/2, (uint8_t *) "  GAME OVER  ", Black, White);
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
						LCD_SetPoint(X+j, Y+i, Black);
					}
		}
	}
	
}

void counter_update(){
	    
				game_state.counter --;
	
				print_number(game_state.counter, 0, 12, White, Black);
					
				if(game_state.counter == 0)
					game_over();
}

void score_update(uint8_t PowerPill){
	
			 if(PowerPill == 1)
					game_state.score += 50;
			 game_state.score += 10;
	
			 print_number(game_state.score, MAX_X-80, 12, White, Black);
			 
			 if(game_state.score >= game_state.lives*1000){
				 game_gain_life();
			 }
}

void print_number(uint16_t value, uint16_t posX, uint16_t posY, uint16_t textColor, uint16_t bgColor){
				char space[2] = "  ";
				char str_value[6] = "     ";
	
				sprintf(str_value, "%s%d", "  ", value);

				GUI_Text(posX, posY, (uint8_t *) "     ", bgColor, bgColor);
				GUI_Text(posX, posY, (uint8_t *) str_value, textColor, bgColor);
}

void print_circle(uint8_t radius, uint16_t posX, uint16_t posY, uint16_t color){
				int i, j;
				
				//y
				for(i=0; i<radius; i++){
					//x
					for(j=0;j<radius; j++){
						//finds the first pixel of the circle in the line and prints it. Using simmetry we can print two lines.
						if(((radius-j)*(radius-j) + (radius-i)*(radius-i)) <= radius*radius+1){
								LCD_DrawLine(posX-radius+j+1, posY-radius+i+1, posX+radius-j-1, posY-radius+i+1, color);
								LCD_DrawLine(posX-radius+j+1, posY+radius-i-1, posX+radius-j-1, posY+radius-i-1, color);
								continue;
						}
					}
				}
	
}


void print_tile(uint16_t r, uint16_t c, uint16_t x_margin, uint16_t y_margin){
	int i;
	
	for(i=0;i<MAP_N;i++){
		LCD_DrawLine(x_margin+c*MAP_N, y_margin+r*MAP_N+i, x_margin+c*MAP_N+MAP_N, y_margin+r*MAP_N+i, Blue);
	}
}

void game_gain_life(){
	int i, j;
	int X, Y;
	
	X = 15 + (game_state.lives)*15;
	Y = MAX_Y - 20;
	
	for(i=0; i<M; i++){
			for(j=0;j<M;j++){
					if(life_sym[i][j]){
						LCD_SetPoint(X+j, Y+i, Yellow);
					}
				}
	}
	game_state.lives++;
}

void game_lose_life(){
	int i, j;
	int X, Y;
	
	X = 15 + (game_state.lives-1)*15;
	Y = MAX_Y - 20;
	
	for(i=0; i<M; i++){
			for(j=0;j<M;j++){
					if(life_sym[i][j]){
						LCD_SetPoint(X+j, Y+i, Black);
					}
				}
	}
		game_state.lives--;
}

void map_init(){
	int i, j;
	int x_margin = (MAX_X-MAP_C*MAP_N)/2;
	int y_margin = (MAX_Y-MAP_R*MAP_N)/2;
	
	for(i=0; i<MAP_R; i++){
		for(j=0; j<MAP_C; j++){
			if(map[i][j] == 0){
				//print wall tile
				print_tile(i, j, x_margin, y_margin);
			}else if(map[i][j] == 1){
				//print pill
				print_circle(2, x_margin+j*MAP_N+(MAP_N/2), y_margin +i*MAP_N+(MAP_N/2), White);
			}else if(map[i][j] == 2){
				//print pacman
				pacman_init(x_margin+j*MAP_N+(MAP_N/2), y_margin+i*MAP_N+(MAP_N/2), SPEED);
			}
		}
	}
}