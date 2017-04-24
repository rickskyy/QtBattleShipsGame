#ifndef CONSTANTS_H
#define CONSTANTS_H

const int ROWS = 10;
const int COLS = 10;
const int N = 10;
const int FLEET_SIZE = 10;
const int CELL_SIZE = 35;

static const int SHIP_SIZES [10] = { 4, 3, 3, 2, 2, 2, 1, 1, 1, 1 };

enum CellType { SHIP, SURROUND, EMPTY };
enum Direction { LEFT, UP, RIGHT, DOWN, NONE };
enum Player { USER, AI };
enum ShotResult { MISS, SHOT, KILL, NO };

#endif // CONSTANTS_H
