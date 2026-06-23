#ifndef AUDIO_H
#define AUDIO_H

#include "DFRobotDFPlayerMini.h"

// ---------- Pins ----------
#define MP3_RX 27
#define MP3_TX 26

// ---------- Volume ----------
#define AUDIO_VOLUME 23

// ---------- Track numbers ----------
#define TRACK_1 1
#define TRACK_2 2
#define TRACK_3 3
#define TRACK_4 4
#define TRACK_5 5
#define TRACK_6 6
#define TRACK_7 7
#define TRACK_8 8

// ---------- Declarations ----------
void audio_setup();
void audio_drain();
void audio_play1();
void audio_play2();
void audio_play3();
void audio_play4();
void audio_play5();
void audio_play6();
void audio_play7();
void audio_play8();
void audio_stop();
void audio_setVolume(int vol);

#endif
