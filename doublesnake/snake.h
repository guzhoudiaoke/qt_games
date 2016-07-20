/** snake.h
  * guzhoudiaoke@126.com
  * 2011-08-09
  */

#ifndef SNAKE_H
#define SNAKE_H

#include "snakedefines.h"

class Snake
{
public:
    Snake();
    ~Snake();

    void initSnake(SPoint pos, int len);// 初始化蛇
    void move();                        // 蛇向前运动
    void addBody();                     // 蛇长度增长 n
    void changeDirect(MoveState dir);   // 改变蛇运动方向

    ImgState updateDirect(MoveState old_dir, MoveState new_dir);

    bool isHeadTouchBody();

    ImgState *getImageState();
    SPoint *getSnakePos();
    int getSnakeLength();
    bool isDie();
    void die();
    void destroy();

private:
    SPoint *snake_pos;          // 蛇的位置信息
    ImgState *snake_img_state;  // 蛇的身体对应图片

    typedef struct SnakeStruct{
        MoveState  head;        // 蛇头
        MoveState *body;        // 蛇身
        MoveState  tail;        // 蛇尾
    } SnakeStruct;

    SnakeStruct snake_new;      // 蛇的新状态所有节点运动状态
    SnakeStruct snake_old;      // 蛇的旧状态所有节点运动状态

    int snake_length;           // 蛇的长度
    MoveState snake_dir;        // 蛇移动方向，转向时用

    bool is_die;                // 是否死亡
};

#endif // SNAKE_H
