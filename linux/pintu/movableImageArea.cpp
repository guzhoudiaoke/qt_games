/** movableImageArea.cpp
  * 孤舟钓客
  * 2011-08-01
  */

#include "movableImageArea.h"
#include <time.h>
#include <stdlib.h>

MovableImageArea::MovableImageArea(QWidget *)
{
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::white);
    setPalette(p);
    setAutoFillBackground(true);

    img = new QImage(":/images/cat.jpg");
    h_nums = 4;
    v_nums = 4;

    img_width = img->width();
    img_height = img->height();
    cell_width = img_width / h_nums;
    cell_height = img_height / v_nums;

    black_pos.row = h_nums-1;
    black_pos.col = v_nums-1;

    setMinimumSize(img_width+1, img_height+1);
    setMaximumSize(img_width+1, img_height+1);

    initGame();
    update();
}


void MovableImageArea::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    QPoint start_point, end_point;
    int i, j;

    // 绘制每块的图像
    for (i = 0; i < v_nums; i++)
        for (j = 0; j < h_nums; j++)
        {
            QPoint cell_topleft = QPoint(cell_width*j, cell_height*i);

            QRect img_rect = QRect(QPoint(img_map[i][j].col*cell_width, img_map[i][j].row*cell_height),
                                   QSize(cell_width, cell_height));
            p.drawImage(cell_topleft, *img, img_rect);
        }

    // 绘制空白方块
    if (!iswin)
    {
        QBrush brush;
        brush.setColor(QColor(12, 34, 56));
        brush.setStyle(Qt::SolidPattern);
        QRect black_rect = QRect(QPoint(black_pos.col*cell_width, black_pos.row*cell_height),
                                 QSize(cell_width, cell_height));
        p.fillRect(black_rect, brush);
    }

    // 绘制横向直线
    for (i = 0; i <= v_nums; i++)
    {
        start_point = QPoint(0, cell_height*i);
        end_point = QPoint(img_width, cell_height*i);
        p.drawLine(start_point, end_point);
    }

    // 绘制纵向直线
    for (j = 0; j <= h_nums; j++)
    {
        start_point = QPoint(cell_width*j, 0);
        end_point = QPoint(cell_width*j, img_width);
        p.drawLine(start_point, end_point);
    }
}

void MovableImageArea::imgMove(int key)
{
    int row = black_pos.row;    // 空白块所在行
    int col = black_pos.col;    // 空白块所在列

    pos temp_pos;
    temp_pos.row = img_map[row][col].row;
    temp_pos.col = img_map[row][col].col;

    switch (key)
    {
    case Qt::Key_Left:
        if (col < h_nums-1)     // 空白块不在最右边
        {
            img_map[row][col].row = img_map[row][col+1].row;
            img_map[row][col].col = img_map[row][col+1].col;

            img_map[row][col+1].row = temp_pos.row;
            img_map[row][col+1].col = temp_pos.col;

            black_pos.col += 1;
        }
        break;
    case Qt::Key_Right:
        if (col > 0)            // 空白块不在最左边
        {
            img_map[row][col].row = img_map[row][col-1].row;
            img_map[row][col].col = img_map[row][col-1].col;

            img_map[row][col-1].row = temp_pos.row;
            img_map[row][col-1].col = temp_pos.col;

            black_pos.col -= 1;
        }
        break;
    case Qt::Key_Up:
        if (row < v_nums-1)
        {
            img_map[row][col].row = img_map[row+1][col].row;
            img_map[row][col].col = img_map[row+1][col].col;

            img_map[row+1][col].row = temp_pos.row;
            img_map[row+1][col].col = temp_pos.col;

            black_pos.row += 1;
        }
        break;
    case Qt::Key_Down:
        if (row > 0)
        {
            img_map[row][col].row = img_map[row-1][col].row;
            img_map[row][col].col = img_map[row-1][col].col;

            img_map[row-1][col].row = temp_pos.row;
            img_map[row-1][col].col = temp_pos.col;

            black_pos.row -= 1;
        }
        break;
    default:
        break;
    }
    update();
}

bool MovableImageArea::isWin()
{
    int i, j;
    for (i = 0; i < v_nums; i++)
        for (j = 0; j < h_nums; j++)
        {
            if (img_map[i][j].row != i || img_map[i][j].col != j)
                return false;
        }

    iswin = true;
    black_pos.row = -1;
    black_pos.row = -1;
    return true;
}

void MovableImageArea::initGame()
{
    int i, j;
    for (i = 0; i < h_nums; i++)
        for (j = 0; j < v_nums; j++)
        {
            img_map[i][j].row = i;
            img_map[i][j].col = j;
        }

    qsrand(time(NULL));

    for (i = 0; i < 300; i++)
    {
        int num = qrand() % 4;
        switch (num)
        {
        case 0:
            imgMove(Qt::Key_Left);
            break;
        case 1:
            imgMove(Qt::Key_Right);
            break;
        case 2:
            imgMove(Qt::Key_Up);
            break;
        case 3:
            imgMove(Qt::Key_Down);
            break;
        }
    }

    iswin = false;
}

int MovableImageArea::getCellWidth()
{
    return cell_width;
}

int MovableImageArea::getCellHeight()
{
    return cell_height;
}

int MovableImageArea::getImgWidth()
{
    return img_width;
}

int MovableImageArea::getImgHeight()
{
    return img_height;
}

MovableImageArea::pos MovableImageArea::getBlackPos()
{
    return black_pos;
}
