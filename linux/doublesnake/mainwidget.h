/** mainwidget.h
  * guzhoudiaoke@126.com
  * 2011-08-08
  */

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include "maparea.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *);

private:
    MapArea *map_area;
};

#endif // MAINWIDGET_H
