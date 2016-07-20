/** mainwidget.cpp
  * guzhoudiaoke@126.com
  * 2011-08-04
  */

#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    QFont font("ZYSong18030", 12);
    setFont(font);

    tetrisArea = new TetrisArea();

    // 布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(tetrisArea);
    mainLayout->setMargin(10);
    mainLayout->setSpacing(10);

    setLayout(mainLayout);

    setMinimumSize(MAP_WIDTH*CELL_WIDTH + 20, MAP_HEIGHT*CELL_WIDTH + 20);
    setMaximumSize(MAP_WIDTH*CELL_WIDTH + 20, MAP_HEIGHT*CELL_WIDTH + 20);

    setWindowTitle(tr("俄罗斯方块"));
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    switch (e->key())
    {
    case Qt::Key_Left:
        tetrisArea->rectLeft();
        break;
    case Qt::Key_Right:
        tetrisArea->rectRight();
        break;
    case Qt::Key_Up:
        tetrisArea->rectChange();
        break;
    case Qt::Key_Down:
        if (!tetrisArea->isBottom())
            tetrisArea->rectDown();
        break;
    default:
        break;
    }
}
