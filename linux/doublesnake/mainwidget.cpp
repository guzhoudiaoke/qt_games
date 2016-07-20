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

    map_area->changeDirect(e->key());
}
