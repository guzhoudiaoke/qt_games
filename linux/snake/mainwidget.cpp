/** mainwidget.cpp
  * guzhoudiaoke@126.com
  * 2011-08-05
  */

#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    QFont font("ZYSong18030", 12);
    setFont(font);

    map_area = new MapArea();

    // 布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(map_area);
    mainLayout->setMargin(10);
    mainLayout->setSpacing(10);

    setLayout(mainLayout);
    setWindowTitle(tr("贪吃蛇"));
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    if (map_area->isGameover())
        return;

    switch (e->key())
    {
    case Qt::Key_Up:
        map_area->changeDirect(MOVE_UP);
        break;
    case Qt::Key_Down:
        map_area->changeDirect(MOVE_DOWN);
        break;
    case Qt::Key_Left:
        map_area->changeDirect(MOVE_LEFT);
        break;
    case Qt::Key_Right:
        map_area->changeDirect(MOVE_RIGHT);
        break;
    default:
        break;
    }
}
