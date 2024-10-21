#include "game.h"
#include <Arduino.h>

void moveUp(board b) {
  for (byte x = 0; x < COLS; x++) {
    // First, shift all non-zero tiles upwards
    for (byte y = 1; y < ROWS; y++) {
      if (b[y][x] != 0) {
        byte newY = y;
        // Move the tile up as far as possible
        while (newY > 0 && b[newY - 1][x] == 0) {
          b[newY - 1][x] = b[newY][x];
          b[newY][x] = 0;
          newY--;
        }
      }
    }

    // Then, merge adjacent tiles with the same value
    for (byte y = 1; y < ROWS; y++) {
      if (b[y][x] != 0 && b[y][x] == b[y - 1][x]) {
        b[y - 1][x]++;  // Merge the tiles by increasing the value (e.g., 2 becomes 4)
        b[y][x] = 0;    // Remove the merged tile
      }
    }

    // Shift again to fill any gaps created by merging
    for (byte y = 1; y < ROWS; y++) {
      if (b[y][x] != 0) {
        byte newY = y;
        while (newY > 0 && b[newY - 1][x] == 0) {
          b[newY - 1][x] = b[newY][x];
          b[newY][x] = 0;
          newY--;
        }
      }
    }
  }
}

// Function to move tiles downwards
void moveDown(board b) {
    for (byte x = 0; x < COLS; x++) {
        // First, shift all non-zero tiles downwards
        for (int y = ROWS - 2; y >= 0; y--) {  // Changed to int to avoid underflow
            if (b[y][x] != 0) {
                int newY = y;  // Changed to int
                while (newY < ROWS - 1 && b[newY+1][x] == 0) {
                    b[newY+1][x] = b[newY][x];
                    b[newY][x] = 0;
                    newY++;
                }
            }
        }

        // Then, merge adjacent tiles with the same value
        for (int y = ROWS - 2; y >= 0; y--) {  // Changed to int
            if (b[y][x] != 0 && b[y][x] == b[y+1][x]) {
                b[y+1][x]++; // Merge the tiles
                b[y][x] = 0;
            }
        }

        // Shift again to fill any gaps created by merging
        for (int y = ROWS - 2; y >= 0; y--) {  // Changed to int
            if (b[y][x] != 0) {
                int newY = y;  // Changed to int
                while (newY < ROWS - 1 && b[newY+1][x] == 0) {
                    b[newY+1][x] = b[newY][x];
                    b[newY][x] = 0;
                    newY++;
                }
            }
        }
    }
}

// Function to move tiles to the left
void moveLeft(board b) {
  for (byte y = 0; y < ROWS; y++) {
    for (byte x = 1; x < COLS; x++) {
      if (b[y][x] != 0) {
        byte newX = x;
        // Move the tile left as far as possible
        while (newX > 0 && b[y][newX - 1] == 0) {
          b[y][newX - 1] = b[y][newX];
          b[y][newX] = 0;
          newX--;
        }
      }
    }
    // Then, merge adjacent tiles with the same value
    for (byte x = 1; x < COLS; x++) {
      if (b[y][x] != 0 && b[y][x] == b[y][x - 1]) {
        b[y][x - 1]++;  // Merge the tiles by increasing the value (e.g., 2 becomes 4)
        b[y][x] = 0;    // Remove the merged tile
      }
    }
    // Shift again to fill any gaps created by merging
    for (byte x = 1; x < COLS; x++) {
      if (b[y][x] != 0) {
        byte newX = x;
        while (newX > 0 && b[y][newX - 1] == 0) {
          b[y][newX - 1] = b[y][newX];
          b[y][newX] = 0;
          newX--;
        }
      }
    }
  }
}

// Function to move tiles to the right
void moveRight(board b) {
  for (int y = 0; y < ROWS; y++) {
    // First, shift all non-zero tiles rightward
    for (int x = COLS - 2; x >= 0; x--) {
      if (b[y][x] != 0) {
        int newX = x;
        // Move the tile right as far as possible
        while (newX < COLS - 1 && b[y][newX + 1] == 0) {
          b[y][newX + 1] = b[y][newX];
          b[y][newX] = 0;
          newX++;
        }
      }
    }

    // Then, merge adjacent tiles with the same value
    for (int x = COLS - 2; x >= 0; x--) {
      if (b[y][x] != 0 && b[y][x] == b[y][x + 1]) {
        b[y][x + 1]++;  // Merge the tiles by increasing the value
        b[y][x] = 0;    // Remove the merged tile
      }
    }

    // Shift again to fill any gaps created by merging
    for (int x = COLS - 2; x >= 0; x--) {
      if (b[y][x] != 0) {
        int newX = x;
        while (newX < COLS - 1 && b[y][newX + 1] == 0) {
          b[y][newX + 1] = b[y][newX];
          b[y][newX] = 0;
          newX++;
        }
      }
    }
  }
}
