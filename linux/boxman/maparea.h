/** maparea.h
  * guzhoudiaoke@126.com
  * 2011-08-03
  */

#ifndef MAPAREA_H
#define MAPAREA_H

#include <QtGui>
#include "mapdefines.h"

class MapArea : public QWidget
{
    typedef struct pos {
        int x;
        int y;
    } pos;

    Q_OBJECT
public:
    MapArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);

    void loadMap();
    void manMove(int key);
    void updateMap(pos man_dis1, pos man_dis2);
    bool isWin();

private:
    QImage *img;

    char map[MAP_HEIGHT][MAP_WIDTH];
    pos man_pos;
    bool gameover;
};

#endif // MAPAREA_H
