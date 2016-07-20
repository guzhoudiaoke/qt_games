/** minearea.cpp
  * guzhoudiaoke@126.com
  * 2011-08-02
  */

#include "minearea.h"
#include "minedefines.h"
#include <time.h>
#include <stdlib.h>

MineArea::MineArea(QWidget *)
{
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::white);
    setPalette(p);
    setAutoFillBackground(true);

    img = new QImage(":/images/img.png");

    x_num = EASY_X;                 // x 方向方格数
    y_num = EASY_Y;                 // y 方向方格数
    mine_num = EASY_NUM;            // 雷数

    // 分配空间
    int i;
    mine_map = new mine*[y_num+2];
    for (i = 0; i < y_num+2; i++)
        mine_map[i] = new mine[x_num+2];

    setMinimumSize(x_num*16, y_num*16);
    setMaximumSize(x_num*16, y_num*16);

    initGame();
    setMines();
    update();
}

void MineArea::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    int i, j;
    // 绘制方格
    for (i = 1; i <= y_num; i++)
        for (j = 1; j <= x_num; j++)
        {
            QPoint cell_topleft = QPoint(16*(j-1), 16*(i-1));
            QRect img_rect = QRect(QPoint(0, mine_map[i][j].state*16), QSize(16, 16));
            p.drawImage(cell_topleft, *img, img_rect);
        }
}

void MineArea::initGame()
{
    int i, j;
    for (i = 0; i < y_num+2; i++)
        for (j = 0; j < x_num+2; j++)
        {
            mine_map[i][j].attrib = ATTRIB_EMPTY;
            mine_map[i][j].state = STATE_NORMAL;
        }

    gameover = false;
}

void MineArea::setMines()
{
    qsrand((unsigned)time(NULL));

    int x, y, num;
    for (num = 0; num < mine_num;)
    {
        x = qrand()%x_num + 1;
        y = qrand()%y_num + 1;

        if (mine_map[y][x].attrib != ATTRIB_MINE)
        {
            mine_map[y][x].attrib = ATTRIB_MINE;
            //mine_map[y][x].state = STATE_MINE;
            num++;
        }
    }
}

void MineArea::setFlag(int x, int y)
{
    if (x >= 0 && x < x_num*16 && y >= 0 && y < y_num*16)
    {
        int row = y/16 + 1;
        int col = x/16 + 1;

        if (mine_map[row][col].state == STATE_NORMAL)
            mine_map[row][col].state = STATE_FLAG;
        else if (mine_map[row][col].state == STATE_FLAG)
            mine_map[row][col].state = STATE_DICEY;
        else if (mine_map[row][col].state == STATE_DICEY)
            mine_map[row][col].state = STATE_NORMAL;

        update();
    }
}

void MineArea::checkMine(int x, int y)
{
    if (x >= 0 && x < x_num*16 && y >= 0 && y < y_num*16)
    {
        int row = y/16 + 1;
        int col = x/16 + 1;

        if (mine_map[row][col].state == STATE_NORMAL)
        {
            if (mine_map[row][col].attrib == ATTRIB_EMPTY)
            {
                mine_map[row][col].state = STATE_EMPTY;
                int num = getAroundMineNum(row, col);
                mine_map[row][col].state = 15 - num;
                expandMines(row, col);
            }
            else if (mine_map[row][col].attrib == ATTRIB_MINE)
            {
                mine_map[row][col].state = STATE_BLAST;
                expandAll();
                gameover = true;
            }
        }

        update();
    }
}

void MineArea::expandAll()
{
    int i, j;
    for (i = 1; i <= y_num; i++)
        for (j = 1; j <= x_num; j++)
        {
            if (mine_map[i][j].attrib == ATTRIB_MINE)
                mine_map[i][j].state = STATE_BLAST;
        }
}

bool MineArea::isGameover()
{
    return gameover;
}

int MineArea::getAroundMineNum(int row, int col)
{
    int i, j, num;
    num = 0;
    for (i = row-1; i <= row+1; i++)
        for (j = col-1; j <= col+1; j++)
        {
            if (mine_map[i][j].attrib == ATTRIB_MINE)
                num++;
        }

    return num;
}

void MineArea::expandMines(int row, int col)
{
    int num = getAroundMineNum(row, col);
    mine_map[row][col].state = 15 - num;

    if (num == 0)   // 周围无雷，递归探索周围是否可以扩展
    {
        int i, j;
        for (i = row-1; i <= row+1; i++)
            for (j = col-1; j <= col+1; j++)
            {
                if (i == row && j == col)
                    continue;
                if (mine_map[i][j].attrib == ATTRIB_EMPTY && mine_map[i][j].state == STATE_NORMAL)
                {
                    if (i < 1 || i > y_num || j < 1 || j > x_num)
                        continue;
                    expandMines(i, j);
                }
            }
    }
}

bool MineArea::isWin()
{
    int i, j;
    for (i = 1; i <= y_num; i++)
        for (j = 1; j <= x_num; j++)
        {
            if (mine_map[i][j].state == STATE_NORMAL)
                return false;
            if (mine_map[i][j].state == STATE_DICEY)
                return false;
        }

    gameover = true;
    QMessageBox::information(this, tr("提示"), tr("恭喜，你获胜了!"));
    return true;
}

int MineArea::getRowNum()
{
    return x_num;
}

int MineArea::getColNum()
{
    return y_num;
}
