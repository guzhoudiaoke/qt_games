/** minedefines.h
  * guzhoudiaoke@126.com
  * 2011-08-02
  */

#ifndef MINEDEFINES_H
#define MINEDEFINES_H

#define EASY_X              10
#define EASY_Y              10
#define EASY_NUM            10

#define SECOND_X            15
#define SECOND_Y            15
#define SECOND_NUM          25

#define HARD_X              20
#define HARD_Y              20
#define HARD_NUM            50


// 普通
#define STATE_NORMAL        0
// 标志
#define STATE_FLAG          1
// 未知
#define STATE_DICEY         2
// 爆炸
#define STATE_BLAST         3
// 错误
#define STATE_ERROR         4
// 雷
#define STATE_MINE          5
// 未知（按下）
#define STATE_DICEY_DOWN    6
// 雷数目
#define STATE_8             7
#define STATE_7             8
#define STATE_6             9
#define STATE_5             10
#define STATE_4             11
#define STATE_3             12
#define STATE_2             13
#define STATE_1             14
#define STATE_EMPTY         15

// 有没有雷
#define ATTRIB_EMPTY        0
#define ATTRIB_MINE         1

#endif // MINEDEFINES_H
