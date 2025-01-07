#include "game.h"
#include <stdio.h>
#include "../GLCD/GLCD.h"
#include "animationLib.h"
#include "../timer/timer.h"


#define START_X 100
#define START_Y 100
#define SPEED 5
#define MAX_TIME 53

#define PILL_DIM 2
#define PPILL_DIM 4
#define PILL_N 240
#define PPILL_N 6

/* GAME ENTITY TYPE                                         */
/*----------------------------------------------------------*/
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

/* PILL TYPE */
typedef struct{
	uint16_t r;
	uint16_t c;
	uint8_t PowerPill;
	uint8_t isEaten;
}pill_t;

/* PILLS WRAPPER STRUCTURE */
typedef struct{
	pill_t v_pills[PILL_N];
	uint8_t n_pills;
	uint8_t powerpills_t[PPILL_N];
	uint8_t pp_i;
}pills_t;

const x_margin = (MAX_X-MAP_C*MAP_N)/2;
const y_margin = (MAX_Y-MAP_R*MAP_N)/2;

volatile entity_state_t pacman;
volatile game_state_t game_state;

volatile pills_t pills;

uint16_t get_rand_in_range(int min, int max){
	
	int seed;
	uint16_t rand_n = 0;
	
	seed = (LPC_TIM1->TC)*65;
	seed &= 0xFFF;
	rand_n = (seed%(max-min-1))+min;
	
	return rand_n;
}

void pacman_init(uint16_t posX, uint16_t posY, uint16_t speed){
	
		pacman.posX = posX;
		pacman.posY = posY;
		pacman.speed = speed;
		pacman.dir = G_LEFT;
	  pacman.anim_frame = 0;
	
		pacman_display(pacman.posX, pacman.posY);
	
}

void game_init(){
	  int i;

		LCD_Clear(Black);
	  
		/*set and print counter*/
		game_state.counter = MAX_TIME;
	  GUI_Text(0,5,(uint8_t *) "  TIME  ", White, Black);
		print_number(game_state.counter, 0, 17, White, Black);

		/*set and print score*/
		game_state.score = 0;
	  GUI_Text(MAX_X-80, 5, (uint8_t *) "  SCORE  ", White, Black); 
	  print_number(game_state.score, MAX_X-80, 17, White, Black);
	
		/*set and print lives*/
		game_state.lives = 0;
		game_gain_life();
		
	  /*initialize map*/
		map_init();
	  game_state.eaten_pills = 0;
	
		/*extract random times in which power pills spawn*/
		ppills_generate_t();
		pills.pp_i = 0;
		
		game_pause();
}

void game_update(){
	
		static uint8_t c, r;
	  uint16_t i;
	  int pos;
	
		pixels2map(pacman.posX, pacman.posY, &r, &c);
		
		//check pillola mangiata
		//fare questo check solo quando ci si trova al centro di una cella - > !capire come fare!
		if(map[r][c] == 1){
			i = pill_getIndex(r,c);
			map[r][c] = 3;
			pills.v_pills[i].isEaten = 1;
			game_state.eaten_pills++;
			score_update(0);
			if(game_state.eaten_pills == PILL_N){
				game_victory();
				return;
			}
			
		}
	
		/*PACMAN RENDERING*/
		pacman_clear(pacman.posX, pacman.posY);
		
		//position update
		switch(pacman.dir){
				case G_UP:
					if(map[r-1][c]!=0)
						pacman.posY -= pacman.speed;
					break;
				case G_DOWN:
					if(map[r+1][c]!=0)
						pacman.posY += pacman.speed;
					break;
				case G_LEFT:
					if(c-1<0 || map[r][c-1]!=0)
						pacman.posX -= pacman.speed;
					break;
				case G_RIGHT:
					if(c+1>MAP_C-1 || map[r][c+1]!=0)
						pacman.posX += pacman.speed;
					break;
				default:
					break;
		}
		
		//check if we have to teleport (only horizontal since we don't have it vertically)
		if(pacman.posX-x_margin<=0){
			pacman.posX = MAX_X-x_margin-MAP_N/2;
			c = MAP_C-1;
		}else if(pacman.posX+x_margin>=MAX_X+MAP_N/2){
			pacman.posX = x_margin+MAP_N/2;
			c = 0;
		}
		
		/* check if we have to generate a pp and generate it*/
		if(pills.pp_i < PPILL_N && game_state.counter == pills.powerpills_t[pills.pp_i]){
			ppill_generate();
		}
		
		pacman_display(pacman.posX, pacman.posY);

	
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
	int c, r, i, j;
	
	c = (MAX_X/2-35 - x_margin)/MAP_N;
	r = (MAX_Y/2 - y_margin)/MAP_N;
	
	//SPOSTARE IN UNA FUNZIONE che prende x, y, margin_x e margin_y
	GUI_Text(MAX_X/2-35, MAX_Y/2, (uint8_t *) "         ", Black, Black);
	
	for(i=r-1; i<r+2; i++){
		for(j=c-1; j<c+9; j++){
			if(map[i][j] == 0)
				print_tile(i,j,x_margin, y_margin);
			else if(map[i][j] == 1)
				print_circle(PILL_DIM, x_margin+j*MAP_N+(MAP_N/2), y_margin +i*MAP_N+(MAP_N/2), Pink);
		}
	}
	
	game_state.curr_state = PLAY;
	
	//resume timer
	enable_timer(0);
}

void game_over(){
	
	game_state.curr_state = GAME_OVER;
	disable_timer(0);
	
	LCD_Clear(Red);
	
	//rendere questo più carino e generalizzare in una funzione
	GUI_Text(MAX_X/2-55, MAX_Y/2, (uint8_t *) "  GAME OVER!  ", Black, White);
}

void game_victory(){
	
	game_state.curr_state = VICTORY;
	disable_timer(0);
	
	LCD_Clear(Green);
	
	//rendere questo più carino e generalizzare in una funzione
	GUI_Text(MAX_X/2-45, MAX_Y/2, (uint8_t *) "  VICTORY!  ", Black, White);
}

void pacman_change_dir(uint8_t direction){
	if((pacman.posX-x_margin-MAP_N/2) % MAP_N == 0 && (pacman.posY-y_margin-MAP_N/2) % MAP_N == 0)
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
	
				print_number(game_state.counter, 0, 17, White, Black);
					
				if(game_state.counter == 0)
					game_over();
}

void score_update(uint8_t PowerPill){
	
			 if(PowerPill == 1)
					game_state.score += 50;
			 game_state.score += 10;
	
			 print_number(game_state.score, MAX_X-80, 17, White, Black);
			 
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

/*returns the screen pixel position of the map tile*/
void map2pixels (uint8_t r, uint8_t c, uint16_t *posX, uint16_t *posY){
	*posX =  x_margin+c*MAP_N+(MAP_N/2);
	*posY =  y_margin+r*MAP_N+(MAP_N/2);
}

/*returns the center of the map tile in screen pixel position*/
void pixels2map (uint16_t posX, uint16_t posY, uint8_t *r, uint8_t *c){
	if((posX-x_margin-MAP_N/2) % MAP_N == 0)
		*c = (posX - x_margin - MAP_N/2)/MAP_N;
	if((posY-y_margin-MAP_N/2) % MAP_N== 0)
		*r = (posY - y_margin - MAP_N/2)/MAP_N;
}

void print_tile(uint16_t r, uint16_t c, uint16_t x_margin, uint16_t y_margin){
	int i;
	uint16_t x, y;
	
	map2pixels(r,c,&x,&y);
	
	/*upper line*/
	if(r-1<0 || map[r-1][c]!=0){
		LCD_DrawLine(x-MAP_N/2, y-MAP_N/2, x+MAP_N/2, y-MAP_N/2, Blue);
		LCD_DrawLine(x-MAP_N/2, y-MAP_N/2-1, x+MAP_N/2, y-MAP_N/2-1, Blue);
	}
	/*bottom line*/
	if(r+1>=MAP_R || map[r+1][c]!=0){
		LCD_DrawLine(x-MAP_N/2, y+MAP_N/2, x+MAP_N/2, y+MAP_N/2, Blue);
		LCD_DrawLine(x-MAP_N/2, y+MAP_N/2-1, x+MAP_N/2, y+MAP_N/2-1, Blue);
	}
	/*left line*/
	if(c-1<0 || map[r][c-1]!=0){
		LCD_DrawLine(x-MAP_N/2, y-MAP_N/2, x-MAP_N/2, y+MAP_N/2, Blue);
		LCD_DrawLine(x-MAP_N/2-1, y-MAP_N/2, x-MAP_N/2-1, y+MAP_N/2, Blue);
	}
	/*right line*/
	if(c+1>=MAP_C || map[r][c+1]!=0){
		LCD_DrawLine(x+MAP_N/2, y-MAP_N/2, x+MAP_N/2, y+MAP_N/2, Blue);
		LCD_DrawLine(x+MAP_N/2-1, y-MAP_N/2, x+MAP_N/2-1, y+MAP_N/2, Blue);
	}
		
	
	/*for(i=0;i<MAP_N;i++){
		LCD_DrawLine(x-MAP_N/2, y-MAP_N/2+i, x+MAP_N/2, y-MAP_N/2+i, Blue);
	}*/
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

uint16_t pill_getIndex(uint8_t r, uint8_t c){
	uint16_t i;
	
	for(i=0;i<PILL_N;i++){
		if(pills.v_pills[i].r == r && pills.v_pills[i].c == c){
			break;
		}
	}
	
	return i;
}

void pill_create(uint8_t r, uint8_t c){
	uint16_t x, y;
	
	map2pixels(r, c, &x, &y);

	pills.v_pills[pills.n_pills].c = c;
	pills.v_pills[pills.n_pills].r = r;
	pills.v_pills[pills.n_pills].isEaten = 0;
	pills.v_pills[pills.n_pills].PowerPill = 0;
	pills.n_pills++;
	
	print_circle(PILL_DIM, x, y, Pink);
	
}

void ppills_generate_t(){
	int min, max, i, t;
	
	min = 51;
	max = 60;
	
	for(i=0; i<PPILL_N; i++){
		pills.powerpills_t[i] = get_rand_in_range(min, max);
		min -= 10;
		max -= 10;
	}
	
}

void ppill_generate(){
	uint16_t posX, posY, pos;
	
	pos = get_rand_in_range(0,PILL_N);
	while(pills.v_pills[pos].isEaten == 1){
		pos = get_rand_in_range(0, PILL_N);
	}
		
	pills.v_pills[pos].PowerPill = 1;
	map2pixels(pills.v_pills[pos].r, pills.v_pills[pos].c, &posX, &posY);
	print_circle(PPILL_DIM, posX, posY, Pink);
	pills.pp_i++;
	
}

void map_init(){
	int i, j;
	
	for(i=0; i<MAP_R; i++){
		for(j=0; j<MAP_C; j++){
			if(map[i][j] == 0){
				//print wall tile
				print_tile(i, j, x_margin, y_margin);
			}else if(map[i][j] == 1){
				//print pill
				//print_circle(PILL_DIM, x_margin+j*MAP_N+(MAP_N/2), y_margin +i*MAP_N+(MAP_N/2), White);
				pill_create(i,j);
			}else if(map[i][j] == 2){
				//print pacman
				pacman_init(x_margin+j*MAP_N+(MAP_N/2), y_margin+i*MAP_N+(MAP_N/2), SPEED);
			}
		}
	}
}