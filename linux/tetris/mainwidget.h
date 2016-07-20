/** mainwidget.h
  * guzhoudiaoke@126.com
  * 2011-08-04
  */

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include "tetrisarea.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *);

private:
    TetrisArea *tetrisArea;
};


#endif // MAINWIDGET_H
