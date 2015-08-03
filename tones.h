#ifndef TONES_H
#define TONES_H

#define F_CPU 16000000UL 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define p   0   //pause = silence
#define b3  253
#define c4  239
#define cx4 225
#define d4  213
#define dx4 201
#define e4  190
#define f4  179
#define fx4 169
#define g4  159
#define gx4 150
#define a4  142
#define ax4 134
#define b4  127
#define c5  119
#define cx5 113
#define d5  106
#define dx5 100
#define e5  95
#define f5  89
#define fx5 84
#define g5  80
#define gx5 75
#define a5  71
#define ax5 67
#define b5  63
#define c6  60
#define cx6 56
#define d6  53
#define dx6 50
#define e6  47
#define f6  45
#define fx6 42
#define g6  40
#define gx6 38
#define a6  36
#define b6  32
#define c7  30

unsigned char play_tune,*tune_array;
unsigned int tempo;
unsigned int ms_tick=0;  //general purpose millisecond timer tick, incremented by TIMER2

#endif /*TONES_H*/