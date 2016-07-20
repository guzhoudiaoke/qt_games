/** timearea.cpp
  * guzhoudiaoke@126.com
  * 2011-08-07
  */

#include "timearea.h"

TimeArea::TimeArea(QWidget *)
{
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::white);
    setPalette(p);
    setAutoFillBackground(true);

    setMinimumSize(120,  MAP_ROW*FRAME_HEIGHT);
    setMaximumSize(120,  MAP_ROW*FRAME_HEIGHT);

    time_left = 0;
    startTimer(500);
}

void TimeArea::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    QString str;
    str = tr("剩余时间: ");
    str += QString::number(time_left);
    p.drawText(QRect(20, 20, 80, 80), str);

    QPoint start, end;
    start = QPoint(54, 40);
    end = QPoint(65, (MAP_ROW*FRAME_HEIGHT-10));
    p.drawRect(QRect(start, end));

    start = QPoint(55, 40 + (double)((MAP_ROW*FRAME_HEIGHT-50) * (MAX_TIME - time_left)) / MAX_TIME);
    end = QPoint(65, (MAP_ROW*FRAME_HEIGHT-10));

    QBrush brush;
    brush.setColor(QColor(int(255.0*(1.0 - (double)time_left/(double)MAX_TIME)),
                          int(255.0*(double)time_left/(double)MAX_TIME),
                          36,
                          255));

    brush.setStyle(Qt::SolidPattern);
    p.setBrush(brush);
    p.fillRect(QRect(start, end), brush);
}

void TimeArea::timerEvent(QTimerEvent *)
{
    time_left = llkan_area->getTimeLeft();
    update();
}

void TimeArea::setLLKanArea(LLKanArea *area)
{
    llkan_area = area;
}
