/** maparea.cpp
  * guzhoudiaoke@126.com
  * 2011-08-08
  */

#include "maparea.h"
#include <time.h>
#include <stdlib.h>

MapArea::MapArea(QWidget *)
{
    QPalette p = palette();
    p.setColor(QPalette::Window, Qt::white);
    setPalette(p);
    setAutoFillBackground(true);

    img_snake1 = new QImage("./images/snake1.bmp");
    img_snake2 = new QImage("./images/snake2.bmp");
    img_wall = new QImage("./images/wall.bmp");

    setMinimumSize(MAP_COL*CELL_WIDTH, MAP_ROW*CELL_WIDTH);
    setMaximumSize(MAP_COL*CELL_WIDTH, MAP_ROW*CELL_WIDTH);

    initGame();
    startGame();
}

void MapArea::paintEvent(QPaintEvent *)
{
    QPainter p(this);

    int i, j;
    QRect cell_rect;
    QRect img_rect;
    for (i = 0; i < MAP_ROW; i++)
    {
        for (j = 0; j < MAP_COL; j++)
        {
            if (map[i][j] == NOTHING)
                continue;

            cell_rect = QRect(QPoint(CELL_WIDTH*j, CELL_WIDTH*i),
                                    QSize(CELL_WIDTH, CELL_WIDTH));
            img_rect = QRect(QPoint(0, 0), QSize(CELL_WIDTH, CELL_WIDTH));

            if (map[i][j] == WALL)
                p.drawImage(cell_rect, *img_wall, img_rect);
            else if (map[i][j] == PRIZON)
            {
                img_rect = QRect(QPoint(0, 0), QSize(CELL_WIDTH, CELL_WIDTH));
                p.drawImage(cell_rect, *img_snake1, img_rect);
            }
            else if (map[i][j] == FRUIT)
            {
                img_rect = QRect(QPoint(CELL_WIDTH, 0), QSize(CELL_WIDTH, CELL_WIDTH));
                p.drawImage(cell_rect, *img_snake1, img_rect);
            }
        }
    }

    if (!snake1.isDie())
        drawSnake(&snake1, *img_snake1);
    if (!snake2.isDie())
        drawSnake(&snake2, *img_snake2);
}

void MapArea::drawSnake(Snake *snake, QImage img)
{
    QRect cell_rect;
    QRect img_rect;

    int snake_length = snake->getSnakeLength();
    QPainter p(this);

    // 画蛇
    int img_row, img_col;

    // 蛇头
    ImgState state = (snake->getImageState())[0];   // snake_img_state[0];
    if (state == IMG_UP_UP)
    {
        img_row = 1;
        img_col = 0;
    }
    else if (state == IMG_DOWN_DOWN)
    {
        img_row = 1;
        img_col = 3;
    }
    else if (state == IMG_LEFT_LEFT)
    {
        img_row = 1;
        img_col = 1;
    }
    else if (state == IMG_RIGHT_RIGHT)
    {
        img_row = 1;
        img_col = 2;
    }

    SPoint *snake_pos = snake->getSnakePos();
    cell_rect = QRect(QPoint(CELL_WIDTH*snake_pos[0].x, CELL_WIDTH*snake_pos[0].y),
                      QSize(CELL_WIDTH, CELL_WIDTH));
    img_rect = QRect(QPoint(img_col*CELL_WIDTH, img_row*CELL_WIDTH), QSize(CELL_WIDTH, CELL_WIDTH));
    p.drawImage(cell_rect, img, img_rect);

    // 蛇身
    int i;
    for (i = 1; i <= snake_length; i++)
    {
        state = (snake->getImageState())[i];
        switch(state)
        {
        case IMG_UP_UP:
        case IMG_DOWN_DOWN:
            img_row = 2;
            img_col = 0;
            break;
        case IMG_LEFT_LEFT:
        case IMG_RIGHT_RIGHT:
            img_row = 2;
            img_col = 1;
            break;
        case IMG_RIGHT_DOWN:
        case IMG_UP_LEFT:
            img_row = 3;
            img_col = 1;
            break;
        case IMG_LEFT_DOWN:
        case IMG_UP_RIGHT:
            img_row = 3;
            img_col = 0;
            break;
        case IMG_RIGHT_UP:
        case IMG_DOWN_LEFT:
            img_row = 2;
            img_col = 3;
            break;
        case IMG_LEFT_UP:
        case IMG_DOWN_RIGHT:
            img_row = 2;
            img_col = 2;
            break;
        default:
            break;
        }

        cell_rect = QRect(QPoint(CELL_WIDTH*snake_pos[i].x, CELL_WIDTH*snake_pos[i].y),
                          QSize(CELL_WIDTH, CELL_WIDTH));
        img_rect = QRect(QPoint(img_col*CELL_WIDTH, img_row*CELL_WIDTH),
                         QSize(CELL_WIDTH, CELL_WIDTH));
        p.drawImage(cell_rect, img, img_rect);
    }

    // 蛇尾
    state = (snake->getImageState())[snake_length+1];
    if (state == IMG_UP_UP)
    {
        img_row = 3;
        img_col = 2;
    }
    else if (state == IMG_DOWN_DOWN)
    {
        img_row = 0;
        img_col = 2;
    }
    else if (state == IMG_LEFT_LEFT)
    {
        img_row = 3;
        img_col = 3;
    }
    else if (state == IMG_RIGHT_RIGHT)
    {
        img_row = 0;
        img_col = 3;
    }

    cell_rect = QRect(QPoint(CELL_WIDTH*snake_pos[snake_length+1].x, CELL_WIDTH*snake_pos[snake_length+1].y),
                      QSize(CELL_WIDTH, CELL_WIDTH));
    img_rect = QRect(QPoint(img_col*CELL_WIDTH, img_row*CELL_WIDTH), QSize(CELL_WIDTH, CELL_WIDTH));
    p.drawImage(cell_rect, img, img_rect);
}

void MapArea::initGame()
{
    int i, j;
    for (i = 0; i < MAP_ROW; i++)
    {
        for (j = 0; j < MAP_COL; j++)
        {
            if (i == HOLE_ROW)
                map[i][j] = NOTHING;
            else if (i < EDGE_WIDTH || i >= MAP_ROW-EDGE_WIDTH ||
                j < EDGE_WIDTH || j >= MAP_COL-EDGE_WIDTH)
                map[i][j] = WALL;
            else
                map[i][j] = NOTHING;
        }
    }

    qsrand((unsigned int)time(NULL));
    i = qrand() % (MAP_ROW-2*EDGE_WIDTH) + EDGE_WIDTH;
    j = qrand() % (MAP_COL-2*EDGE_WIDTH) + EDGE_WIDTH;
    map[i][j] = FRUIT;

    i = qrand() % (MAP_ROW-2*EDGE_WIDTH) + EDGE_WIDTH;
    j = qrand() % (MAP_COL-2*EDGE_WIDTH) + EDGE_WIDTH;
    map[i][j] = PRIZON;

    int x;
    for (x = 0; x < 30; x++)
    {
        i = qrand() % (MAP_ROW-2*EDGE_WIDTH) + EDGE_WIDTH;
        j = qrand() % (MAP_COL-2*EDGE_WIDTH) + EDGE_WIDTH;
        if (map[i][j] == NOTHING)
            map[i][j] = WALL;
    }

    gameover = false;
    has_hole1 = true;
    has_hole2 = true;
}

void MapArea::startGame()
{
    SPoint p;
    p.x = 0;
    p.y = HOLE_ROW;
    snake1.initSnake(p, 3);
    snake1.changeDirect(MOVE_RIGHT);

    p.x = MAP_COL-1;
    p.y = HOLE_ROW;
    snake2.initSnake(p, 3);
    snake2.changeDirect(MOVE_LEFT);

    timer_no = startTimer(300);
}

void MapArea::timerEvent(QTimerEvent *)
{
    SPoint *snake1_pos;
    SPoint *snake2_pos;
    int snake1_length;
    int snake2_length;
    int i;

    if (!snake1.isDie())
    {
        snake1.move();

        if (snake1.isHeadTouchBody())
        {
            snake1.die();
            return;
        }

        snake1_pos = snake1.getSnakePos();
        snake1_length = snake1.getSnakeLength();
        switch (map[snake1_pos[0].y][snake1_pos[0].x])
        {
        case WALL:
            snake1.die();
            return;
        case PRIZON:
            clearPrizon(snake1_pos[0].y, snake1_pos[0].x);
            snake1.die();
            update();
            break;
        case FRUIT:
            clearFruit(snake1_pos[0].y, snake1_pos[0].x);
            snake1.addBody();
            addFruit();
            addPrizon();
            break;
        }

        if (has_hole1 && snake1_pos[snake1_length+1].x > EDGE_WIDTH)
        {
            has_hole1 = false;
            map[HOLE_ROW][HOLE_COL] = WALL;
            map[HOLE_ROW][HOLE_COL+1] = WALL;
        }
    }
    else
    {
        if (has_hole1)
        {
            has_hole1 = false;
            map[HOLE_ROW][HOLE_COL] = WALL;
            map[HOLE_ROW][HOLE_COL+1] = WALL;
        }
    }

    if (!snake2.isDie())
    {
        snake2.move();

        if (snake2.isHeadTouchBody())
        {
            snake2.die();
            return;
        }

        snake2_pos = snake2.getSnakePos();
        snake2_length = snake2.getSnakeLength();
        switch (map[snake2_pos[0].y][snake2_pos[0].x])
        {
        case WALL:
            snake2.die();
            return;
        case PRIZON:
            clearPrizon(snake2_pos[0].y, snake2_pos[0].x);
            snake2.die();
            update();
            break;
        case FRUIT:
            clearFruit(snake2_pos[0].y, snake2_pos[0].x);
            snake2.addBody();
            addFruit();
            addPrizon();
            break;
        }

        if (has_hole2 && snake2_pos[snake2_length+1].x < MAP_COL-HOLE_COL)
        {
            has_hole2 = false;
            map[HOLE_ROW][MAP_COL-1] = WALL;
            map[HOLE_ROW][MAP_COL-2] = WALL;
        }
    }
    else
    {
        if (has_hole2)
        {
            has_hole2 = false;
            map[HOLE_ROW][MAP_COL-1] = WALL;
            map[HOLE_ROW][MAP_COL-2] = WALL;
        }
    }

    if ((!snake1.isDie()) && (!snake2.isDie()))
    {
        snake1_pos = snake1.getSnakePos();
        snake1_length = snake1.getSnakeLength();
        snake2_pos = snake2.getSnakePos();
        snake2_length = snake2.getSnakeLength();

        for (i = 1; i < snake2_length+2; i++)
        {
            if (snake2_pos[i].x == snake1_pos[0].x && snake2_pos[i].y == snake1_pos[0].y)
            {
                snake1.die();
                snake2.die();
                return;
            }
        }

        for (i = 1; i < snake1_length+2; i++)
        {
            if (snake1_pos[i].x == snake2_pos[0].x && snake1_pos[i].y == snake2_pos[0].y)
            {
                snake1.die();
                snake2.die();
                return;
            }
        }
    }

    update();

    if (snake1.isDie() && snake2.isDie())
    {
        gameover = true;
        killTimer(timer_no);
        QMessageBox::information(this, tr("提示"), tr("游戏结束"));
    }
}

void MapArea::changeDirect(int key)
{
    switch (key)
    {
    case Qt::Key_Up:
        snake2.changeDirect(MOVE_UP);
        break;
    case Qt::Key_Down:
        snake2.changeDirect(MOVE_DOWN);
        break;
    case Qt::Key_Left:
        snake2.changeDirect(MOVE_LEFT);
        break;
    case Qt::Key_Right:
        snake2.changeDirect(MOVE_RIGHT);
        break;

    case Qt::Key_W:
        snake1.changeDirect(MOVE_UP);
        break;
    case Qt::Key_S:
        snake1.changeDirect(MOVE_DOWN);
        break;
    case Qt::Key_A:
        snake1.changeDirect(MOVE_LEFT);
        break;
    case Qt::Key_D:
        snake1.changeDirect(MOVE_RIGHT);
        break;
    default:
        break;
    }

    update();
}

bool MapArea::isGameover()
{
    return gameover;
}

void MapArea::clearFruit(int row, int col)
{
    if (map[row][col] == FRUIT)
        map[row][col] = NOTHING;
}

void MapArea::clearPrizon(int row, int col)
{
    if (map[row][col] == PRIZON)
        map[row][col] = NOTHING;
}

void MapArea::addFruit()
{
    qsrand((unsigned int)time(NULL));

    int i, j;

    bool ok = true;
    do {
        i = qrand() % (MAP_ROW-2*EDGE_WIDTH) + EDGE_WIDTH;
        j = qrand() % (MAP_COL-2*EDGE_WIDTH) + EDGE_WIDTH;

        ok = map[i][j] != NOTHING;
        if (!snake1.isDie())
            ok = ok || isAtSnake(i, j, &snake1);
        if (!snake2.isDie())
            ok = ok || isAtSnake(i, j, &snake2);
    } while (ok);

    map[i][j] = FRUIT;
}

void MapArea::addPrizon()
{
    qsrand((unsigned int)time(NULL));

    int i, j;

    bool ok = true;
    do {
        i = qrand() % (MAP_ROW-2*EDGE_WIDTH) + EDGE_WIDTH;
        j = qrand() % (MAP_COL-2*EDGE_WIDTH) + EDGE_WIDTH;

        ok = map[i][j] != NOTHING;
        if (!snake1.isDie())
            ok = ok || isAtSnake(i, j, &snake1);
        if (!snake2.isDie())
            ok = ok || isAtSnake(i, j, &snake2);
    } while (ok);

    map[i][j] = PRIZON;
}

bool MapArea::isAtSnake(int row, int col, Snake *snake)
{
    SPoint *snake_pos = snake->getSnakePos();
    int snake_length = snake->getSnakeLength();
    int i;
    for (i = 0; i < snake_length+2; i++)
        if (snake_pos[i].y == row && snake_pos[i].x == col)
            return true;

    return false;
}
