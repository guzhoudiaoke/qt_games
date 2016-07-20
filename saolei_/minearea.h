/** minearea.h
  * guzhoudiaoke@126.com
  * 2011-08-02
  */

#ifndef MINEAREA_H
#define MINEAREA_H

#include <QtGui>

class MineArea : public QWidget
{
    typedef struct mine{
        int state;
        int attrib;
    } mine;

public:
    MineArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

    void initGame();
    void setMines();                            // 布雷
    void setFlag(int x, int y);                 // 设置标志（右键）
    void checkMine(int x, int y);               // 检查左键点击
    int  getAroundMineNum(int row, int col);    // 获取周围雷数目
    void expandMines(int row, int col);         // 扩展雷区
    void expandAll();                           // 展开所有雷
    bool isWin();                               // 检查是否获胜
    bool isGameover();                          // 是否游戏结束

    int getRowNum();
    int getColNum();

private:
    QImage *img;

    int x_num;
    int y_num;
    int mine_num;

    bool gameover;

    mine **mine_map;
};

#endif // MINEAREA_H
