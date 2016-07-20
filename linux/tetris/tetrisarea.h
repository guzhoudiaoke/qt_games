/** tetrisarea.h
  * guzhoudiaoke@126.com
  * 2011-08-04
  */

#ifndef TETRISAREA_H
#define TETRISAREA_H

#define MAP_WIDTH   12
#define MAP_HEIGHT  12
#define CELL_WIDTH  50

#include <QtGui>

class TetrisArea : public QWidget
{
    typedef struct pos{
        int x;
        int y;
    } pos;

    Q_OBJECT
public:
    TetrisArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);

    void initGame();
    void startGame();

    void rectDown();
    void rectLeft();
    void rectRight();
    void rectChange();

    bool canMoveLeft();
    bool canMoveRight();

    void setCurrent();
    void activeCurrent();

    bool isBottom();

private:
    QImage *img;

    int timer_no;
    int score;                                  // 得分
    pos current_pos[4];                         // 当前下坠物体的四个方块位置
    int state_map[MAP_HEIGHT][MAP_WIDTH];       // 方块地图
    int current_status;                         // 当前下坠物体的样式(1~7)
    int current_dir;                            // 当前下坠物体的四个方向(0~3)
    int inter_face[8][4][4];                    // 下落块接触面。7：七种类型下落块(0不用)，4：四个方向，4：第几个接触面
};

#endif // TETRISAREA_H
