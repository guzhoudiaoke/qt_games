/** llkanarea.cpp
  * guzhoudiaoke@126.com
  * 2011-08-05
  */

#include "llkanarea.h"
#include <time.h>
#include <stdlib.h>

LLKanArea::LLKanArea(QWidget *)
{
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::gray);
    setPalette(p);
    setAutoFillBackground(true);

    img = new QImage(":/images/img.bmp");
    frame = new QImage(":/images/3DFrames.bmp");

    setMinimumSize(MAP_COL*FRAME_WIDTH + 1, MAP_ROW*FRAME_HEIGHT + 1);
    setMaximumSize(MAP_COL*FRAME_WIDTH + 1, MAP_ROW*FRAME_HEIGHT + 1);

    initGame();
}

void LLKanArea::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    int i, j;

    // 绘制边框和图案
    for (i = 0; i < MAP_ROW; i++)
    {
        for (j = 0; j < MAP_COL; j++)
        {
            if (map[i][j] != BLANK_STATE)
            {
                QRect cell_rect = QRect(QPoint(FRAME_WIDTH*j, FRAME_HEIGHT*i), QSize(CELL_WIDTH, CELL_HEIGHT));
                QRect img_rect = QRect(QPoint(0, 4*CELL_HEIGHT), QSize(CELL_WIDTH, CELL_HEIGHT));
                // 绘制3D 边框
                p.drawImage(cell_rect, *frame, img_rect);

                // 绘制图案
                cell_rect = QRect(QPoint(FRAME_WIDTH*j+1, FRAME_HEIGHT*i+6), QSize(IMG_WIDTH, IMG_HEIGHT));
                img_rect = QRect(QPoint(0, map[i][j]*(IMG_HEIGHT+12)), QSize(IMG_WIDTH, IMG_HEIGHT));
                p.drawImage(cell_rect, *img, img_rect);
            }
        }
    }

    // 绘制选中框
    if (is_selected)
    {
        QPen pen = QPen(QColor(Qt::red));
        pen.setWidth(2);
        p.setPen(pen);
        QRect select_rect = QRect(QPoint(FRAME_WIDTH*select_col, FRAME_HEIGHT*select_row),
                                  QSize(FRAME_WIDTH, FRAME_HEIGHT));
        p.drawRect(select_rect);
    }

    unsigned int index;
    // 绘制连接线
    if (is_link)
    {
        QPen pen = QPen(QColor(Qt::green));
        pen.setWidth(2);
        p.setPen(pen);
        for (index = 0; index < link_lines.size(); index++)
        {
            LinkLine line = link_lines.at(index);
            QPoint start, end;
            start = QPoint(line.start_col*FRAME_WIDTH + FRAME_WIDTH/2,
                           line.start_row*FRAME_HEIGHT + FRAME_HEIGHT/2);
            end = QPoint(line.end_col*FRAME_WIDTH + FRAME_WIDTH/2,
                         line.end_row*FRAME_HEIGHT + FRAME_HEIGHT/2);
            p.drawLine(start, end);
        }
    }
}

void LLKanArea::initGame()
{
    int i, j;
    // 初始化全部为空白方块
    for (i = 0; i < MAP_ROW; i++)
    {
        for (j = 0; j < MAP_COL; j++)
        {
            map[i][j] = BLANK_STATE;
        }
    }

    qsrand((unsigned int)time(NULL));

    vector<int> vec;
    for (i = 0; i < (MAP_COL-2)*(MAP_ROW-2)/4; i++)
        for (j = 0; j < 4; j++)
            vec.push_back(i);

    for (i = 1; i < MAP_ROW-1; i++)
        for (j = 1; j < MAP_COL-1; j++)
        {
            int index = (int(qrand()*0.01 + qrand()*0.1 + qrand())) % vec.size();
            map[i][j] = vec.at(index);
            vector<int>::iterator iter = vec.begin() + index;
            vec.erase(iter);
        }

    is_link = false;
    is_selected = false;

    select_row = -1;
    select_col = -1;

    gameover = false;

    update();

    time_left = MAX_TIME;
    startTimer(1000 );
}

void LLKanArea::lbuttonDownAt(int x, int y)
{
    if (gameover)
        return;

    bool b_sound = false;
    if (x >= FRAME_WIDTH && x < FRAME_WIDTH*(MAP_COL-1) && y >= FRAME_HEIGHT && y < FRAME_HEIGHT*(MAP_ROW-1))
    {
        int row = y / FRAME_HEIGHT;
        int col = x / FRAME_WIDTH;

        if (is_selected)        // 已经选中一个方� �
        {
            if ((row != select_row || col != select_col) &&     // 点的不是已选中的方� �
                map[row][col] == map[select_row][select_col])   // 点的与选中的是同一种图案
            {
                if (isLink(row, col, select_row, select_col))   // 已选中的与新选中的可以连接
                {
                    // 将两个选中的方块清除
                    is_link = true;
                    update();

                    QSound::play("./sounds/PLACE.WAV");
                    b_sound = true;

                    QElapsedTimer t;
                    t.start();
                    while (t.elapsed() < 300)
                            QCoreApplication::processEvents();

                    //QTest::qWait(300);
                    map[row][col] = BLANK_STATE;
                    map[select_row][select_col] = BLANK_STATE;

                    is_link = false;
                    link_lines.clear();
                    update();

                    time_left += 5;
                    if (time_left > MAX_TIME)
                        time_left = MAX_TIME;
                }
            }
            select_row = -1;
            select_col = -1;
            is_selected = false;
        }
        else                    // 还没选中一个方� �
        {
            if (map[row][col] != BLANK_STATE)
            {
                select_row = row;
                select_col = col;
                is_selected = true;
            }
        }

        update();
    }

    if (!b_sound)
        QSound::play("./sounds/push.WAV");

    if (isWin())
    {
        gameover = true;
        killTimer(1);
        QMessageBox::information(this, tr("提示"), tr("恭喜，� 获胜了！"));
        QSound::play("./sounds/WAV2.WAV");
    }
}

bool LLKanArea::isLink(int row, int col, int select_row, int select_col)
{
    // 列直连方式
    if (col == select_col)
    {
        if (isColLink(row, col, select_row, select_col))
            return true;
    }

    // 行直连方式
    else if (row == select_row)
    {
        if (isRowLink(row, col, select_row, select_col))
            return true;
    }

    // 一个折点连接方式
    if (isOneCornerLink(row, col, select_row, select_col))
        return true;

    // 两个折点连接方式
    else if(isTwoCornerLink(row, col, select_row, select_col))
        return true;

    return false;
}

bool LLKanArea::isRowLink(int row, int col, int select_row, int select_col)
{
    int min_col, max_col;
    if (col > select_col)
    {
        min_col = select_col;
        max_col = col;
    }
    else
    {
        min_col = col;
        max_col = select_col;
    }

    int i;
    for (i = min_col+1; i <= max_col; i++)
    {
        if (i == max_col)
        {
            LinkLine line;
            line.start_row = row;
            line.start_col = col;
            line.end_row = select_row;
            line.end_col = select_col;

            link_lines.push_back(line);

            return true;
        }
        if (map[row][i] != BLANK_STATE)
            break;
    }

    return false;
}

bool LLKanArea::isColLink(int row, int col, int select_row, int select_col)
{
    int min_row, max_row;
    if (row > select_row)
    {
        min_row = select_row;
        max_row = row;
    }
    else
    {
        min_row = row;
        max_row = select_row;
    }

    int i;
    for (i = min_row+1; i <= max_row; i++)
    {
        if (i == max_row)
        {
            LinkLine line;
            line.start_row = row;
            line.start_col = col;
            line.end_row = select_row;
            line.end_col = select_col;
            link_lines.push_back(line);

            return true;
        }
        if (map[i][col] != BLANK_STATE)
            break;
    }

    return false;
}

bool LLKanArea::isRow(int v_row, int v_col, int, int r_col)
{
    int i;
    if (v_col > r_col)
    {
        for (i = v_col; i >= r_col; i--)
        {
            if (i == r_col)
                return true;
            if (map[v_row][i] != BLANK_STATE)
                break;
        }
    }
    else if(v_col < r_col)
    {
        for (i = v_col; i <= r_col; i++)
        {
            if (i == r_col)
                return true;
            if (map[v_row][i] != BLANK_STATE)
                break;
        }
    }

    return false;
}

bool LLKanArea::isCol(int v_row, int v_col, int r_row, int)
{
    int i;
    if (v_row > r_row)
    {
        for (i = v_row; i >= r_row; i--)
        {
            if (i == r_row)
                return true;
            if (map[i][v_col] != BLANK_STATE)
                break;
        }
    }
    else if (v_row < r_row)
    {
        for (i = v_row; i <= r_row; i++)
        {
            if (i == r_row)
                return true;
            if (map[i][v_col] != BLANK_STATE)
                break;
        }
    }

    return false;
}

bool LLKanArea::isOneCornerLink(int row, int col, int select_row, int select_col)
{
    int left_row, left_col, right_row, right_col;
    if (col > select_col)
    {
        left_row = select_row;
        left_col = select_col;
        right_row = row;
        right_col = col;
    }
    else
    {
        left_row = row;
        left_col = col;
        right_row = select_row;
        right_col = select_col;
    }

    if (left_row < right_row)   // 左上，右下
    {
        if (isCol(right_row, left_col, left_row, left_col) &&
            isRow(right_row, left_col, right_row, right_col))
        {
            LinkLine line;
            line.start_row = left_row;
            line.start_col = left_col;
            line.end_row = right_row;
            line.end_col = left_col;
            link_lines.push_back(line);

            line.start_row = right_row;
            line.start_col = right_col;
            line.end_row = right_row;
            line.end_col = left_col;
            link_lines.push_back(line);

            return true;
        }
        if (isRow(left_row, right_col, left_row, left_col) &&
            isCol(left_row, right_col, right_row, right_col))
        {
            LinkLine line;
            line.start_row = left_row;
            line.start_col = left_col;
            line.end_row = left_row;
            line.end_col = right_col;
            link_lines.push_back(line);

            line.start_row = right_row;
            line.start_col = right_col;
            line.end_row = left_row;
            line.end_col = right_col;
            link_lines.push_back(line);

            return true;
        }
    }
    else                        // 左下，右上
    {
        if (isCol(right_row, left_col, left_row, left_col) &&
            isRow(right_row, left_col, right_row, right_col))
        {
            LinkLine line;
            line.start_row = right_row;
            line.start_col = left_col;
            line.end_row = left_row;
            line.end_col = left_col;
            link_lines.push_back(line);

            line.start_row = right_row;
            line.start_col = left_col;
            line.end_row = right_row;
            line.end_col = right_col;
            link_lines.push_back(line);

            return true;
        }
        if (isRow(left_row, right_col, left_row, left_col) &&
            isCol(left_row, right_col, right_row, right_col))
        {
            LinkLine line;
            line.start_row = left_row;
            line.start_col = right_col;
            line.end_row = left_row;
            line.end_col = left_col;
            link_lines.push_back(line);

            line.start_row = left_row;
            line.start_col = right_col;
            line.end_row = right_row;
            line.end_col = right_col;
            link_lines.push_back(line);

            return true;
        }
    }

    return false;
}

bool LLKanArea::isTwoCornerLink(int row, int col, int select_row, int select_col)
{
    int i;
    // 上
    for (i = row-1; i >= 0; i--)
    {
        if (map[i][col] != BLANK_STATE)
            break;
        if (isOneCornerLink(i, col, select_row, select_col))
        {
            LinkLine line;
            line.start_row = row;
            line.start_col = col;
            line.end_row = i;
            line.end_col = col;
            link_lines.push_back(line);

            return true;
        }
    }

    // 下
    for (i = row+1; i < MAP_ROW; i++)
    {
        if (map[i][col] != BLANK_STATE)
            break;
        if (isOneCornerLink(i, col, select_row, select_col))
        {
            LinkLine line;
            line.start_row = row;
            line.start_col = col;
            line.end_row = i;
            line.end_col = col;
            link_lines.push_back(line);

            return true;
        }
    }

    // 左
    for (i = col-1; i >= 0; i--)
    {
        if (map[row][i] != BLANK_STATE)
            break;
        if (isOneCornerLink(row, i, select_row, select_col))
        {
            LinkLine line;
            line.start_row = row;
            line.start_col = col;
            line.end_row = row;
            line.end_col = i;
            link_lines.push_back(line);

            return true;
        }
    }

    // 右
    for (i = col+1; i < MAP_COL; i++)
    {
        if (map[row][i] != BLANK_STATE)
            break;
        if (isOneCornerLink(row, i, select_row, select_col))
        {
            LinkLine line;
            line.start_row = row;
            line.start_col = col;
            line.end_row = row;
            line.end_col = i;
            link_lines.push_back(line);

            return true;
        }
    }

    return false;
}

bool LLKanArea::isWin()
{
    int i, j;
    for (i = 0; i < MAP_ROW; i++)
    {
        for (j = 0; j < MAP_COL; j++)
        {
            if (map[i][j] != BLANK_STATE)
                return false;
        }
    }

    return true;
}

void LLKanArea::timerEvent(QTimerEvent *)
{
    time_left--;
    if (time_left <= 0)
    {
        gameover = true;
        killTimer(1);
        QMessageBox::information(this, tr("提示"), tr("时间到!"));
    }
}

int LLKanArea::getTimeLeft()
{
    return time_left;
}
