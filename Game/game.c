#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include "../GLCD/GLCD.h"
#include "animationLib.h"
#include "../music/music.h"
#include "../timer/timer.h"
#include "../CAN/CAN.h"

/*change speed to 5 if using custom scaling value, leave it to 2 for default scaling or board*/
#define P_SPEED 2
#define MAX_TIME 60

#define G_SPEED 1

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

/* PILL TYPE																								 */
/*-----------------------------------------------------------*/
/* - r, c: pill coordinates in the game map matrix           */
/* - PowerPill: bool that indicates if it is a powerpill     */
/* - isEaten: bool that indicates if the pill has been eaten */

typedef struct{
	uint16_t r;
	uint16_t c;
	uint8_t PowerPill;
	uint8_t isEaten;
}pill_t;

/* PILLS WRAPPER STRUCTURE                                   */
/*-----------------------------------------------------------*/
/* - v_pills: vector containing all the pills                */
/* - n_pills: pills index (used for filling the vector)      */
/* - powerpills_t: stores spawn times for the power pills    */
/* - pp_i: power pills current index													*/

typedef struct{
	pill_t v_pills[PILL_N];
	uint8_t n_pills;
	uint8_t powerpills_t[PPILL_N];
	uint8_t pp_i;
}pills_t;

const x_margin = (MAX_X-MAP_C*MAP_N)/2;
const y_margin = (MAX_Y-MAP_R*MAP_N)/2;
const int moves[4][3] = {
        {-1, 0, G_UP}, // Up
        {1, 0, G_DOWN},  // Down
        {0, -1, G_LEFT}, // Left
        {0, 1, G_RIGHT}   // Right
    };

volatile entity_state_t pacman;
volatile entity_state_t ghost;
volatile game_state_t game_state;
volatile pills_t pills;

uint16_t get_rand_in_range(int min, int max);


void game_init(){
	  int i;

		/*set initial sound*/
		sfx_init();
	  
		/*set and print counter*/
		game_state.counter = MAX_TIME;
	  GUI_Text(0,5,(uint8_t *) "  TIME  ", White, Black);
		print_number(game_state.counter, 0, 17, White, Black);

		/*set and print score*/
		game_state.score = 0;
	  GUI_Text(MAX_X-80, 5, (uint8_t *) "  SCORE  ", White, Black); 
	  print_number(game_state.score, MAX_X-80, 17, White, Black);
	
		/*set and print lives*/
		game_state.lives = 1;
		
	  /*initialize map*/
		map_init();
	  game_state.eaten_pills = 0;
		
	
		/*extract random times in which power pills spawn*/
		ppills_generate_t();
		pills.pp_i = 0;
		
		game_pause();
}

void game_update(){
	
		static uint8_t c, r, g_c, g_r;
	  uint16_t i;
		uint8_t prev_g_dir;
	  int pos;
	
		pixels2map(pacman.posX, pacman.posY, &r, &c);
		pixels2map(ghost.posX, ghost.posY, &g_r, &g_c);
		
	
		/* PACMAN RENDERING */
		pacman_clear(pacman.posX, pacman.posY);	
		pacman_pos_update(r,c);
		pacman_display(pacman.posX, pacman.posY);
	
		/*GHOST RENDERING*/
		ghost_clear(ghost.posX, ghost.posY);
	  prev_g_dir = ghost.dir;
		ghost_pos_update(g_r, g_c, r, c);
		ghost_display(ghost.posX, ghost.posY, prev_g_dir);
		
		/* check if we have to generate a pp and generate it*/
		if(pills.pp_i < PPILL_N && game_state.counter == pills.powerpills_t[pills.pp_i]){
			ppill_generate();
		}
		
		
		//check for eaten pill
		if(map[r][c] == 1 || map[r][c] == 6){
			
			changeSound(CHOMP);
			i = pill_getIndex(r,c);
			map[r][c] = 3;
			pills.v_pills[i].isEaten = 1;
			game_state.eaten_pills++;
			score_update(pills.v_pills[i].PowerPill); //can update is here
			
			//victory check
			if(game_state.eaten_pills == PILL_N){
				game_victory();
				return;
			}
		}
}

void game_pause(){
	
	game_state.curr_state = PAUSE;
	
	GUI_Text(MAX_X/2-28, MAX_Y/2, (uint8_t *) "       ", Black, Black);
	GUI_Text(MAX_X/2-28, MAX_Y/2, (uint8_t *) " PAUSE ", Black, Blue);
	
	//suspend countdown
	disable_timer(0);
}

void game_resume(){
	int c, r, i, j;
	
	c = (MAX_X/2-35 - x_margin)/MAP_N;
	r = (MAX_Y/2 - y_margin)/MAP_N;
	
	//delete "pause" 
	GUI_Text(MAX_X/2-35, MAX_Y/2, (uint8_t *) "         ", Black, Black);
	
	//render underlying map area
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
	
	CAN_disable_IRQ();
	game_state.curr_state = GAME_OVER;
	changeSound(LOSE);
	disable_timer(0);
	
	LCD_Clear(Red);
	
	GUI_Text(MAX_X/2-55, MAX_Y/2, (uint8_t *) "  GAME OVER!  ", Black, Red);
}

void game_victory(){
	
	CAN_disable_IRQ();
	changeSound(THEME);
	game_state.curr_state = VICTORY;
	disable_timer(0);
	
	LCD_Clear(Green);

	GUI_Text(MAX_X/2-45, MAX_Y/2, (uint8_t *) "  VICTORY!  ", Black, Green);
}

void game_display_life(int lives){
	int i, j;
	int X, Y;
	
	X = 15 + (lives-1)*15;
	Y = MAX_Y - 20;
	
	//print life
	for(i=0; i<M; i++){
			for(j=0;j<M;j++){
					if(life_sym[i][j]){
						LCD_SetPoint(X+j, Y+i, Yellow);
					}
				}
	}
}

void game_clear_life(int lives){
	int i, j;
	int X, Y;
	
	X = 15 + (lives)*15;
	Y = MAX_Y - 20;
	
	//erase life
	for(i=0; i<M; i++){
			for(j=0;j<M;j++){
					if(life_sym[i][j]){
						LCD_SetPoint(X+j, Y+i, Black);
					}
				}
	}
}

void pacman_init(uint16_t posX, uint16_t posY, uint16_t speed){
	
		pacman.posX = posX;
		pacman.posY = posY;
		pacman.speed = speed;
		pacman.dir = G_LEFT;
	  pacman.anim_frame = 0;
	
		pacman_display(pacman.posX, pacman.posY);
	
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

void pacman_pos_update(uint8_t r, uint8_t c){
	
			//position update
		switch(pacman.dir){
				case G_UP:
					if(map[r-1][c]!=0 && map[r-1][c]!=4)
						pacman.posY -= pacman.speed;
					break;
				case G_DOWN:
					if(map[r+1][c]!=0 && map[r+1][c]!=4)
						pacman.posY += pacman.speed;
					break;
				case G_LEFT:
					if(c-1<0 || (map[r][c-1]!=0 && map[r][c-1]!=4))
						pacman.posX -= pacman.speed;
					break;
				case G_RIGHT:
					if(c+1>MAP_C-1 || (map[r][c+1]!=0 && map[r][c+1]!=4))
						pacman.posX += pacman.speed;
					break;
				default:
					break;
		}
		
		/*check if we have to teleport (only horizontal since we don't have it vertically)*/
		if(pacman.posX-x_margin<=0){
			pacman.posX = MAX_X-x_margin-MAP_N/2;
			c = MAP_C-1;
		}else if(pacman.posX+x_margin>=MAX_X+MAP_N/2){
			pacman.posX = x_margin+MAP_N/2;
			c = 0;
		}
	
}

void ghost_init(uint16_t posX, uint16_t posY, uint16_t speed){
		
		ghost.posX = posX;
		ghost.posY = posY;
		ghost.speed = speed;
		ghost.dir = G_LEFT;
	  ghost.anim_frame = 0;
	
		ghost_display(ghost.posX, ghost.posY, ghost.dir);
	
}

void ghost_display(uint16_t Xpos, uint16_t Ypos, uint8_t prev_dir){
	int i, j;
	uint8_t r, c;
	uint16_t tx, ty;
	int X, Y;
	
	pixels2map(Xpos, Ypos, &r, &c);
	
	X = Xpos - N/2;
	Y = Ypos - N/2;
	
	r = r - moves[prev_dir][0];
	c = c - moves[prev_dir][1];
	
	map2pixels(r, c, &tx, &ty);
	
	switch(map[r][c]){
		case 1:
			//print pill
			print_circle(PILL_DIM, tx, ty, Pink);
			break;
		case 4:
			//print gate
			print_gate(r, c, x_margin, y_margin);
			break;
		case 6:
			//print ppill
			print_circle(PPILL_DIM, tx, ty, Pink);
			break;
		default:
			break;
	}
	
		if (ghost.dir == G_LEFT || ghost.dir == G_UP){
			for(i=0; i<N; i++){
				for(j=0;j<N;j++){
					if(ghost_anim[ghost.anim_frame][i][N-j-1] == 1){
						LCD_SetPoint(X+j, Y+i, Red);
					}else if(ghost_anim[ghost.anim_frame][i][N-j-1] == 2){
						LCD_SetPoint(X+j, Y+i, White);
					}
				}
			}
		}
		else if (ghost.dir == G_RIGHT || ghost.dir == G_DOWN){
			for(i=0; i<N; i++){
				for(j=0;j<N;j++){
					if(ghost_anim[ghost.anim_frame][i][j] == 1){
						LCD_SetPoint(X+j, Y+i, Red);
					}else if(ghost_anim[ghost.anim_frame][i][j] == 2){
						LCD_SetPoint(X+j, Y+i, White);
					}
				}
			}
		}
	
	ghost.anim_frame ++;
	if(ghost.anim_frame >= 3)
		ghost.anim_frame = 0;
	
}

void ghost_clear(uint16_t Xpos, uint16_t Ypos){
	int i, j;
	int X, Y;
	
	X = Xpos - N/2;
	Y = Ypos - N/2;
	
	for(i=0; i<N; i++){
		for(j=0;j<N;j++){
				LCD_SetPoint(X+j, Y+i, Black);
		}
	}
	
}

void ghost_change_dir(uint8_t g_c, uint8_t g_r, uint8_t p_c, uint8_t p_r){
	uint8_t best_r = g_r;
	uint8_t best_c = g_c;
	int min_dist = N*N;
	uint8_t new_r, new_c, best_dir, new_dir;
	int i, dist;
		
	best_dir = ghost.dir;
		
	for(i=0; i<4; i++){
		new_r = g_r + moves[i][0];
		new_c = g_c + moves[i][1];
		new_dir = moves[i][2];
		
		if (new_r >= 0 && new_r < MAP_R && new_c >= 0 && new_c < MAP_C && map[new_r][new_c] != 0) {
			dist = abs(new_r - p_r) + abs(new_c - p_c);
			
			// Update the best move if this one is better
      if (dist < min_dist) {
          min_dist = dist;
          best_r = new_r;
          best_c = new_c;
				  best_dir = new_dir;
      }
		}
	}
	
	ghost.dir = best_dir;

}

void ghost_pos_update(uint8_t g_r, uint8_t g_c, uint8_t p_r, uint8_t p_c){
	
		if((ghost.posX-x_margin-MAP_N/2) % MAP_N == 0 && (ghost.posY-y_margin-MAP_N/2) % MAP_N == 0)
			ghost_change_dir(g_c, g_r, p_c, p_r);
	
			//position update
		switch(ghost.dir){
				case G_UP:
					if(map[g_r-1][g_c]!=0)
						ghost.posY -= ghost.speed;
					break;
				case G_DOWN:
					if(map[g_r+1][g_c]!=0)
						ghost.posY += ghost.speed;
					break;
				case G_LEFT:
					if(map[g_r][g_c-1]!=0)
						ghost.posX -= ghost.speed;
					break;
				case G_RIGHT:
					if(map[g_r][g_c+1]!=0)
						ghost.posX += ghost.speed;
					break;
				default:
					break;
		}
		
}

void counter_update(){
				
				game_state.counter --; 
				CAN_send_status(game_state.counter, game_state.lives, game_state.score); 
				//print_number(counter, 0, 17, White, Black);
					
				if(game_state.counter == 0)
					game_over();
				
}

void score_update(uint8_t PowerPill){
	
			 if(PowerPill == 1){
					game_state.score += 50;
			 }else{
					game_state.score += 10;
			 }
			 
			 //CAN_send_status(game_state.counter, game_state.lives, game_state.score);
	
			 //print_number(game_state.score, MAX_X-80, 17, White, Black);
			 
			 if(game_state.score >= game_state.lives*1000){
				 game_state.lives ++;
				 changeSound(NEW_LIFE);
			 }
			 
			 CAN_send_status(game_state.counter, game_state.lives, game_state.score); 					
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
	map[pills.v_pills[pos].r][pills.v_pills[pos].c] = 6;
	print_circle(PPILL_DIM, posX, posY, Pink);
	pills.pp_i++;
	
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

void map_init(){
	int i, j;
	
	for(i=0; i<MAP_R; i++){
		for(j=0; j<MAP_C; j++){
			switch(map[i][j]){
				case 0:
					//print wall tile
					print_tile(i, j, x_margin, y_margin);
					break;
				case 1:
					//print pill
					pill_create(i,j);
					break;
				case 2:
					//print pacman
					pacman_init(x_margin+j*MAP_N+(MAP_N/2), y_margin+i*MAP_N+(MAP_N/2), P_SPEED);
				case 3:
					break;
				case 4:
					//print gate
					print_gate(i, j, x_margin, y_margin);
					break;
				case 5:
					ghost_init(x_margin+j*MAP_N+(MAP_N/2), y_margin+i*MAP_N+(MAP_N/2), G_SPEED);
				  map[i][j] = 3;
				break;
				default:
					break;
			}
		}
	}
}


//valutare se spostare il funzionamento in CAN
void CAN_send_status(uint16_t counter, uint8_t lives, uint16_t score){
	
	char score_h = (score & 0xFF00) >> 8;
	
	CAN_TxMsg.data[0] = (char) score;
	CAN_TxMsg.data[1] = score_h;
	CAN_TxMsg.data[2] = (char)lives;
	CAN_TxMsg.data[3] = (char) counter;
	CAN_TxMsg.len = 4;
	CAN_TxMsg.id = 2;
	CAN_TxMsg.format = STANDARD_FORMAT;
	CAN_TxMsg.type = DATA_FRAME;
	CAN_wrMsg(1, &CAN_TxMsg);
}

uint16_t get_rand_in_range(int min, int max){
	
	int seed;
	uint16_t rand_n = 0;
	
	seed = (LPC_TIM0->TC)*65;
	seed &= 0xFFF;
	rand_n = (seed%(max-min-1))+min;
	
	return rand_n;
}