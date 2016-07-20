/** maparea.cpp
  * guzhoudiaoke@126.com
  * 2011-08-03
  */

#include "maparea.h"
#include <iostream>
#include <fstream>

using namespace std;

MapArea::MapArea(QWidget *)
{
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::white);
    setPalette(p);
    setAutoFillBackground(true);

    img = new QImage(":/data/map.png");

    setMinimumSize(MAP_WIDTH *30, MAP_HEIGHT*30);
    setMaximumSize(MAP_WIDTH *30, MAP_HEIGHT*30);

    loadMap();
    gameover = false;
}

void MapArea::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    int i, j;
    // 绘制方格
    for (i = 0; i < MAP_HEIGHT; i++)
        for (j = 0; j < MAP_WIDTH; j++)
        {
            int wid = map[i][j] - '0';
            QPoint cell_topleft = QPoint(30*j, 30*i);
            QRect img_rect = QRect(QPoint(wid*30, 0), QSize(30, 30));
            p.drawImage(cell_topleft, *img, img_rect);
        }
}

void MapArea::loadMap()
{
    ifstream infile;
    infile.open("./data/map.info");
    if (!infile)
    {
        QMessageBox::warning(this, tr("警告"), tr("加载地图失败"));
        return;
    }
    //infile.read((char *)&map[0][0], sizeof(char) * MAP_WIDTH *MAP_HEIGHT);

    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
        for (j = 0; j < MAP_WIDTH; j++)
        {
            infile >> map[i][j];
            if (map[i][j] == MAP_MANWALL || map[i][j] == MAP_MANBALL)
            {
                man_pos.x = j;
                man_pos.y = i;
            }
        }

    infile.close();
}

void MapArea::manMove(int key)
{
    if (gameover)
        return;

    pos man_dis1, man_dis2;

    switch (key)
    {
    case Qt::Key_Left:
        man_dis1.x = man_pos.x-1;
        man_dis1.y = man_pos.y;
        man_dis2.x = man_pos.x-2;
        man_dis2.y = man_pos.y;
        break;
    case Qt::Key_Right:
        man_dis1.x = man_pos.x+1;
        man_dis1.y = man_pos.y;
        man_dis2.x = man_pos.x+2;
        man_dis2.y = man_pos.y;
        break;
    case Qt::Key_Up:
        man_dis1.x = man_pos.x;
        man_dis1.y = man_pos.y-1;
        man_dis2.x = man_pos.x;
        man_dis2.y = man_pos.y-2;
        break;
    case Qt::Key_Down:
        man_dis1.x = man_pos.x;
        man_dis1.y = man_pos.y+1;
        man_dis2.x = man_pos.x;
        man_dis2.y = man_pos.y+2;
        break;
    default:
        break;
    }
    updateMap(man_dis1, man_dis2);
}

void MapArea::updateMap(pos man_dis1, pos man_dis2)
{
    switch (map[man_dis1.y][man_dis1.x])
    {
    case MAP_WHITEWALL:     // 前边是墙，不能移动
        break;
    case MAP_BLUEWALL:      // 前边是通道
        map[man_dis1.y][man_dis1.x] = MAP_MANWALL;
        if (map[man_pos.y][man_pos.x] == MAP_MANWALL)
            map[man_pos.y][man_pos.x] = MAP_BLUEWALL;
        else if (map[man_pos.y][man_pos.x] == MAP_MANBALL)
            map[man_pos.y][man_pos.x] = MAP_BALL;
        man_pos.x = man_dis1.x;
        man_pos.y = man_dis1.y;
        break;
    case MAP_BALL:          // 前边是目标点
        map[man_dis1.y][man_dis1.x] = MAP_MANBALL;
        if (map[man_pos.y][man_pos.x] == MAP_MANWALL)
            map[man_pos.y][man_pos.x] = MAP_BLUEWALL;
        else if (map[man_pos.y][man_pos.x] == MAP_MANBALL)
            map[man_pos.y][man_pos.x] = MAP_BALL;
        man_pos.x = man_dis1.x;
        man_pos.y = man_dis1.y;
        break;
    case MAP_YELLOWBOX:     // 前边是箱子
        if (map[man_dis2.y][man_dis2.x] == MAP_BALL)    // 前边(箱子)的前边是目标点
        {
            map[man_dis2.y][man_dis2.x] = MAP_REDBOX;
            map[man_dis1.y][man_dis1.x] = MAP_MANWALL;
            if (map[man_pos.y][man_pos.x] == MAP_MANWALL)
                map[man_pos.y][man_pos.x] = MAP_BLUEWALL;
            else if (map[man_pos.y][man_pos.x] == MAP_MANBALL)
                map[man_pos.y][man_pos.x] = MAP_BALL;
            man_pos.x = man_dis1.x;
            man_pos.y = man_dis1.y;
        }
        else if (map[man_dis2.y][man_dis2.x] == MAP_BLUEWALL)
        {
            map[man_dis2.y][man_dis2.x] = MAP_YELLOWBOX;
            map[man_dis1.y][man_dis1.x] = MAP_MANWALL;
            if (map[man_pos.y][man_pos.x] == MAP_MANWALL)
                map[man_pos.y][man_pos.x] = MAP_BLUEWALL;
            else if (map[man_pos.y][man_pos.x] == MAP_MANBALL)
                map[man_pos.y][man_pos.x] = MAP_BALL;
            man_pos.x = man_dis1.x;
            man_pos.y = man_dis1.y;
        }
        break;
    case MAP_REDBOX:        // 前边是在目标点的箱子
        if (map[man_dis2.y][man_dis2.x] == MAP_BALL)    // 前边(箱子)的前边是目标点
        {
            map[man_dis2.y][man_dis2.x] = MAP_REDBOX;
            map[man_dis1.y][man_dis1.x] = MAP_MANBALL;
            if (map[man_pos.y][man_pos.x] == MAP_MANWALL)
                map[man_pos.y][man_pos.x] = MAP_BLUEWALL;
            else if (map[man_pos.y][man_pos.x] == MAP_MANBALL)
                map[man_pos.y][man_pos.x] = MAP_BALL;
            man_pos.x = man_dis1.x;
            man_pos.y = man_dis1.y;
        }
        else if (map[man_dis2.y][man_dis2.x] == MAP_BLUEWALL)
        {
            map[man_dis2.y][man_dis2.x] = MAP_YELLOWBOX;
            map[man_dis1.y][man_dis1.x] = MAP_MANBALL;
            if (map[man_pos.y][man_pos.x] == MAP_MANWALL)
                map[man_pos.y][man_pos.x] = MAP_BLUEWALL;
            else if (map[man_pos.y][man_pos.x] == MAP_MANBALL)
                map[man_pos.y][man_pos.x] = MAP_BALL;
            man_pos.x = man_dis1.x;
            man_pos.y = man_dis1.y;
        }
        break;

    case MAP_BACKGROUND:    // 前边是背景，不应该出现的情况，地图设计错误
    case MAP_MANWALL:       // 前边是工人，不应该出现的情况，地图设计错误
    case MAP_MANBALL:       // 前边是在目标点的工人，不应该出现的情况，地图设计错误
        QMessageBox::critical(this, tr("错误"), tr("地图设计错误！"));
        gameover = true;
        break;
    }

    update();

    if (isWin())
    {
        QMessageBox::information(this, tr("提示"), tr("恭喜，你完成任务了！"));
    }
}

bool MapArea::isWin()
{
    int i, j;
    for (i = 0; i < MAP_HEIGHT; i++)
        for (j = 0; j < MAP_WIDTH; j++)
        {
            if (map[i][j] == MAP_BALL || map[i][j] == MAP_MANBALL)
                return false;
        }

    gameover = true;
    return true;
}
