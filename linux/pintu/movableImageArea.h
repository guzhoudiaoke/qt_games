/** movableImageArea.h
  * 孤舟钓客
  * 2011-08-01
  */

#ifndef MOVABLEIMAGEAREA_H
#define MOVABLEIMAGEAREA_H

#include <QtGui>

class MovableImageArea : public QWidget
{

    typedef struct pos {
        int row;            // 方块所在行
        int col;            // 方块所在列
    } pos;

    Q_OBJECT
public:
    MovableImageArea(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);

    void imgMove(int key);
    bool isWin();
    void initGame();

    int getCellWidth();
    int getCellHeight();
    int getImgWidth();
    int getImgHeight();
    pos getBlackPos();

private:
    QImage *img;
    bool iswin;

    int h_nums;             // 水平方向分块数
    int v_nums;             // 垂直方向分块数

    int img_width;          // 图像宽度
    int img_height;         // 图像高度

    int cell_width;         // 每块宽度
    int cell_height;        // 每块高度

    pos img_map[10][10];    // 每个方块对应的子图像位置
    pos black_pos;          // 空白方块对应位置
};

#endif // MOVABLEIMAGEAREA_H
