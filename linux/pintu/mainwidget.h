/** mainwdiget.h
  * 孤舟钓客
  * 2011-08-01
  */

#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QtGui>
#include "movableImageArea.h"
#include "staticImageArea.h"

class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QWidget *parent = 0);

    void keyPressEvent(QKeyEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);

    void createMenus();
    void createActions();

private:
    MovableImageArea *movableImageArea;
    StaticImageArea *staticImageArea;

    QMenu *hardMenu;
    QMenu *imgMenu;
    QMenu *aboutMenu;

    QAction *hard3x3Action;
    QAction *hard4x4Action;
    QAction *hard5x5Action;

protected slots:
};

#endif // MAINWIDGET_H
