/** timearea.h
  * guzhoudiaoke@126.com
  * 2011-08-07
  */

#ifndef TIMEAREA_H
#define TIMEAREA_H

#include <QtGui>
#include "llkanarea.h"

class TimeArea : public QWidget
{
    Q_OBJECT
public:
    TimeArea(QWidget *parent = 0);
    void paintEvent(QPaintEvent *);
    void timerEvent(QTimerEvent *);

    void setLLKanArea(LLKanArea *area);
    void setTime();

private:
    int time_left;
    LLKanArea *llkan_area;
};

#endif // TIMEAREA_H
