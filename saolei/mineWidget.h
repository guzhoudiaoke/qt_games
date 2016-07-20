/** mineWidget.h
  * guzhoudiaoke@126.com
  * 2011-08-02
  */

#ifndef MINEWIDGET_H
#define MINEWIDGET_H

#include <QtGui>
#include "minearea.h"

class MineWidget : public QWidget
{
    Q_OBJECT
public:
    MineWidget(QWidget *parent = 0);

    void mousePressEvent(QMouseEvent *);

private:
    MineArea *mineArea;
};

#endif // MINEWIDGET_H
