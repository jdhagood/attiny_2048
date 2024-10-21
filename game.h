#include <Arduino.h>

#ifndef GAME_H
#define GAME_H

// Define constants
#define NUMPIXELS 16
#define ROWS 4
#define COLS 4

// Define types
typedef byte board[ROWS][COLS];

// Function declarations
void moveUp(board b);
void moveDown(board b);
void moveLeft(board b);
void moveRight(board b);

#endif