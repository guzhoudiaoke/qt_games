/** mainwdiget.cpp
  * 孤舟钓客
  * 2011-08-01
  */

#include "mainwidget.h"

MainWidget::MainWidget(QWidget *parent)
    : QWidget(parent)
{
    QFont font("ZYSong18030",12);
    setFont(font);

    movableImageArea = new MovableImageArea;
    staticImageArea = new StaticImageArea;

    // 布局
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(movableImageArea);
    mainLayout->addWidget(staticImageArea);
    mainLayout->setMargin(10);
    mainLayout->setSpacing(10);

    setLayout(mainLayout);

    setMinimumSize(movableImageArea->getImgWidth()*2 + 30, movableImageArea->getImgHeight() + 20);
    setMaximumSize(movableImageArea->getImgWidth()*2 + 30, movableImageArea->getImgHeight() + 20);

    setWindowTitle(tr("拼图游戏"));
}

void MainWidget::keyPressEvent(QKeyEvent *e)
{
    if (movableImageArea->isWin())
        return;
    movableImageArea->imgMove(e->key());
    if (movableImageArea->isWin())
    {
        QMessageBox::information(this, tr("提示"), tr("恭喜，你获胜了！"));
    }
}

void MainWidget::createMenus()
{
    /*hardMenu = menuBar()->addMenu(tr("难度"));
    imgMenu = menuBar()->addMenu(tr("图像"));
    aboutMenu = menuBar()->addMenu(tr("关于"));

    hardMenu->addAction(hard3x3Action);
    hardMenu->addAction(hard4x4Action);
    hardMenu->addAction(hard5x5Action);*/
}

void MainWidget::mousePressEvent(QMouseEvent *e)
{
    if (movableImageArea->isWin())
        return;
    if (e->button() == Qt::LeftButton)
    {
        int x = e->x() - 10;
        int y = e->y() - 10;
        int row = y / movableImageArea->getCellHeight();
        int col = x / movableImageArea->getCellWidth();
        int black_row = movableImageArea->getBlackPos().row;
        int black_col = movableImageArea->getBlackPos().col;

        if (abs(row-black_row) + abs(col-black_col) == 1)
        {
            if (row-black_row == 1)   // 点击空白方块上方块
                movableImageArea->imgMove(Qt::Key_Up);
            if (row-black_row == -1)
                movableImageArea->imgMove(Qt::Key_Down);
            if (col-black_col == 1)
                movableImageArea->imgMove(Qt::Key_Left);
            if (col-black_col == -1)
                movableImageArea->imgMove(Qt::Key_Right);
        }
    }

    if (movableImageArea->isWin())
    {
        QMessageBox::information(this, tr("提示"), tr("恭喜，你获胜了！"));
    }
}

void MainWidget::mouseMoveEvent(QMouseEvent *)
{

}
