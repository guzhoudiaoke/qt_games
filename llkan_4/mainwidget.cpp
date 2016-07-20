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

    llkanArea = new LLKanArea();
    timeArea = new TimeArea();

    timeArea->setLLKanArea(llkanArea);

    // 布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(llkanArea);
    mainLayout->addWidget(timeArea);
    mainLayout->setMargin(10);
    mainLayout->setSpacing(10);

    setLayout(mainLayout);

    setMinimumSize(MAP_COL*FRAME_WIDTH + 150, MAP_ROW*FRAME_HEIGHT + 20);
    setMaximumSize(MAP_COL*FRAME_WIDTH + 150, MAP_ROW*FRAME_HEIGHT + 20);

    setWindowTitle(tr("真三装备连连看"));
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        int x = e->x() - 10;
        int y = e->y() - 10;
        llkanArea->lbuttonDownAt(x, y);
    }
}
