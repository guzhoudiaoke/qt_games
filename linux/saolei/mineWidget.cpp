/** mineWidget.cpp
  * guzhoudiaoke@126.com
  * 2011-08-02
  */

#include "mineWidget.h"

MineWidget::MineWidget(QWidget *)
{
    QFont font("ZYSong18030", 12);
    setFont(font);

    mineArea = new MineArea;

    // 布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(mineArea);
    mainLayout->setMargin(10);
    mainLayout->setSpacing(10);

    setLayout(mainLayout);
    setMinimumSize(mineArea->getColNum()*16 + 20, mineArea->getRowNum()*16 + 20);
    setMaximumSize(mineArea->getColNum()*16 + 20, mineArea->getRowNum()*16 + 20);

    setWindowTitle(tr("扫雷游戏"));
}

void MineWidget::mousePressEvent(QMouseEvent *e)
{
    if (mineArea->isGameover())
        return;

    int x, y;
    x = e->x() - 10;
    y = e->y() - 10;

    if (e->button() == Qt::RightButton) // 右键按下
    {
        mineArea->setFlag(x, y);
    }

    if (e->button() == Qt::LeftButton)
    {
        mineArea->checkMine(x, y);
    }

    mineArea->isWin();
}
