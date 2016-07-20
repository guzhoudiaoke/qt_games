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

    img_snake = new QImage("./images/snake.bmp");
    img_wall = new QImage("./images/wall.bmp");

    setMinimumSize(MAP_COL*CELL_WIDTH, MAP_ROW*CELL_WIDTH);
    setMaximumSize(MAP_COL*CELL_WIDTH, MAP_ROW*CELL_WIDTH);

    initGame();
    startGame();
}

MapArea::~MapArea()
{
    delete []snake_pos;
    snake_pos = NULL;
    delete []snake_old.body;
    snake_old.body = NULL;
    delete []snake_new.body;
    snake_new.body = NULL;
    delete []snake_img_state;
    snake_img_state = NULL;
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
                p.drawImage(cell_rect, *img_snake, img_rect);
            }
            else if (map[i][j] == FRUIT)
            {
                img_rect = QRect(QPoint(CELL_WIDTH, 0), QSize(CELL_WIDTH, CELL_WIDTH));
                p.drawImage(cell_rect, *img_snake, img_rect);
            }
        }
    }

    // 画蛇
    int img_row, img_col;

    // 蛇头
    ImgState state = snake_img_state[0];
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

    cell_rect = QRect(QPoint(CELL_WIDTH*snake_pos[0].x, CELL_WIDTH*snake_pos[0].y),
                      QSize(CELL_WIDTH, CELL_WIDTH));
    img_rect = QRect(QPoint(img_col*CELL_WIDTH, img_row*CELL_WIDTH), QSize(CELL_WIDTH, CELL_WIDTH));
    p.drawImage(cell_rect, *img_snake, img_rect);

    // 蛇身
    for (i = 1; i <= snake_length; i++)
    {
        state = snake_img_state[i];
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
        p.drawImage(cell_rect, *img_snake, img_rect);
    }

    // 蛇尾
    state = snake_img_state[snake_length+1];
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
    p.drawImage(cell_rect, *img_snake, img_rect);
}

void MapArea::initGame()
{
    int i, j;
    for (i = 0; i < MAP_ROW; i++)
    {
        for (j = 0; j < MAP_COL; j++)
        {
            if (i == HOLE_ROW && (j == HOLE_COL || j == HOLE_COL+1))
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

    // 蛇的长度
    snake_length = 4;

    // 蛇的位置坐标
    snake_pos = new SPoint[snake_length+2];
    snake_pos[0].x = 0;
    snake_pos[0].y = HOLE_ROW;
    for (i = 1; i < snake_length+2; i++)
    {
        snake_pos[i].x = -1;
        snake_pos[i].y = -1;
    }

    // 蛇图片状态
    snake_img_state = new ImgState[snake_length+2];
    for (i = 0; i < snake_length+2; i++)
    {
        snake_img_state[i] = IMG_NONE;
    }

    // 蛇的运动状态
    snake_new.head = MOVE_NONE;
    snake_old.head = MOVE_NONE;
    snake_new.body = new MoveState[snake_length];
    snake_old.body = new MoveState[snake_length];
    for (i = 0; i < snake_length; i++)
    {
        snake_new.body[i] = MOVE_NONE;
        snake_old.body[i] = MOVE_NONE;
    }
    snake_new.tail = MOVE_NONE;
    snake_old.tail = MOVE_NONE;

    gameover = false;
    snake_dir = MOVE_NONE;
    has_hole = true;
}

void MapArea::startGame()
{
    changeDirect(MOVE_RIGHT);
    timer_no = startTimer(300);
}

void MapArea::timerEvent(QTimerEvent *)
{
    snakeMove();
    if (isHeadTouchBody())
    {
        gameover = true;
        killTimer(timer_no);
        QMessageBox::information(this, tr("提示"), tr("游戏结束!"));
        return;
    }

    switch (map[snake_pos[0].y][snake_pos[0].x])
    {
    case WALL:
        gameover = true;
        killTimer(timer_no);
        QMessageBox::information(this, tr("提示"), tr("游戏结束!"));
        return;
    case PRIZON:
        gameover = true;
        killTimer(timer_no);
        update();
        QMessageBox::information(this, tr("提示"), tr("游戏结束!"));
        break;
    case FRUIT:
        clearFruit(snake_pos[0].y, snake_pos[0].x);
        snakeAddBody();
        addFruit();
        addPrizon();
        break;
    }

    if (has_hole && snake_pos[snake_length+1].x >= EDGE_WIDTH)
    {
        has_hole = false;
        map[HOLE_ROW][HOLE_COL] = WALL;
        map[HOLE_ROW][HOLE_COL+1] = WALL;
    }

    update();
}

void MapArea::snakeMove()
{
    snake_new.head = snake_dir;

    int i;
    // 保存蛇身体
    for (i = 0; i < snake_length; i++)
        snake_old.body[i] = snake_new.body[i];

    // 蛇身体状态根据前面状态改变一次
    snake_new.tail = snake_new.body[snake_length-1];
    for (i = snake_length-1; i > 0; i--)
        snake_new.body[i] = snake_new.body[i-1];
    snake_new.body[0] = snake_new.head;

    // 根据新旧方向获得正确图片状态
    snake_img_state[0] = getDirect(snake_old.head, snake_new.head);
    for (i = 0; i < snake_length; i++)
        snake_img_state[i+1] = getDirect(snake_old.body[i], snake_new.body[i]);
    snake_img_state[snake_length+1] = getDirect(snake_old.tail, snake_new.tail);

    // 移动蛇坐标
    for (i = snake_length+1; i > 0; i--)
        snake_pos[i] = snake_pos[i-1];
    switch (snake_new.head)
    {
    case MOVE_UP:
        snake_pos[0].y -= 1;
        break;
    case MOVE_DOWN:
        snake_pos[0].y += 1;
        break;
    case MOVE_LEFT:
        snake_pos[0].x -= 1;
        break;
    case MOVE_RIGHT:
        snake_pos[0].x += 1;
        break;
    default:
        break;
    }
}

void MapArea::changeDirect(MoveState dir)
{
    switch (dir)
    {
    case MOVE_NONE:
        snake_dir = dir;
        break;
    case MOVE_UP:
        if (snake_new.head != MOVE_DOWN)
            snake_dir = dir;
        break;
    case MOVE_DOWN:
        if (snake_new.head != MOVE_UP)
            snake_dir = dir;
        break;
    case MOVE_LEFT:
        if (snake_new.head != MOVE_RIGHT)
            snake_dir = dir;
        break;
    case MOVE_RIGHT:
        if (snake_new.head != MOVE_LEFT)
            snake_dir = dir;
        break;
    }

    update();
}

ImgState MapArea::getDirect(MoveState old_dir, MoveState new_dir)
{
    ImgState img_state;
    switch (old_dir)
    {
    case MOVE_NONE:
        switch (new_dir)
        {
        case MOVE_NONE:
            img_state = IMG_NONE;
            break;
        case MOVE_UP:
            img_state = IMG_UP_UP;
            break;
        case MOVE_DOWN:
            img_state = IMG_DOWN_DOWN;
            break;
        case MOVE_LEFT:
            img_state = IMG_LEFT_LEFT;
            break;
        case MOVE_RIGHT:
            img_state = IMG_RIGHT_RIGHT;
            break;
        }
        break;
    case MOVE_UP:
        switch (new_dir)
        {
        case MOVE_UP:
            img_state = IMG_UP_UP;
            break;
        case MOVE_LEFT:
            img_state = IMG_UP_LEFT;
            break;
        case MOVE_RIGHT:
            img_state = IMG_UP_RIGHT;
            break;
        default:
            img_state = IMG_NONE;
        }
        break;
    case MOVE_DOWN:
        switch (new_dir)
        {
        case MOVE_DOWN:
            img_state = IMG_DOWN_DOWN;
            break;
        case MOVE_LEFT:
            img_state = IMG_DOWN_LEFT;
            break;
        case MOVE_RIGHT:
            img_state = IMG_DOWN_RIGHT;
            break;
        default:
            img_state = IMG_NONE;
        }
        break;
    case MOVE_LEFT:
        switch (new_dir)
        {
        case MOVE_UP:
            img_state = IMG_LEFT_UP;
            break;
        case MOVE_DOWN:
            img_state = IMG_LEFT_DOWN;
            break;
        case MOVE_LEFT:
            img_state = IMG_LEFT_LEFT;
            break;
        default:
            img_state = IMG_NONE;
        }
        break;
    case MOVE_RIGHT:
        switch (new_dir)
        {
        case MOVE_UP:
            img_state = IMG_RIGHT_UP;
            break;
        case MOVE_DOWN:
            img_state = IMG_RIGHT_DOWN;
            break;
        case MOVE_RIGHT:
            img_state = IMG_RIGHT_RIGHT;
            break;
        default:
            img_state = IMG_NONE;
        }
        break;
    }

    return img_state;
}

bool MapArea::isHeadTouchBody()
{
    int i;
    for (i = 1; i < snake_length+2; i++)
        if (snake_pos[0].x == snake_pos[i].x && snake_pos[0].y == snake_pos[i].y)
            return true;

    return false;
}

bool MapArea::isGameover()
{
    return gameover;
}

void MapArea::snakeAddBody()
{
    SnakeStruct save_snake_old, save_snake_new;
    SPoint *save_pos;
    ImgState *save_img_state;

    int i;

    // 保存蛇的位置信息
    save_pos = new SPoint[snake_length+2];
    for (i = 0; i < snake_length+2; i++)
        save_pos[i] = snake_pos[i];

    // 保存蛇的状态信息
    save_snake_old.head = snake_old.head;
    save_snake_old.body = new MoveState[snake_length];
    for (i = 0; i < snake_length; i++)
        save_snake_old.body[i] = snake_old.body[i];
    save_snake_old.tail = snake_old.tail;

    save_snake_new.head = snake_new.head;
    save_snake_new.body = new MoveState[snake_length];
    for (i = 0; i < snake_length; i++)
        save_snake_new.body[i] = snake_new.body[i];
    save_snake_new.tail = snake_new.tail;

    save_img_state = new ImgState[snake_length+2];
    for (i = 0; i < snake_length+2; i++)
        save_img_state[i] = snake_img_state[i];

    // 长度增长 n
    snake_length += 1;

    // 释放原来的空间
    delete []snake_pos;
    snake_pos = NULL;
    delete []snake_old.body;
    snake_old.body = NULL;
    delete []snake_new.body;
    snake_new.body = NULL;
    delete []snake_img_state;
    snake_img_state = NULL;

    // 创建并初始化增长后蛇的数据空间
    snake_new.head = MOVE_NONE;
    snake_old.head = MOVE_NONE;
    snake_new.body = new MoveState[snake_length];
    snake_old.body = new MoveState[snake_length];
    for (i = 0; i < snake_length; i++)
    {
        snake_new.body[i] = MOVE_NONE;
        snake_old.body[i] = MOVE_NONE;
    }
    snake_new.tail = MOVE_NONE;
    snake_old.tail = MOVE_NONE;

    snake_img_state = new ImgState[snake_length+2];
    for (i = 0; i < snake_length+2; i++)
    {
        snake_img_state[i] = IMG_NONE;
    }

    snake_pos = new SPoint[snake_length+2];
    for (i = 0; i < snake_length+2; i++)
    {
        snake_pos[i].x = -1;
        snake_pos[i].y = -1;
    }

    // 恢复原来长度的数据
    snake_new.head = save_snake_new.head;
    snake_old.head = save_snake_old.head;
    for (i = 0; i < snake_length-1; i++)
    {
        snake_new.body[i] = save_snake_new.body[i];
        snake_old.body[i] = save_snake_old.body[i];
    }
    snake_new.tail = save_snake_new.tail;
    snake_old.tail = save_snake_old.tail;

    for (i = 0; i < snake_length+1; i++)
        snake_img_state[i] = save_img_state[i];

    for (i = 0; i < snake_length+1; i++)
        snake_pos[i] = save_pos[i];

    snake_new.body[snake_length-1] = save_snake_new.tail;

    switch (snake_new.body[snake_length-1])
    {
    case MOVE_LEFT:
        snake_pos[snake_length+1].x = snake_pos[snake_length].x + 1;
        snake_pos[snake_length+1].y = snake_pos[snake_length].y;
        snake_img_state[snake_length+1] = IMG_LEFT_LEFT;
        break;
    case MOVE_RIGHT:
        snake_pos[snake_length+1].x = snake_pos[snake_length].x - 1;
        snake_pos[snake_length+1].y = snake_pos[snake_length].y;
        snake_img_state[snake_length+1] = IMG_RIGHT_RIGHT;
        break;
    case MOVE_UP:
        snake_pos[snake_length+1].x = snake_pos[snake_length].x;
        snake_pos[snake_length+1].y = snake_pos[snake_length].y + 1;
        snake_img_state[snake_length+1] = IMG_UP_UP;
        break;
    case MOVE_DOWN:
        snake_pos[snake_length+1].x = snake_pos[snake_length].x;
        snake_pos[snake_length+1].y = snake_pos[snake_length].y - 1;
        snake_img_state[snake_length+1] = IMG_DOWN_DOWN;
        break;
    default:
        break;
    }
}

void MapArea::clearFruit(int row, int col)
{
    if (map[row][col] == FRUIT)
        map[row][col] = NOTHING;
}

void MapArea::addFruit()
{
    qsrand((unsigned int)time(NULL));

    int i, j;

    do {
        i = qrand() % (MAP_ROW-2*EDGE_WIDTH) + EDGE_WIDTH;
        j = qrand() % (MAP_COL-2*EDGE_WIDTH) + EDGE_WIDTH;
    } while (map[i][j] != NOTHING || isAtSnake(i, j));

    map[i][j] = FRUIT;
}

void MapArea::addPrizon()
{
    qsrand((unsigned int)time(NULL));

    int i, j;

    do {
        i = qrand() % (MAP_ROW-2*EDGE_WIDTH) + EDGE_WIDTH;
        j = qrand() % (MAP_COL-2*EDGE_WIDTH) + EDGE_WIDTH;
    } while (map[i][j] != NOTHING || isAtSnake(i, j));

    map[i][j] = PRIZON;
}

bool MapArea::isAtSnake(int row, int col)
{
    int i;
    for (i = 0; i < snake_length+2; i++)
        if (snake_pos[i].y == row && snake_pos[i].x == col)
            return true;

    return false;
}
