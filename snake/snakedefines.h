/** snakedefines.h
  * guzhoudiaoke@126.com
  * 2011-08-08
  */

#ifndef SNAKEDEFINES_H
#define SNAKEDEFINES_H

#define MAP_ROW     30
#define MAP_COL     40
#define CELL_WIDTH  20
#define EDGE_WIDTH  2

#define FRUIT   0
#define PRIZON  1
#define WALL    2
#define NOTHING -1

#define HOLE_ROW    27
#define HOLE_COL    0

enum ImgState {
    IMG_NONE, IMG_UP_UP, IMG_DOWN_DOWN, IMG_LEFT_LEFT, IMG_RIGHT_RIGHT,
    IMG_UP_LEFT, IMG_UP_RIGHT, IMG_LEFT_UP, IMG_LEFT_DOWN,
    IMG_RIGHT_UP, IMG_RIGHT_DOWN, IMG_DOWN_RIGHT, IMG_DOWN_LEFT
};

enum MoveState {
    MOVE_NONE, MOVE_UP, MOVE_DOWN, MOVE_LEFT, MOVE_RIGHT
};

typedef struct SPoint
{
    int x;
    int y;
} SPoint;

#endif // SNAKEDEFINES_H
