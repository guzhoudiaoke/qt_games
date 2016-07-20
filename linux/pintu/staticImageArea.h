/** staticImageArea.h
  * 孤舟钓客
  * 2011-08-01
  */

#ifndef STATICIMAGEAREA_H
#define STATICIMAGEAREA_H

#include <QtGui>

class StaticImageArea : public QWidget
{
    Q_OBJECT
public:
    StaticImageArea(QWidget *parent = 0);

    void paintEvent(QPaintEvent *);

private:
    QImage *img;
};

#endif // STATICIMAGEAREA_H
