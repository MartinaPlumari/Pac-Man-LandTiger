#include "animationLib.h"
#include "../GLCD/GLCD.h"
#include <stdio.h>

uint8_t eat_anim [4][N][N] = {
// First matrix
        {
            {0, 0, 1, 1, 1, 1, 0, 0},
						{0, 1, 1, 1, 1, 1, 1, 0},
						{1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1},
						{1, 1, 1, 1, 1, 1, 1, 1},
						{0, 1, 1, 1, 1, 1, 1, 0},
						{0, 0, 1, 1, 1, 1, 0, 0}
        },
        // Second frame: semi opened mouth
        {
            {0, 0, 1, 1, 1, 1, 0, 0},
						{0, 1, 1, 1, 1, 1, 1, 0},
						{1, 1, 1, 1, 1, 1, 0, 0},
						{1, 1, 1, 1, 1, 0, 0, 0},
						{1, 1, 1, 1, 1, 0, 0, 0},
						{1, 1, 1, 1, 1, 1, 0, 0},
						{0, 1, 1, 1, 1, 1, 1, 0},
						{0, 0, 1, 1, 1, 1, 0, 0}
        },
        // Third frame: opened mouth
        {
            {0, 0, 1, 1, 1, 1, 0, 0},
						{0, 1, 1, 1, 1, 1, 1, 0},
						{1, 1, 1, 1, 1, 0, 0, 0},
						{1, 1, 1, 1, 0, 0, 0, 0},
						{1, 1, 1, 1, 0, 0, 0, 0},
						{1, 1, 1, 1, 1, 0, 0, 0},
						{0, 1, 1, 1, 1, 1, 1, 0},
						{0, 0, 1, 1, 1, 1, 0, 0}
        },
        // Fourth frame: semi opened mouth
        {
            {0, 0, 1, 1, 1, 1, 0, 0},
						{0, 1, 1, 1, 1, 1, 1, 0},
						{1, 1, 1, 1, 1, 1, 0, 0},
						{1, 1, 1, 1, 1, 0, 0, 0},
						{1, 1, 1, 1, 1, 0, 0, 0},
						{1, 1, 1, 1, 1, 1, 0, 0},
						{0, 1, 1, 1, 1, 1, 1, 0},
						{0, 0, 1, 1, 1, 1, 0, 0}
        }
};

uint8_t life_sym[M][M] =
        {
            {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
						{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
						{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
						{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
						{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
						{1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0},
						{1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0},
						{1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0},
						{1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0},
						{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0},
						{0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0},
						{0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0}
        };


uint8_t map[MAP_R][MAP_C] = {
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0},
				{0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
				{0, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 0},
				{0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 0, 0, 1, 0},
				{0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0},
				{0, 1, 0, 0, 1, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1, 0, 0, 1, 0},
				{0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0},
				{0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0},
				{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
				{0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0},
				{3, 1, 1, 1, 1, 1, 1, 1, 0, 3, 3, 3, 0, 1, 1, 1, 1, 1, 1, 1, 3},
				{0, 1, 0, 0, 1, 0, 0, 1, 0, 3, 3, 3, 0, 1, 0, 0, 1, 0, 0, 1, 0},
				{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
				{0, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 0},
				{0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0},
				{0, 0, 0, 1, 0, 1, 0, 0, 0, 1, 2, 1, 0, 0, 0, 1, 0, 1, 0, 0, 0},
				{0, 0, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0},
				{0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0},
				{0, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 0, 0},
				{0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 1, 0, 0, 0},
				{0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
				{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

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
						if(((radius-j)*(radius-j) + (radius-i)*(radius-i)) <= radius*radius + 2){
								LCD_DrawLine(posX-radius+j+1, posY-radius+i+1, posX+radius-j-1, posY-radius+i+1, color);
								LCD_DrawLine(posX-radius+j+1, posY+radius-i-1, posX+radius-j-1, posY+radius-i-1, color);
								continue;
						}
					}
				}
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
}