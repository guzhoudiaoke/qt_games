/** maparea.h
  * guzhoudiaoke@126.com
  * 2011-08-08
  */

#ifndef MAPAREA_H
#define MAPAREA_H

#include <QtGui>
#include "snakedefines.h"
#include "snake.h"

class MapArea : public QWidget
{
    Q_OBJECT
public:
    MapArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);

    void drawSnake(Snake *snake, QImage img);    // 画蛇

    void initGame();                    // 游戏初始化
    void startGame();                   // 开始游戏

    void changeDirect(int key);         // 改变蛇运动方向

    void clearFruit(int row, int col);  // 清除果子
    void clearPrizon(int row, int col); // 清除毒果子
    void addPrizon();                   // 添加毒果子
    void addFruit();                    // 添加果子

    bool isAtSnake(int row, int col, Snake *snake);      // 位置是否在蛇身上
    bool isGameover();

private:
    QImage *img_wall;
    QImage *img_snake1;
    QImage *img_snake2;

    int map[MAP_ROW][MAP_COL];  // 地图信息

    bool gameover;
    bool has_hole1;
    bool has_hole2;
    int timer_no;

    Snake snake1;
    Snake snake2;
};

#endif // MAPAREA_H
