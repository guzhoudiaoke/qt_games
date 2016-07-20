/** snake.cpp
  * guzhoudiaoke@126.com
  * 2011-08-09
  */

#include "snake.h"
#include <stdlib.h>

Snake::Snake()
{

}

Snake::~Snake()
{
    // 释放原来的空间
    if (snake_pos)
        delete []snake_pos;
    snake_pos = NULL;
    if (snake_old.body)
        delete []snake_old.body;
    snake_old.body = NULL;
    if (snake_new.body)
        delete []snake_new.body;
    snake_new.body = NULL;
    if (snake_img_state)
        delete []snake_img_state;
    snake_img_state = NULL;
}

void Snake::destroy()
{
    // 释放原来的空间
    if (snake_pos)
        delete []snake_pos;
    snake_pos = NULL;
    if (snake_old.body)
        delete []snake_old.body;
    snake_old.body = NULL;
    if (snake_new.body)
        delete []snake_new.body;
    snake_new.body = NULL;
    if (snake_img_state)
        delete []snake_img_state;
    snake_img_state = NULL;
}

void Snake::initSnake(SPoint pos, int len)
{
    int i;
    // 蛇的长度
    snake_length = len;

    // 蛇的位置坐标
    snake_pos = new SPoint[snake_length+2];
    snake_pos[0].x = pos.x;
    snake_pos[0].y = pos.y;
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

    snake_dir = MOVE_NONE;

    is_die = false;
}

void Snake::move()
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
    snake_img_state[0] = updateDirect(snake_old.head, snake_new.head);
    for (i = 0; i < snake_length; i++)
        snake_img_state[i+1] = updateDirect(snake_old.body[i], snake_new.body[i]);
    snake_img_state[snake_length+1] = updateDirect(snake_old.tail, snake_new.tail);

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

void Snake::addBody()
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

ImgState Snake::updateDirect(MoveState old_dir, MoveState new_dir)
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

void Snake::changeDirect(MoveState dir)
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
}

bool Snake::isHeadTouchBody()
{
    int i;
    for (i = 1; i < snake_length+2; i++)
        if (snake_pos[0].x == snake_pos[i].x && snake_pos[0].y == snake_pos[i].y)
            return true;

    return false;
}

ImgState *Snake::getImageState()
{
    return snake_img_state;
}

SPoint *Snake::getSnakePos()
{
    return snake_pos;
}

int Snake::getSnakeLength()
{
    return snake_length;
}

bool Snake::isDie()
{
    return is_die;
}

void Snake::die()
{
    is_die = true;
    destroy();
}
