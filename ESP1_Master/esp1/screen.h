#ifndef SCREEN_H
#define SCREEN_H

#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>

// ---------- Pins ----------
#define TFT_CS  22
#define TFT_RST 17
#define TFT_DC  16

// ---------- Colors ----------
#define BLACK      0x0000
#define WHITE      0xFFFF
#define YELLOW     0xFFE0
#define GREEN      0x07E0
#define DARK_GREEN 0x03E0
#define ORANGE     0xFD20
#define BROWN      0x8200
#define SKY_BLUE   0x867F
#define DARK_BROWN 0x6200

// ---------- Declarations ----------
void screen_setup();
void screen_clear();
void screen_showNumber(int n);
void screen_showSun();
void screen_showSmallLeaf();
void screen_showLeaves();
void screen_showSunflower();
void screen_showSad();
void screen_showHappy();

#endif