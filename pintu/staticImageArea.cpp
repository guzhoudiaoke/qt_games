/** staticImageArea.cpp
  * 孤舟钓客
  * 2011-08-01
  */

#include "staticImageArea.h"

StaticImageArea::StaticImageArea(QWidget *)
{
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::white);
    setPalette(p);
    setAutoFillBackground(true);

    img = new QImage(":/images/cat.jpg");

    setMinimumSize(img->width()+1, img->height()+1);
    setMaximumSize(img->width()+1, img->height()+1);
}

void StaticImageArea::paintEvent(QPaintEvent *)
{
    QPainter p(this);
    p.drawImage(0, 0, *img);
}
