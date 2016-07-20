/** maparea.h
  * guzhoudiaoke@126.com
  * 2011-08-08
  */

#ifndef MAPAREA_H
#define MAPAREA_H

#include <QtGui>
#include "snakedefines.h"

class MapArea : public QWidget
{
    Q_OBJECT
public:
    MapArea(QWidget *parent = 0);
    ~MapArea();
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);

    void initGame();                    // 游戏初始化
    void startGame();                   // 开始游戏
    void snakeMove();                   // 蛇向前运动
    void snakeAddBody();                // 蛇长度增长 n
    void changeDirect(MoveState dir);   // 改变蛇运动方向

    void clearFruit(int row, int col);  // 清除果子
    void addPrizon();                   // 添加毒果子
    void addFruit();                    // 添加果子

    ImgState getDirect(MoveState old_dir, MoveState new_dir);

    bool isHeadTouchBody();
    bool isAtSnake(int row, int col);   // 位置是否在蛇身上
    bool isGameover();

private:
    QImage *img_wall;
    QImage *img_snake;

    int map[MAP_ROW][MAP_COL];  // 地图信息

    SPoint *snake_pos;          // 蛇的位置信息
    ImgState *snake_img_state;  // 蛇的身体对应图片

    typedef struct SnakeStruct{
        MoveState  head;        // 蛇头
        MoveState *body;        // 蛇身
        MoveState  tail;        // 蛇尾
    } SnakeStruct;

    SnakeStruct snake_new;      // 蛇的新状态所有节点运动状态
    SnakeStruct snake_old;      // 蛇的旧状态所有节点运动状态

    int snake_length;           // 蛇的长度

    bool gameover;
    bool has_hole;
    int timer_no;

    MoveState snake_dir;
};

#endif // MAPAREA_H
