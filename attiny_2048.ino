#include <tinyNeoPixel.h>
#include "game.h"
// Pin where the NeoPixel data line is connected
#define PIN PIN_PA3

// Number of NeoPixels
#define NUMPIXELS 16  // 4x4 grid
#define ROWS 4
#define COLS 4

// Initialize the strip for SK6812 RGBW
tinyNeoPixel strip = tinyNeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// Define button pins
#define MIDDLE_BUTTON PIN_PB0
#define DOWN_BUTTON PIN_PC0
#define LEFT_BUTTON PIN_PC1
#define RIGHT_BUTTON PIN_PC2
#define UP_BUTTON PIN_PC3  //ON PC3

typedef byte board[ROWS][COLS];
board gameBoard = { 0 };  // Initialize the board to zeros

void setup() {
  strip.begin();  // Initialize NeoPixel strip
  strip.clear();
  strip.show();  // Initialize all pixels to 'off'

  // Set up button inputs
  pinMode(MIDDLE_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);

  // Initialize board with two random tiles
  spawnTile(gameBoard);
  spawnTile(gameBoard);
  /*
  uint32_t c = strip.Color(0, 0, 10); //R G B
  strip.setPixelColor(0, c);
  strip.setPixelColor(1, strip.Color(10, 0, 0));
  strip.setPixelColor(2, strip.Color(0, 10, 0));
  strip.setPixelColor(3, strip.Color(0, 0, 10));
  strip.show();
  */
  drawBoard(gameBoard);
}

void loop() {
  if (digitalRead(UP_BUTTON) == LOW || digitalRead(DOWN_BUTTON) == LOW || digitalRead(LEFT_BUTTON) == LOW || digitalRead(RIGHT_BUTTON) == LOW) {
    if (digitalRead(UP_BUTTON) == LOW) {
      moveUp(gameBoard);
    }
    if (digitalRead(DOWN_BUTTON) == LOW) {  //freezes game
      moveDown(gameBoard);
    }
    if (digitalRead(LEFT_BUTTON) == LOW) {
      moveLeft(gameBoard);
    }
    if (digitalRead(RIGHT_BUTTON) == LOW) {
      moveRight(gameBoard);
    }
    spawnTile(gameBoard);
    drawBoard(gameBoard);
    delay(100);
    while (digitalRead(UP_BUTTON) == LOW || digitalRead(DOWN_BUTTON) == LOW || digitalRead(LEFT_BUTTON) == LOW || digitalRead(RIGHT_BUTTON) == LOW) {}

    if(game_won(gameBoard) == true) {
      win_animation();
      reset_game(gameBoard);
    }
    
    if(game_lost(gameBoard) == true) {
      lose_animation();
      reset_game(gameBoard);
    }
  }
}

bool game_lost(board b) {
  //should return true only if there are no more open squares and up down left and right does not lead to any merges
  byte previousValue_x = 0;
  byte previousValue_y = 0;
  for (byte y = 0; y < ROWS; y++) {
    for (byte x = 0; x < COLS; x++) {
      if(b[y][x] == 0) {//blank space and we can keep going
        return false;
      }
      if (b[y][x] == previousValue_x) {//two of the same tiles are next to each other
        return false;
      }
      if(b[x][y] == previousValue_y) {
        return false;
      }
      previousValue_x = b[y][x];
      previousValue_y = b[x][y];
    }
    previousValue_x = 0;
    previousValue_y = 0;
  }

  return true;
}

void reset_game(board b) {
  for (byte y = 0; y < ROWS; y++) {
    for (byte x = 0; x < COLS; x++) {
      b[y][x] = 0;
    }
  }
  spawnTile(b);
  spawnTile(b);
  drawBoard(b);
}

bool game_won(board b) {
  for (byte y = 0; y < ROWS; y++) {
    for (byte x = 0; x < COLS; x++) {
      if(b[y][x] >= 11) {
        return true;
      }
    }
  }
  return false;
}

// Function to spawn a tile (2 or 4) at a random empty location
void spawnTile(board b) {
  // Find all empty positions
  byte emptyTiles[ROWS * COLS];
  byte emptyCount = 0;

  for (byte y = 0; y < ROWS; y++) {
    for (byte x = 0; x < COLS; x++) {
      if (b[y][x] == 0) {
        emptyTiles[emptyCount++] = y * COLS + x;
      }
    }
  }

  // If no empty tiles, return
  if (emptyCount == 0) return;

  // Pick a random empty position
  byte idx = random(0, emptyCount);
  byte pos = emptyTiles[idx];

  // 90% chance to spawn a 2, 10% chance to spawn a 4
  byte value = (random(0, 10) < 9) ? 1 : 2;

  // Assign the value to the board
  b[pos / COLS][pos % COLS] = value;
}

// Function to draw the current state of the board
void drawBoard(board b) {
  strip.clear();
  for (byte y = 0; y < ROWS; y++) {
    for (byte x = 0; x < COLS; x++) {
      uint32_t color = getColorForValue(b[y][x]);
      setDisplayPixel(x, y, color);
    }
  }
  strip.show();
}

// Function to set pixel color based on position in the grid
void setDisplayPixel(byte x, byte y, uint32_t color) {
  y = COLS - 1 - y;
  if (y % 2 == 0) {
    strip.setPixelColor(y * COLS + COLS - x - 1, color);  // Zigzag pattern
  } else {
    strip.setPixelColor(y * COLS + x, color);
  }
}

float totalBrightness = 0.10;
// Function to map tile values to colors (you can customize these); Make the colors more distinct in the future
uint32_t getColorForValue(byte value) {
  totalBrightness = constrain(totalBrightness, 0, 1);  // Ensure brightness is between 0 and 1

  switch (value) {
    case 1: return strip.Color(255 * totalBrightness, 0, 0);                                           // Red
    case 2: return strip.Color(0, 255 * totalBrightness, 0);                                           // Green
    case 3: return strip.Color(0, 0, 255 * totalBrightness);                                           // Blue
    case 4: return strip.Color(255 * totalBrightness, 255 * totalBrightness, 0);                       // Yellow
    case 5: return strip.Color(0, 255 * totalBrightness, 200 * totalBrightness);                       // Cyan
    case 6: return strip.Color(255 * totalBrightness, 0, 255 * totalBrightness);                       // Magenta
    case 7: return strip.Color(255 * totalBrightness, 100 * totalBrightness, 0);                       // Orange
    case 8: return strip.Color(128 * totalBrightness, 0, 50 * totalBrightness);                        // Purple
    case 9: return strip.Color(255 * totalBrightness, 50 * totalBrightness, 255 * totalBrightness);    // Pink
    case 10: return strip.Color(255 * totalBrightness, 200 * totalBrightness, 50 * totalBrightness);   // Light Green
    case 11: return strip.Color(255 * totalBrightness, 255 * totalBrightness, 255 * totalBrightness);  // White
    default: return strip.Color(0, 0, 0);                                                              // Off for empty
  }
}

void win_animation() {
  // Define the letters for "WIN!"
  const board U = {
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 0, 0, 1 },
    { 1, 1, 1, 1 }
  };

  const board W = {
    { 2, 0, 0, 2 },
    { 2, 0, 0, 2 },
    { 2, 2, 2, 2 },
    { 0, 2, 2, 0 }
  };

  const board I = {
    { 3, 3, 3, 3 },
    { 0, 3, 3, 0 },
    { 0, 3, 3, 0 },
    { 3, 3, 3, 3 }
  };

  const board N = {
    { 4, 0, 0, 4 },
    { 4, 4, 0, 4 },
    { 4, 0, 4, 4 },
    { 4, 0, 0, 4 }
  };

  // Animation loop
  for (byte i = 0; i < 3; i++) {
    drawBoard(U);
    delay(1000);
    drawBoard(W);
    delay(1000);
    drawBoard(I);
    delay(1000);
    drawBoard(N);
    delay(1000);
  }
}

void lose_animation() {
  const board red = { { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 } };
  const board clear = { 0 };
  for (byte i = 0; i < 3; i++) {
    drawBoard(red);
    delay(500);
    drawBoard(clear);
    delay(500);
  }
}