/** mainwidget.cpp
  * guzhoudiaoke@126.com
  * 2011-08-03
  */

#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    QFont font("ZYSong18030",12);
    setFont(font);

    mapArea = new MapArea;

    // 布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(mapArea);
    mainLayout->setMargin(10);
    mainLayout->setSpacing(10);

    setLayout(mainLayout);

    setMinimumSize(MAP_WIDTH *30 + 20, MAP_HEIGHT*30 + 20);
    setMaximumSize(MAP_WIDTH *30 + 20, MAP_HEIGHT*30 + 20);

    setWindowTitle(tr("推箱子"));
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    mapArea->manMove(e->key());
}
