/** tetrisarea.cpp
  * guzhoudiaoke@126.com
  * 2011-08-04
  */

#include "tetrisarea.h"
#include <time.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

TetrisArea::TetrisArea(QWidget *)
{
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::gray);
    setPalette(p);
    setAutoFillBackground(true);

    img = new QImage(":/images/rect.png");

    setMinimumSize(MAP_WIDTH*CELL_WIDTH + 1, MAP_HEIGHT*CELL_WIDTH + 1);
    setMaximumSize(MAP_WIDTH*CELL_WIDTH + 1, MAP_HEIGHT*CELL_WIDTH + 1);

    initGame();
    startGame();
}

void TetrisArea::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QPoint start_point, end_point;
    int i, j;
    // 绘制横向直线
    for (i = 0; i <= MAP_HEIGHT; i++)
    {
        start_point = QPoint(0, CELL_WIDTH*i);
        end_point = QPoint(MAP_WIDTH*CELL_WIDTH, CELL_WIDTH*i);
        p.drawLine(start_point, end_point);
    }

    // 绘制纵向直线
    for (j = 0; j <= MAP_WIDTH; j++)
    {
        start_point = QPoint(CELL_WIDTH*j, 0);
        end_point = QPoint(CELL_WIDTH*j, MAP_HEIGHT*CELL_WIDTH);
        p.drawLine(start_point, end_point);
    }

    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            if (state_map[i][j] != 0)
            {
                QPoint cell_topleft = QPoint(CELL_WIDTH*j, CELL_WIDTH*i);

                QRect img_rect = QRect(QPoint(0*CELL_WIDTH, 0), QSize(CELL_WIDTH, CELL_WIDTH));
                p.drawImage(cell_topleft, *img, img_rect);
            }
        }
    }
}

void TetrisArea::initGame()
{
    int i, j, k;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        for (j = 0; j < MAP_WIDTH; j++)
        {
            state_map[i][j] = 0;
        }
    }

    for (i = 0; i < 4; i++)
    {
        current_pos[i].x = 0;
        current_pos[i].y = MAP_HEIGHT;
    }

    for (i = 0; i < 8; i++)
        for (j = 0; j < 4; j++)
            for (k = 0; k < 4; k++)
                inter_face[i][j][k] = -1;

    // 类型 1，只有两个方向
    inter_face[1][0][0] = 3;
    inter_face[1][1][0] = 0; inter_face[1][1][0] = 1; inter_face[1][1][0] = 2; inter_face[1][1][0] = 3;

    // 类型 2，只有一个方向
    inter_face[2][0][0] = 2; inter_face[2][0][1] = 3;

    // 类型 3，有4个方向
    inter_face[3][0][0] = 2; inter_face[3][0][1] = 3;
    inter_face[3][1][0] = 0; inter_face[3][1][1] = 1; inter_face[3][1][2] = 2;
    inter_face[3][2][0] = 0; inter_face[3][2][1] = 3;
    inter_face[3][3][0] = 3; inter_face[3][3][1] = 1; inter_face[3][3][2] = 0;

    // 类型 4，有4个方向
    inter_face[4][0][0] = 0; inter_face[4][0][1] = 3;
    inter_face[4][1][0] = 1; inter_face[4][1][1] = 2; inter_face[4][1][2] = 0;
    inter_face[4][2][0] = 1; inter_face[4][2][1] = 0;
    inter_face[4][3][0] = 3; inter_face[4][3][1] = 2; inter_face[4][3][2] = 1;

    // 类型 5，有4个方向
    inter_face[5][0][0] = 2; inter_face[5][0][1] = 3;
    inter_face[5][1][0] = 0; inter_face[5][1][1] = 1; inter_face[5][1][2] = 2;
    inter_face[5][2][0] = 0; inter_face[5][2][1] = 3;
    inter_face[5][3][0] = 2; inter_face[5][3][1] = 3; inter_face[5][3][2] = 0;

    // 类型 6，有4个方向
    inter_face[6][0][0] = 1; inter_face[6][0][1] = 3;
    inter_face[6][1][0] = 2; inter_face[6][1][1] = 0; inter_face[6][1][0] = 1;
    inter_face[6][2][0] = 2; inter_face[6][2][1] = 0;
    inter_face[6][3][0] = 1; inter_face[6][3][1] = 3; inter_face[6][3][0] = 2;

    // 类型 7，有4个方向
    inter_face[7][0][0] = 1; inter_face[7][0][1] = 3;
    inter_face[7][1][0] = 0; inter_face[7][1][1] = 1; inter_face[7][1][2] = 3;
    inter_face[7][2][0] = 2; inter_face[7][2][1] = 0;
    inter_face[7][3][0] = 0; inter_face[7][3][1] = 2; inter_face[7][3][2] = 3;

    current_dir = 0;
    score = 0;
}

void TetrisArea::startGame()
{
    timer_no = startTimer(1000);
}

void TetrisArea::timerEvent(QTimerEvent *)
{
    // 消行以及积分
    bool is_succed = false;
    int succed_num = 0;

    int i, j, k, l;
    for (i = 0; i < MAP_HEIGHT; i++)
    {
        is_succed = true;
        for (j = 0; j < MAP_WIDTH; j++)
            if (state_map[i][j] == 0)
                is_succed = false;

        if (is_succed)                      // 该行已填满
        {
            for (k = i; k > 0; k--)         // 逐行下移
                for (l = 0; l < MAP_WIDTH; l++)
                    state_map[k][l] = state_map[k-1][l];

            for (k = 0; k < MAP_WIDTH; k++) // 第一行清空
                state_map[0][k] = 0;

            succed_num++;
            update();
        }
    }

    if (succed_num > 0)
    {
        score += succed_num * succed_num * 100;
    }

    bool is_bottom = isBottom();
    if (is_bottom)              // 碰到底
    {
        setCurrent();
        activeCurrent();

        is_bottom = isBottom();

        if (is_bottom)          // 碰到已落下的方块
        {
            killTimer(timer_no);
            QString str;
            str = tr("不好意思，你输了！你的分数是") + QString::number(score);
            QMessageBox::information(this, tr("提示"), str);
        }
    }
    else                        // 没碰到底
    {
        rectDown();
    }
}

void TetrisArea::rectDown()
{
    int i;
    for (i = 0; i < 4; i++)
    {
        state_map[current_pos[i].y][current_pos[i].x] = 0;
        current_pos[i].y += 1;
    }
    for (i = 0; i < 4; i++)
    {
        state_map[current_pos[i].y][current_pos[i].x] = 1;
    }
    update();
}

void TetrisArea::rectLeft()
{
    if (!canMoveLeft())
        return;

    int i;
    for (i = 0; i < 4; i++)
    {
        state_map[current_pos[i].y][current_pos[i].x] = 0;
        current_pos[i].x -= 1;
    }
    for (i = 0; i < 4; i++)
    {
        state_map[current_pos[i].y][current_pos[i].x] = 1;
    }
    update();
}

void TetrisArea::rectRight()
{
    if (!canMoveRight())
        return;

    int i;
    for (i = 0; i < 4; i++)
    {
        state_map[current_pos[i].y][current_pos[i].x] = 0;
        current_pos[i].x += 1;
    }
    for (i = 0; i < 4; i++)
    {
        state_map[current_pos[i].y][current_pos[i].x] = 1;
    }
    update();
}

void TetrisArea::rectChange()
{
    int x0 = current_pos[0].x;
    int y0 = current_pos[0].y;
    int x1 = current_pos[1].x;
    int y1 = current_pos[1].y;
    int x2 = current_pos[2].x;
    int y2 = current_pos[2].y;
    int x3 = current_pos[3].x;
    int y3 = current_pos[3].y;

    int change_dir;
    pos change_pos[4];

    switch (current_status)
    {
    case 1:
        if (current_dir == 0)
        {
            change_pos[0].x = x0-1; change_pos[0].y = y0+1;
            change_pos[1].x = x1+0; change_pos[1].y = y1-0;
            change_pos[2].x = x2+1; change_pos[2].y = y2-1;
            change_pos[3].x = x3+2; change_pos[3].y = y3-2;
            change_dir = 1;
        }
        else if (current_dir == 1)
        {
            change_pos[0].x = x0+1; change_pos[0].y = y0-1;
            change_pos[1].x = x1-0; change_pos[1].y = y1+0;
            change_pos[2].x = x2-1; change_pos[2].y = y2+1;
            change_pos[3].x = x3-2; change_pos[3].y = y3+2;
            change_dir = 0;
        }
        break;
    case 2:
        return;
        break;
    case 3:
        if (current_dir == 0)
        {
            change_pos[0].x = x0-1; change_pos[0].y = y0+1;
            change_pos[1].x = x1+0; change_pos[1].y = y1-0;
            change_pos[2].x = x2+1; change_pos[2].y = y2-1;
            change_pos[3].x = x3-0; change_pos[3].y = y3-2;
            change_dir = 1;
        }
        if (current_dir == 1)
        {
            change_pos[0].x = x0+1; change_pos[0].y = y0+1;
            change_pos[1].x = x1+0; change_pos[1].y = y1-0;
            change_pos[2].x = x2-1; change_pos[2].y = y2-1;
            change_pos[3].x = x3-2; change_pos[3].y = y3-0;
            change_dir = 2;
        }
        if (current_dir == 2)
        {
            change_pos[0].x = x0+1; change_pos[0].y = y0-1;
            change_pos[1].x = x1+0; change_pos[1].y = y1-0;
            change_pos[2].x = x2-1; change_pos[2].y = y2+1;
            change_pos[3].x = x3-0; change_pos[3].y = y3+2;
            change_dir = 3;
        }
        if (current_dir == 3)
        {
            change_pos[0].x = x0-1; change_pos[0].y = y0-1;
            change_pos[1].x = x1+0; change_pos[1].y = y1-0;
            change_pos[2].x = x2+1; change_pos[2].y = y2+1;
            change_pos[3].x = x3+2; change_pos[3].y = y3+0;
            change_dir = 0;
        }
        break;
    case 4:
        if (current_dir == 0)
        {
            change_pos[0].x = x0+2; change_pos[0].y = y0+0;
            change_pos[1].x = x1-1; change_pos[1].y = y1+1;
            change_pos[2].x = x2+0; change_pos[2].y = y2+0;
            change_pos[3].x = x3+1; change_pos[3].y = y3-1;
            change_dir = 1;
        }
        if (current_dir == 1)
        {
            change_pos[0].x = x0+0; change_pos[0].y = y0-2;
            change_pos[1].x = x1+1; change_pos[1].y = y1+1;
            change_pos[2].x = x2+0; change_pos[2].y = y2+0;
            change_pos[3].x = x3-1; change_pos[3].y = y3-1;
            change_dir = 2;
        }
        if (current_dir == 2)
        {
            change_pos[0].x = x0-2; change_pos[0].y = y0-0;
            change_pos[1].x = x1+1; change_pos[1].y = y1-1;
            change_pos[2].x = x2+0; change_pos[2].y = y2+0;
            change_pos[3].x = x3-1; change_pos[3].y = y3+1;
            change_dir = 3;
        }
        if (current_dir == 3)
        {
            change_pos[0].x = x0-0; change_pos[0].y = y0+2;
            change_pos[1].x = x1-1; change_pos[1].y = y1-1;
            change_pos[2].x = x2+0; change_pos[2].y = y2+0;
            change_pos[3].x = x3+1; change_pos[3].y = y3+1;
            change_dir = 0;
        }
        break;
    case 5:
        if (current_dir == 0)
        {
            change_pos[0].x = x0-1; change_pos[0].y = y0+1;
            change_pos[1].x = x1-0; change_pos[1].y = y1-0;
            change_pos[2].x = x2+1; change_pos[2].y = y2-1;
            change_pos[3].x = x3-1; change_pos[3].y = y3-1;
            change_dir = 1;
        }
        if (current_dir == 1)
        {
            change_pos[0].x = x0+1; change_pos[0].y = y0+1;
            change_pos[1].x = x1-0; change_pos[1].y = y1-0;
            change_pos[2].x = x2-1; change_pos[2].y = y2-1;
            change_pos[3].x = x3-1; change_pos[3].y = y3+1;
            change_dir = 2;
        }
        if (current_dir == 2)
        {
            change_pos[0].x = x0+1; change_pos[0].y = y0-1;
            change_pos[1].x = x1-0; change_pos[1].y = y1-0;
            change_pos[2].x = x2-1; change_pos[2].y = y2+1;
            change_pos[3].x = x3+1; change_pos[3].y = y3+1;
            change_dir = 3;
        }
        if (current_dir == 3)
        {
            change_pos[0].x = x0-1; change_pos[0].y = y0-1;
            change_pos[1].x = x1-0; change_pos[1].y = y1-0;
            change_pos[2].x = x2+1; change_pos[2].y = y2+1;
            change_pos[3].x = x3+1; change_pos[3].y = y3-1;
            change_dir = 0;
        }
        break;
    case 6:
        if (current_dir == 0)
        {
            change_pos[0].x = x0-0; change_pos[0].y = y0-0;
            change_pos[1].x = x1+1; change_pos[1].y = y1-1;
            change_pos[2].x = x2-2; change_pos[2].y = y2+0;
            change_pos[3].x = x3-1; change_pos[3].y = y3-1;
            change_dir = 1;
        }
        if (current_dir == 1)
        {
            change_pos[0].x = x0-0; change_pos[0].y = y0-0;
            change_pos[1].x = x1-1; change_pos[1].y = y1-1;
            change_pos[2].x = x2-0; change_pos[2].y = y2+2;
            change_pos[3].x = x3-1; change_pos[3].y = y3+1;
            change_dir = 2;
        }
        if (current_dir == 2)
        {
            change_pos[0].x = x0-0; change_pos[0].y = y0-0;
            change_pos[1].x = x1-1; change_pos[1].y = y1+1;
            change_pos[2].x = x2+2; change_pos[2].y = y2+0;
            change_pos[3].x = x3+1; change_pos[3].y = y3+1;
            change_dir = 3;
        }
        if (current_dir == 3)
        {
            change_pos[0].x = x0-0; change_pos[0].y = y0-0;
            change_pos[1].x = x1+1; change_pos[1].y = y1+1;
            change_pos[2].x = x2+0; change_pos[2].y = y2-2;
            change_pos[3].x = x3+1; change_pos[3].y = y3-1;
            change_dir = 0;
        }
        break;
    case 7:
        if (current_dir == 0)
        {
            change_pos[0].x = x0-1; change_pos[0].y = y0+1;
            change_pos[1].x = x1+0; change_pos[1].y = y1+0;
            change_pos[2].x = x2-1; change_pos[2].y = y2-1;
            change_pos[3].x = x3-0; change_pos[3].y = y3-2;
            change_dir = 1;
        }
        if (current_dir == 1)
        {
            change_pos[0].x = x0+1; change_pos[0].y = y0+1;
            change_pos[1].x = x1+0; change_pos[1].y = y1+0;
            change_pos[2].x = x2-1; change_pos[2].y = y2+1;
            change_pos[3].x = x3-2; change_pos[3].y = y3-0;
            change_dir = 2;
        }
        if (current_dir == 2)
        {
            change_pos[0].x = x0+1; change_pos[0].y = y0-1;
            change_pos[1].x = x1+0; change_pos[1].y = y1+0;
            change_pos[2].x = x2+1; change_pos[2].y = y2+1;
            change_pos[3].x = x3-0; change_pos[3].y = y3+2;
            change_dir = 3;
        }
        if (current_dir == 3)
        {
            change_pos[0].x = x0-1; change_pos[0].y = y0-1;
            change_pos[1].x = x1+0; change_pos[1].y = y1+0;
            change_pos[2].x = x2+1; change_pos[2].y = y2-1;
            change_pos[3].x = x3+2; change_pos[3].y = y3+0;
            change_dir = 0;
        }
        break;
    }

    int i;
    // 测试变形后有没有足够的空间
    for (i = 0; i < 4; i++)
        state_map[current_pos[i].y][current_pos[i].x] = 0;

    for (i = 0; i < 4; i++)
    {
        if (state_map[change_pos[i].y][change_pos[i].x] ||      // 空间已被占据
            change_pos[i].x < 0 || change_pos[i].x >= MAP_WIDTH ||
            change_pos[i].y < 0 || change_pos[i].y >= MAP_HEIGHT)
        {
            for (i = 0; i < 4; i++)
                state_map[current_pos[i].y][current_pos[i].x] = 1;
            return;
        }
    }

    for (i = 0; i < 4; i++)
    {
        current_pos[i].x = change_pos[i].x;
        current_pos[i].y = change_pos[i].y;
        state_map[change_pos[i].y][change_pos[i].x] = 1;
    }
    current_dir = change_dir;
    update();
}

bool TetrisArea::canMoveLeft()
{
    int i;
    for (i = 0; i < 4; i++)
        if (current_pos[i].x == 0 || current_pos[i].y == MAP_HEIGHT-1)
            return false;

    int x0 = current_pos[0].x;
    int y0 = current_pos[0].y;
    int x1 = current_pos[1].x;
    int y1 = current_pos[1].y;
    int x2 = current_pos[2].x;
    int y2 = current_pos[2].y;
    int x3 = current_pos[3].x;
    int y3 = current_pos[3].y;

    switch (current_status)
    {
    case 1:
        if (current_dir == 0)
        {
            if (state_map[y0][x0-1] || state_map[y1][x1-1] || state_map[y2][x2-1] || state_map[y3][x3-1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y0][x0-1])
                return false;
        }
        break;
    case 2:
        if (current_dir == 0)
        {
            if (state_map[y0][x0-1] || state_map[y2][x2-1])
                return false;
        }
        break;
    case 3:
        if (current_dir == 0)
        {
            if (state_map[y0][x0-1] || state_map[y1][x1-1] || state_map[y2][x2-1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y0][x0-1])
                return false;
        }
        else if (current_dir == 2)
        {
            if (state_map[y0][x0-1] || state_map[y1][x1-1] || state_map[y3][x3-1])
                return false;
        }
        else if (current_dir == 3)
        {
            if (state_map[y3][x3-1] || state_map[y2][x2-1])
                return false;
        }
        break;
    case 4:
        if (current_dir == 0)
        {
            if (state_map[y0][x0-1] || state_map[y1][x1-1] || state_map[y2][x2-1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y0][x0-1] || state_map[y1][x1-1])
                return false;
        }
        else if (current_dir == 2)
        {
            if (state_map[y2][x2-1] || state_map[y1][x1-1] || state_map[y3][x3-1])
                return false;
        }
        else if (current_dir == 3)
        {
            if (state_map[y3][x3-1] || state_map[y0][x0-1])
                return false;
        }
        break;
    case 5:
        if (current_dir == 0)
        {
            if (state_map[y0][x0-1] || state_map[y1][x1-1] || state_map[y2][x2-1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y0][x0-1] || state_map[y3][x3-1])
                return false;
        }
        else if (current_dir == 2)
        {
            if (state_map[y0][x0-1] || state_map[y2][x2-1] || state_map[y3][x3-1])
                return false;
        }
        else if (current_dir == 3)
        {
            if (state_map[y3][x3-1] || state_map[y2][x2-1])
                return false;
        }
        break;
    case 6:
        if (current_dir == 0)
        {
            if (state_map[y0][x0-1] || state_map[y1][x1-1] || state_map[y2][x2-1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y0][x0-1] || state_map[y2][x2-1])
                return false;
        }
        else if (current_dir == 2)
        {
            if (state_map[y1][x1-1] || state_map[y2][x2-1] || state_map[y3][x3-1])
                return false;
        }
        else if (current_dir == 3)
        {
            if (state_map[y3][x3-1] || state_map[y1][x1-1])
                return false;
        }
        break;
    case 7:
        if (current_dir == 0)
        {
            if (state_map[y0][x0-1] || state_map[y1][x1-1] || state_map[y3][x3-1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y0][x0-1] || state_map[y2][x2-1])
                return false;
        }
        else if (current_dir == 2)
        {
            if (state_map[y0][x0-1] || state_map[y2][x2-1] || state_map[y3][x3-1])
                return false;
        }
        else if (current_dir == 3)
        {
            if (state_map[y3][x3-1] || state_map[y1][x1-1])
                return false;
        }
        break;
    }
    return true;
}

bool TetrisArea::canMoveRight()
{
    int i;
    for (i = 0; i < 4; i++)
        if (current_pos[i].x == MAP_WIDTH-1 || current_pos[i].y == MAP_HEIGHT-1)
            return false;

    int x0 = current_pos[0].x;
    int y0 = current_pos[0].y;
    int x1 = current_pos[1].x;
    int y1 = current_pos[1].y;
    int x2 = current_pos[2].x;
    int y2 = current_pos[2].y;
    int x3 = current_pos[3].x;
    int y3 = current_pos[3].y;

    switch (current_status)
    {
    case 1:
        if (current_dir == 0)
        {
            if (state_map[y0][x0+1] || state_map[y1][x1+1] || state_map[y2][x2+1] || state_map[y3][x3+1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y3][x3+1])
                return false;
        }
        break;
    case 2:
        if (current_dir == 0)
        {
            if (state_map[y1][x1+1] || state_map[y3][x3+1])
                return false;
        }
        break;
    case 3:
        if (current_dir == 0)
        {
            if (state_map[y0][x0+1] || state_map[y1][x1+1] || state_map[y3][x3+1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y2][x2+1] || state_map[y3][x3+1])
                return false;
        }
        else if (current_dir == 2)
        {
            if (state_map[y0][x0+1] || state_map[y1][x1+1] || state_map[y2][x2+1])
                return false;
        }
        else if (current_dir == 3)
        {
            if (state_map[y0][x0+1])
                return false;
        }
        break;
    case 4:
        if (current_dir == 0)
        {
            if (state_map[y3][x3+1] || state_map[y1][x1+1] || state_map[y2][x2+1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y0][x0+1] || state_map[y3][x3+1])
                return false;
        }
        else if (current_dir == 2)
        {
            if (state_map[y0][x0+1] || state_map[y1][x1+1] || state_map[y2][x2+1])
                return false;
        }
        else if (current_dir == 3)
        {
            if (state_map[y0][x0+1] || state_map[y1][x1+1])
                return false;
        }
        break;
    case 5:
        if (current_dir == 0)
        {
            if (state_map[y0][x0+1] || state_map[y3][x3+1] || state_map[y2][x2+1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y2][x2+1] || state_map[y3][x3+1])
                return false;
        }
        else if (current_dir == 2)
        {
            if (state_map[y0][x0+1] || state_map[y2][x2+1] || state_map[y1][x1+1])
                return false;
        }
        else if (current_dir == 3)
        {
            if (state_map[y3][x3+1] || state_map[y0][x0+1])
                return false;
        }
        break;
    case 6:
        if (current_dir == 0)
        {
            if (state_map[y3][x3+1] || state_map[y1][x1+1] || state_map[y2][x2+1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y1][x1+1] || state_map[y3][x3+1])
                return false;
        }
        else if (current_dir == 2)
        {
            if (state_map[y1][x1+1] || state_map[y2][x2+1] || state_map[y0][x0+1])
                return false;
        }
        else if (current_dir == 3)
        {
            if (state_map[y0][x0+1] || state_map[y2][x2+1])
                return false;
        }
        break;
    case 7:
        if (current_dir == 0)
        {
            if (state_map[y0][x0+1] || state_map[y2][x2+1] || state_map[y3][x3+1])
                return false;
        }
        else if (current_dir == 1)
        {
            if (state_map[y1][x1+1] || state_map[y3][x3+1])
                return false;
        }
        else if (current_dir == 2)
        {
            if (state_map[y1][x1+1] || state_map[y3][x3+1] || state_map[y0][x0+1])
                return false;
        }
        else if (current_dir == 3)
        {
            if (state_map[y0][x0+1] || state_map[y2][x2+1])
                return false;
        }
        break;
    }
    return true;
}

void TetrisArea::setCurrent()
{
    qsrand((unsigned int)time(NULL));
    current_status = qrand()%7 + 1;     // 1~7 的随机数，表示7 种类型的下落物体

    switch (current_status)
    {
    case 1:
        current_pos[0].x = 5; current_pos[0].y = 0;
        current_pos[1].x = 5; current_pos[1].y = 1;
        current_pos[2].x = 5; current_pos[2].y = 2;
        current_pos[3].x = 5; current_pos[3].y = 3;
        break;
    case 2:
        current_pos[0].x = 5; current_pos[0].y = 0;
        current_pos[1].x = 6; current_pos[1].y = 0;
        current_pos[2].x = 5; current_pos[2].y = 1;
        current_pos[3].x = 6; current_pos[3].y = 1;
        break;
    case 3:
        current_pos[0].x = 5; current_pos[0].y = 0;
        current_pos[1].x = 5; current_pos[1].y = 1;
        current_pos[2].x = 5; current_pos[2].y = 2;
        current_pos[3].x = 6; current_pos[3].y = 2;
        break;
    case 4:
        current_pos[0].x = 5; current_pos[0].y = 2;
        current_pos[1].x = 6; current_pos[1].y = 0;
        current_pos[2].x = 6; current_pos[2].y = 1;
        current_pos[3].x = 6; current_pos[3].y = 2;
        break;
    case 5:
        current_pos[0].x = 5; current_pos[0].y = 0;
        current_pos[1].x = 5; current_pos[1].y = 1;
        current_pos[2].x = 5; current_pos[2].y = 2;
        current_pos[3].x = 6; current_pos[3].y = 1;
        break;
    case 6:
        current_pos[0].x = 5; current_pos[0].y = 1;
        current_pos[1].x = 5; current_pos[1].y = 2;
        current_pos[2].x = 6; current_pos[2].y = 0;
        current_pos[3].x = 6; current_pos[3].y = 1;
        break;
    case 7:
        current_pos[0].x = 5; current_pos[0].y = 0;
        current_pos[1].x = 5; current_pos[1].y = 1;
        current_pos[2].x = 6; current_pos[2].y = 1;
        current_pos[3].x = 6; current_pos[3].y = 2;
        break;
    default:
        break;
    }
    current_dir = 0;
}

void TetrisArea::activeCurrent()
{
    int i;
    for (i = 0; i < 4; i++)
        state_map[current_pos[i].y][current_pos[i].x] = 1;

    update();
}

bool TetrisArea::isBottom()
{
    int i;
    // 检测是否与底部相碰
    for (i = 0; i < 4; i++)
        if (current_pos[i].y >= MAP_HEIGHT-1)
            return true;

    // 检测是否与已落下的下落物体相碰
    for (i = 0; i < 4; i++)
    {
        // 取接触面序号
        int index = inter_face[current_status][current_dir][i];
        if (index == -1)
            continue;

        // 当前下落物体，序号为index 的接触面正下方的坐标
        pos down_pos;
        down_pos.x = current_pos[index].x;
        down_pos.y = current_pos[index].y + 1;

        // 判断上面得到的坐标是否已被占据
        if (state_map[down_pos.y][down_pos.x] == 1)
            return true;
    }

    return false;
}
