#ifndef MUSIC_H
#define MUSIC_H

#include "LPC17xx.h"
//Default: 1.65
#define SPEEDUP 1.0

#define TIMERSCALER 1

#define SECOND 0x17D7840 * TIMERSCALER

// beat 1/4 = 1.65/4 seconds
#define RIT_SEMIMINIMA 8
#define RIT_MINIMA 16
#define RIT_INTERA 32

#define UPTICKS 1

typedef char BOOL;
#define TRUE 1
#define FALSE 0

typedef enum sound_codes{
	THEME,
	CHOMP,
	NEW_LIFE,
	LOSE
}SOUND_CODE;
	
typedef enum note_durations
{
	time_semibiscroma = (unsigned int)(SECOND * SPEEDUP / 64.0f + 0.5), // 1/128
	time_biscroma = (unsigned int)(SECOND * SPEEDUP / 32.0f + 0.5), // 1/64
	time_semicroma = (unsigned int)(SECOND * SPEEDUP / 16.0f + 0.5), // 1/32
	time_croma = (unsigned int)(SECOND * SPEEDUP / 8.0f + 0.5), // 1/16
	time_semiminima = (unsigned int)(SECOND * SPEEDUP / 4.0f + 0.5), // 1/4
	time_minima = (unsigned int)(SECOND * SPEEDUP / 2.0f + 0.5), // 1/2
	time_semibreve = (unsigned int)(SECOND * SPEEDUP + 0.5), // 1
} NOTE_DURATION;

typedef enum frequencies
{
    // Ottava 4
    c4 = 2120,    // 262Hz   k=2120 c4
    c4b = 2249,   // 247Hz   k=2249 c4b
    d4 = 1890,    // 294Hz   k=1890 d4
    d4b = 2003,   // 277Hz   k=2003 d4b
    e4 = 1684,    // 330Hz   k=1684 e4
    e4b = 1782,   // 311Hz   k=1782 e4b
    f4 = 1592,    // 349Hz   k=1592 f4
    f4b = 1684,   // 330Hz   k=1684 f4b
    g4 = 1417,    // 392Hz   k=1417 g4
    g4b = 1501,   // 370Hz   k=1501 g4b
    a4 = 1263,    // 440Hz   k=1263 a4
    a4b = 1335,   // 415Hz   k=1335 a4b
    b4 = 1125,    // 494Hz   k=1125 b4
    b4b = 1194,   // 466Hz   k=1194 b4b

    // Ottava 5
    c5 = 1062,    // 523Hz   k=1062 c5
    c5b = 1125,   // 494Hz   k=1125 c5b
    d5 = 945,     // 587Hz   k=945 d5
    d5b = 1001,   // 554Hz   k=1001 d5b
    e5 = 842,     // 659Hz   k=842 e5
    e5b = 891,    // 622Hz   k=891 e5b
    f5 = 796,     // 698Hz   k=796 f5
    f5b = 842,    // 659Hz   k=842 f5b
    g5 = 709,     // 784Hz   k=709 g5
    g5b = 750,    // 740Hz   k=750 g5b
    a5 = 632,     // 880Hz   k=632 a5
    a5b = 668,    // 830Hz   k=668 a5b
    b5 = 562,     // 988Hz   k=562 b5
    b5b = 597,    // 932Hz   k=597 b5b

    // Ottava 6
    c6 = 531,     // 1047Hz  k=531 c6
    c6b = 562,    // 988Hz   k=562 c6b
    d6 = 472,     // 1175Hz  k=472 d6
    d6b = 501,    // 1109Hz  k=501 d6b
    e6 = 421,     // 1319Hz  k=421 e6
    e6b = 445,    // 1245Hz  k=445 e6b
    f6 = 398,     // 1397Hz  k=398 f6
    f6b = 421,    // 1319Hz  k=421 f6b
    g6 = 354,     // 1568Hz  k=354 g6
    g6b = 375,    // 1480Hz  k=375 g6b
    a6 = 316,     // 1760Hz  k=316 a6
    a6b = 334,    // 1661Hz  k=334 a6b
    b6 = 281,     // 1976Hz  k=281 b6
    b6b = 298,    // 1865Hz  k=298 b6b

    // Ottava 7
    c7 = 265,     // 2093Hz  k=265 c7
    c7b = 281,    // 1976Hz  k=281 c7b
    d7 = 236,     // 2349Hz  k=236 d7
    d7b = 250,    // 2217Hz  k=250 d7b
    e7 = 210,     // 2637Hz  k=210 e7
    e7b = 223,    // 2489Hz  k=223 e7b
    f7 = 199,     // 2794Hz  k=199 f7
    f7b = 210,    // 2637Hz  k=210 f7b
    g7 = 177,     // 3136Hz  k=177 g7
    g7b = 188,    // 2960Hz  k=188 g7b
    a7 = 158,     // 3520Hz  k=158 a7
    a7b = 167,    // 3322Hz  k=167 a7b
    b7 = 141,     // 3951Hz  k=141 b7
    b7b = 149,    // 3729Hz  k=149 b7b

    pause = 0      // DO NOT SOUND
} FREQUENCY;




typedef struct 
{
	FREQUENCY freq;
	NOTE_DURATION duration;
} NOTE;


void playNote(NOTE note);
BOOL isNotePlaying(void);
void playSound();
void changeSound(SOUND_CODE code);
void sfx_init();

#endif
/* EOF */
