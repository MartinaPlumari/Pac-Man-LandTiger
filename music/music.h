#ifndef MUSIC_H
#define MUSIC_H

#include "LPC17xx.h"
//Default: 1.65
#define SPEEDUP 1.6

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
    c2 = 8480,    // 65Hz    k=8480 c2
    c2b = 8972,   // 61Hz    k=8972 c2b
    d2 = 7558,    // 73Hz    k=7558 d2
    d2b = 8006,   // 69Hz    k=8006 d2b
    e2 = 6734,    // 82Hz    k=6734 e2
    e2b = 7129,   // 77Hz    k=7129 e2b
    f2 = 6349,    // 87Hz    k=6349 f2
    f2b = 6734,   // 82Hz    k=6734 f2b
    g2 = 5667,    // 98Hz    k=5667 g2
    g2b = 5988,   // 92Hz    k=5988 g2b
    a2 = 5049,    // 110Hz   k=5049 a2
    a2b = 5351,   // 103Hz   k=5351 a2b
    b2 = 4500,    // 123Hz   k=4500 b2
    b2b = 4778,   // 116Hz   k=4778 b2b

    c3 = 4240,    // 131Hz   k=4240 c3
    c3b = 4370,   // 127Hz   k=4370 c3b
    d3 = 3779,    // 147Hz   k=3779 d3
    d3b = 4006,   // 139Hz   k=4006 d3b
    e3 = 3367,    // 165Hz   k=3367 e3
    e3b = 3551,   // 155Hz   k=3551 e3b
    f3 = 3175,    // 175Hz   k=3175 f3
    f3b = 3367,   // 165Hz   k=3367 f3b
    g3 = 2834,    // 196Hz   k=2834 g3
    g3b = 3003,   // 185Hz   k=3003 g3b
    a3 = 2525,    // 220Hz   k=2525 a3
    a3b = 2670,   // 208Hz   k=2670 a3b
    b3 = 2249,    // 247Hz   k=2249 b3
    b3b = 2389,   // 233Hz   k=2389 b3b

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

    c5 = 1062,    // 523Hz   k=1062 c5
    pause = 0      // DO NOT SOUND
} FREQUENCY;



typedef struct 
{
	FREQUENCY freq;
	NOTE_DURATION duration;
} NOTE;


extern NOTE theme[];

void playNote(NOTE note);
BOOL isNotePlaying(void);

#endif
/* EOF */
