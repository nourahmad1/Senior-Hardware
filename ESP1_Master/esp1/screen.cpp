#include "screen.h"
#include <Arduino.h>

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// =============================================
// Internal helper
// =============================================

void _clearScreen(uint16_t color = BLACK) {
  tft.fillScreen(color);
}

// =============================================
// Setup
// =============================================

void screen_setup() {
  tft.init(240, 320);
  tft.setRotation(1);
  tft.invertDisplay(false);
  _clearScreen(BLACK);

  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(80, 110);
  //tft.println("Plant Monitor");
  delay(1500);
  _clearScreen(BLACK);
}

void screen_clear() {
  _clearScreen(BLACK);
}

// =============================================
// D0, D1, D2, D3 — big number
// =============================================

void screen_showNumber(int n) {
  _clearScreen(BLACK);

  tft.setTextColor(WHITE);
  tft.setTextSize(10);
  tft.setCursor(130, 80);
  tft.print(n);

  tft.setTextSize(2);
  tft.setTextColor(0xAD55);
  tft.setCursor(110, 200);
  //tft.print("Stage ");
 // tft.print(n);
}

// =============================================
// SUN ☀️
// =============================================

void screen_showSun() {
  _clearScreen(SKY_BLUE);

  int cx = 160, cy = 100;

  for (int i = 0; i < 12; i++) {
    float angle = i * 3.14159 * 2 / 12;
    int x1 = cx + 50 * cos(angle);
    int y1 = cy + 50 * sin(angle);
    int x2 = cx + 70 * cos(angle);
    int y2 = cy + 70 * sin(angle);
    tft.drawLine(x1,   y1, x2,   y2, YELLOW);
    tft.drawLine(x1+1, y1, x2+1, y2, YELLOW);
  }

  tft.fillCircle(cx, cy, 45, YELLOW);
  tft.fillCircle(cx, cy, 38, ORANGE);

  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(115, 185);
  tft.println("Sunny day!");
}

// =============================================
// SLF — baby sunflower seedling 🌱
// =============================================

void screen_showSmallLeaf() {
  _clearScreen(BLACK);

  int cx = 160;

  // Soil
  tft.fillRect(0, 195, 320, 45, BROWN);
  tft.fillRect(0, 195, 320, 8,  DARK_BROWN);

  // Short thick stem
  tft.fillRect(cx - 4, 150, 8, 45, DARK_GREEN);

  // Left cotyledon
  tft.fillCircle(cx - 22, 148, 16, GREEN);
  tft.fillCircle(cx - 28, 142, 13, GREEN);
  tft.fillCircle(cx - 16, 142, 13, GREEN);
  tft.drawLine(cx - 3, 152, cx - 24, 144, DARK_GREEN);

  // Right cotyledon
  tft.fillCircle(cx + 22, 148, 16, GREEN);
  tft.fillCircle(cx + 28, 142, 13, GREEN);
  tft.fillCircle(cx + 16, 142, 13, GREEN);
  tft.drawLine(cx + 3, 152, cx + 24, 144, DARK_GREEN);

  // Tiny bud at top
  tft.fillCircle(cx - 5, 142, 6, DARK_GREEN);
  tft.fillCircle(cx + 5, 142, 6, DARK_GREEN);
  tft.fillCircle(cx,     138, 5, GREEN);

  // Small sun hint corner
  tft.fillCircle(270, 40, 18, YELLOW);
  for (int i = 0; i < 8; i++) {
    float a = i * 3.14159 * 2 / 8;
    tft.drawLine(270 + 20*cos(a), 40 + 20*sin(a),
                 270 + 28*cos(a), 40 + 28*sin(a), YELLOW);
  }

  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(85, 210);
  tft.println("Seedling!");
}

// =============================================
// LF — young sunflower, more leaves, bigger 🌿
// clearly part of the sunflower growth story
// =============================================
// =============================================
// LF — young sunflower growing up 🌿
// smaller and shorter than SFL
// =============================================

void screen_showLeaves() {
  _clearScreen(SKY_BLUE);

  int cx = 160;

  // Ground
  tft.fillRect(0, 195, 320, 45, BROWN);
  tft.fillRect(0, 195, 320, 8,  DARK_BROWN);

  // Stem — shorter and thinner than SFL (SFL stem is 10px wide, 160px tall)
  // this one is 7px wide, 110px tall — clearly smaller
  tft.fillRect(cx - 3, 85, 7, 110, DARK_GREEN);

  // --- Bottom-left leaf (medium, not huge) ---
  tft.fillCircle(cx - 25, 158, 15, GREEN);
  tft.fillCircle(cx - 33, 150, 12, GREEN);
  tft.fillCircle(cx - 20, 148, 13, GREEN);
  tft.fillTriangle(cx-36, 143, cx-44, 133, cx-28, 139, GREEN);
  tft.drawLine(cx - 2, 160, cx - 34, 144, DARK_GREEN);
  tft.drawLine(cx - 18, 153, cx - 28, 146, DARK_GREEN);

  // --- Bottom-right leaf (medium) ---
  tft.fillCircle(cx + 25, 138, 15, GREEN);
  tft.fillCircle(cx + 33, 130, 12, GREEN);
  tft.fillCircle(cx + 20, 128, 13, GREEN);
  tft.fillTriangle(cx+36, 123, cx+44, 113, cx+28, 119, GREEN);
  tft.drawLine(cx + 2, 140, cx + 34, 124, DARK_GREEN);
  tft.drawLine(cx + 18, 133, cx + 28, 126, DARK_GREEN);

  // --- Mid-left small leaf ---
  tft.fillCircle(cx - 20, 115, 11, GREEN);
  tft.fillCircle(cx - 27, 108, 9,  GREEN);
  tft.fillCircle(cx - 14, 108, 10, GREEN);
  tft.drawLine(cx - 2, 117, cx - 24, 106, DARK_GREEN);

  // --- Bud at top — yellow just peeking ---
  tft.fillCircle(cx, 80, 11, DARK_GREEN);
  tft.fillCircle(cx, 75,  8, GREEN);
  for (int i = 0; i < 6; i++) {
    float a = i * 3.14159 * 2 / 6;
    tft.fillCircle(cx + 7*cos(a), 75 + 7*sin(a), 3, YELLOW);
  }
  tft.fillCircle(cx, 75, 5, DARK_GREEN);

  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(95, 210);
  tft.println("Growing up!");
}

// =============================================
// SFL — full sunflower 🌻
// =============================================

void screen_showSunflower() {
  _clearScreen(SKY_BLUE);

  int cx = 160, cy = 90;

  // Ground
  tft.fillRect(0, 195, 320, 45, BROWN);
  tft.fillRect(0, 195, 320, 8,  DARK_BROWN);

  // Thick stem
  tft.fillRect(cx - 5, cy + 35, 10, 160, DARK_GREEN);

  // Left stem leaf
  tft.fillCircle(cx - 32, cy + 95, 22, GREEN);
  tft.fillCircle(cx - 44, cy + 85, 17, GREEN);
  tft.fillTriangle(cx-50, cy+75, cx-62, cy+62, cx-40, cy+70, GREEN);
  tft.drawLine(cx - 4, cy + 98, cx - 46, cy + 78, DARK_GREEN);

  // Right stem leaf
  tft.fillCircle(cx + 32, cy + 130, 22, GREEN);
  tft.fillCircle(cx + 44, cy + 120, 17, GREEN);
  tft.fillTriangle(cx+50, cy+110, cx+62, cy+97, cx+40, cy+104, GREEN);
  tft.drawLine(cx + 4, cy + 133, cx + 46, cy + 113, DARK_GREEN);

  // Petals — 16 for fullness
  for (int i = 0; i < 16; i++) {
    float angle = i * 3.14159 * 2 / 16;
    int px = cx + 36 * cos(angle);
    int py = cy + 36 * sin(angle);
    tft.fillCircle(px, py, 14, YELLOW);
  }

  // Outer petal ring (slightly offset)
  for (int i = 0; i < 16; i++) {
    float angle = (i + 0.5) * 3.14159 * 2 / 16;
    int px = cx + 38 * cos(angle);
    int py = cy + 38 * sin(angle);
    tft.fillCircle(px, py, 10, ORANGE);
  }

  // Center
  tft.fillCircle(cx, cy, 26, DARK_BROWN);
  tft.fillCircle(cx, cy, 20, BROWN);

  // Seed pattern
  for (int i = 0; i < 8; i++) {
    float angle = i * 3.14159 * 2 / 8;
    tft.fillCircle(cx + 11*cos(angle), cy + 11*sin(angle), 3, DARK_BROWN);
  }
  for (int i = 0; i < 5; i++) {
    float angle = i * 3.14159 * 2 / 5;
    tft.fillCircle(cx + 5*cos(angle), cy + 5*sin(angle), 2, DARK_BROWN);
  }

  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(100, 210);
  tft.println("Sunflower!");
}

// =============================================
// SAD 😢 — simple clean version
// =============================================

void screen_showSad() {
  _clearScreen(BLACK);

  int cx = 160, cy = 105;

  // Face
  tft.fillCircle(cx, cy, 75, YELLOW);
  tft.drawCircle(cx, cy, 75, ORANGE);

  // Left eye
  tft.fillCircle(cx - 25, cy - 18, 10, BLACK);
  tft.fillCircle(cx - 25, cy - 18,  5, WHITE);

  // Right eye
  tft.fillCircle(cx + 25, cy - 18, 10, BLACK);
  tft.fillCircle(cx + 25, cy - 18,  5, WHITE);

  // Tears
  tft.fillCircle(cx - 28, cy - 2,  5, SKY_BLUE);
  tft.fillCircle(cx - 26, cy + 8,  4, SKY_BLUE);
  tft.fillCircle(cx + 28, cy - 2,  5, SKY_BLUE);
  tft.fillCircle(cx + 26, cy + 8,  4, SKY_BLUE);

  // Sad mouth
  for (int i = -18; i <= 18; i++) {
    float angle = i * 3.14159 / 18.0;
    int mx = cx + 30 * sin(angle);
    int my = cy + 35 - 14 * cos(angle);
    tft.drawPixel(mx,   my,   BLACK);
    tft.drawPixel(mx+1, my,   BLACK);
    tft.drawPixel(mx,   my+1, BLACK);
  }

  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(125, 195);
  tft.println("Sad");
}

// =============================================
// HPY 😊 — simple clean version
// =============================================

void screen_showHappy() {
  _clearScreen(BLACK);

  int cx = 160, cy = 105;

  // Face
  tft.fillCircle(cx, cy, 75, YELLOW);
  tft.drawCircle(cx, cy, 75, ORANGE);

  // Left eye
  tft.fillCircle(cx - 25, cy - 18, 10, BLACK);
  tft.fillCircle(cx - 22, cy - 21,  4, WHITE);

  // Right eye
  tft.fillCircle(cx + 25, cy - 18, 10, BLACK);
  tft.fillCircle(cx + 28, cy - 21,  4, WHITE);

  // Rosy cheeks
  tft.fillCircle(cx - 48, cy + 5, 14, 0xF810);
  tft.fillCircle(cx + 48, cy + 5, 14, 0xF810);

  // Happy mouth
  for (int i = -20; i <= 20; i++) {
    float angle = i * 3.14159 / 20.0;
    int mx = cx + 32 * sin(angle);
    int my = cy + 28 + 18 * cos(angle);
    tft.drawPixel(mx,   my,   BLACK);
    tft.drawPixel(mx+1, my,   BLACK);
    tft.drawPixel(mx,   my+1, BLACK);
    tft.drawPixel(mx+1, my+1, BLACK);
  }

  tft.setTextColor(WHITE);
  tft.setTextSize(2);
  tft.setCursor(115, 195);
  tft.println("Happy");
}