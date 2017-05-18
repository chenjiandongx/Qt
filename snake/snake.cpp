#include "snake.h"
#include <QMessageBox>
#include <QString>
#include <QtDebug>
#include <windows.h>
#include <QThread>
#include <QStyle>

Snake::Snake(QWidget *parent) :
    QMainWindow(parent)
{
    score = 0;
    level = 1;

    this->setWindowTitle("Snake");
    this->resize((MAX_X + 7) * BLOCK_SIZE,MAX_Y * BLOCK_SIZE);

    startGame = new QPushButton("startGame",this);
    startGame->setStyleSheet("QPushButton{ background-color: rgb(143,122,102); border-radius: 10px; font: bold; color: white; }");

    QFont fontBtn("Consolas",15);
    startGame->setFont(fontBtn);
    startGame->setGeometry(MAX_X * BLOCK_SIZE / 2 - 80,MAX_Y * BLOCK_SIZE / 2 - 40,160,80);

    mLabel = new QLabel(this);
    mLabel->setGeometry(MAX_X * BLOCK_SIZE,BLOCK_SIZE,6 * BLOCK_SIZE,(MAX_Y - 2) * BLOCK_SIZE);
    QFont fontView("Consolas",16);
    mLabel->setFont(fontView);
    mLabel->setAlignment(Qt::AlignCenter);
    // 将分数转换为QString类型
    viewText = QString::number (score,10);
    mLabel->setText("score:" + viewText +'\n' + "level:" + QString::number (level,10));
    mLabel->show();

    init();         // 初始化游戏窗口
    drawBorder();
    initSnake();
    drawViewArea();

    QObject::connect(&timer,SIGNAL(timeout()),this,SLOT(snakeMoveSlots()));
    QObject::connect(startGame,SIGNAL(clicked()),this,SLOT(startGameSlots()));
    Sleep(2000);    // 显示开始画面
}

Snake::~Snake()
{
}

// 画出显示分数区域的边界
void Snake::drawViewArea()
{
    for(int x = MAX_X; x < MAX_X + 7; x++)
    {
        for(int y = 0; y < MAX_Y; y += MAX_Y - 1)
        {
            QLabel *label = new QLabel(this);
            label->setStyleSheet("background:gray");
            label->setGeometry(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1);
        }
    }

    for(int y = 1; y < MAX_Y - 1; y++)
    {
        QLabel *label = new QLabel(this);
        label->setStyleSheet("background:gray");
        label->setGeometry((MAX_X + 6) * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1);
    }

}

// 初始化游戏
void Snake::init()
{
    foodCount = 0;
    moveSpeed = 400;
    // 先把所有的label都初始化为NORMAL类型
    for(int x = 0; x < MAX_X; x++)
    {
        for(int y = 0; y < MAX_Y; y++)
        {
            QLabel *label = new QLabel(this);
            label->setStyleSheet("background:gray");
            label->setGeometry(x * BLOCK_SIZE, y * BLOCK_SIZE, BLOCK_SIZE - 1, BLOCK_SIZE - 1);
            label->hide();

            matrix[x][y] = new snakeNode;
            matrix[x][y]->label = label;
            matrix[x][y]->type = NORMAL_LABEL;
            matrix[x][y]->x = x;
            matrix[x][y]->y = y;
        }
    }
}

// 画边界
void Snake::drawBorder()
{
    for(int x = 0; x < MAX_X; x++)
    {
        for(int y = 0; y < MAX_Y; y++)
        {
            if(x == 0 || y == 0 || x == MAX_X -1 || y == MAX_Y - 1)
            {
                matrix[x][y]->label->setStyleSheet("background:gray");
                matrix[x][y]->label->show();
                matrix[x][y]->type = BORDER_LABEL;
            }
        }
    }
}

// 初始化蛇
void Snake::initSnake()
{
    int snakeLen = 3;   // 蛇长
    int snakeX = 5;     // 蛇头初始化坐标
    int snakeY = 5;

    directionX = 1;
    directionY = 0;
    snake.clear();      // 清除蛇身
    // 将蛇结点追加到表尾，表尾即蛇头
    for(int x = snakeX; x < snakeX + snakeLen; x++)
    {
        snake.append(matrix[x][snakeY]);
        snake.at(snake.length() - 1)->x = x;
        snake.at(snake.length() - 1)->y = snakeY;
        matrix[x][snakeY]->type = SNAKE_LABEL;
        matrix[x][snakeY]->label->setStyleSheet("background:pink");
        matrix[x][snakeY]->label->show();
    }
}

// 移动操作
void Snake::moveSnake()
{
    // 获取蛇头和蛇尾指针
    getHeadTail();
    // 由键盘事件获得的方向来确定蛇头的位置
    snakeNode *tempNode = matrix[head->x + directionX][head->y + directionY];
    tempNode->label->setStyleSheet("background:pink");
    tempNode->label->show();
    // 游戏结束检测，撞到自己和边界即游戏结束
    if(tempNode->type == BORDER_LABEL || tempNode->type == SNAKE_LABEL)
    {
        tempNode->label->setStyleSheet("background:gray");
        gameOver();     // 游戏结束
    }
    else
    {
        // 吃到食物
        if(tempNode->type == FOOD_LABEL)
        {
            showViewArea();
            tempNode->type = SNAKE_LABEL;
            snake.append(tempNode); // 追加到末尾，不隐藏蛇尾
            createFood();           // 生成新的食物
        }
        else
        {
            tempNode->type = SNAKE_LABEL;
            snake.append(tempNode);
            tail->label->hide();    // 将蛇尾隐藏
            tail->type = NORMAL_LABEL;
            // 删除表头，即蛇尾
            snake.removeFirst();
        }
    }
}

// 游戏结束
void Snake::gameOver()
{
    score = 0;      // 初始化数据
    level = 1;
    foodCount = 0;
    moveSpeed = 400;
    timer.stop();   // 计时器停止
    QMessageBox::information(this,"Message:","Game is over! Do you want to try it again?",QMessageBox::Ok);
    // 等同于刷新布局操作
    for(int x = 1; x < MAX_X-1; x++)
    {
        for(int y = 1; y < MAX_Y-1; y++)
        {
            this->matrix[x][y]->type = NORMAL_LABEL;
            this->matrix[x][y]->label->hide();
        }
    }
    // 重新绘制边界
    drawBorder();
    // 显示按钮，可以进行下一次游戏
    startGame->setEnabled(true);
    startGame->show();
}

// 显示分数和等级
void Snake::showViewArea()
{
    foodCount++;
    level = (foodCount / 10) + 1;   // 等级最高为7级
    if(level >= 8)
    {
        level = 8;
    }
    moveSpeed = 50 * (9 - level);   // 计算移动速度
    score = foodCount * level;      // 计算分数
    viewText = QString::number (score,10);
    mLabel->setText("score:" + viewText +'\n' + "level:" + QString::number (level,10));
    mLabel->setAlignment(Qt::AlignCenter);
}

// 获取蛇头和蛇尾指针
void Snake::getHeadTail()
{
    // 蛇头位于表的尾部，即表头为蛇尾
    head = snake.at(snake.length() - 1);
    tail = snake.at(0);
}

void Snake::snakeMoveSlots()
{
    moveSnake();
}

// 开始游戏
void Snake::startGameSlots()
{
    drawBorder();
    initSnake();
    moveSnake();
    createFood();
    viewText = QString::number (score,10);
    mLabel->setText("score:" + viewText +'\n' + "level:" + QString::number (level,10));
    // 开始移动
    timer.start(moveSpeed);
    // 设置按钮不可用，以便窗口获取焦点
    startGame->setEnabled(false);
    startGame->hide();
}

// 键盘事件
void Snake::keyPressEvent(QKeyEvent *e)
{
    switch(e->key())
    {
    // 同方向或者反方向不做任何操作
    case Qt::Key_Up:
        if(up())
        {
            directionY = -1;
            directionX = 0;
            moveSnake();
        }
        break;
    case Qt::Key_Down:
        if(down())
        {
            directionY = 1;
            directionX = 0;
            moveSnake();
        }
        break;
    case Qt::Key_Left:
        if(left())
        {
            directionY = 0;
            directionX = -1;
            moveSnake();

        }
        break;
    case Qt::Key_Right:
        if(right())
        {
            directionY = 0;
            directionX = 1;
            moveSnake();
        }
        break;
    // 游戏暂停
    case Qt::Key_Space:
        if(timer.isActive())
        {
            timer.stop();
        }
        else
        {
            timer.start();
        }
        break;
    default:
        break;
    }
}

// 生成食物
void Snake::createFood()
{
    int foodX;
    int foodY;
    // 随机生成食物坐标
    srand((unsigned)time(0));
    do
    {
        foodX = rand()%MAX_X;
        foodY = rand()%MAX_Y;
    }while(matrix[foodX][foodY]->type != NORMAL_LABEL);

    matrix[foodX][foodY]->type = FOOD_LABEL;
    matrix[foodX][foodY]->label->setStyleSheet("background:lightblue");
    matrix[foodX][foodY]->label->show();
}

// 移动判断
bool Snake::up()
{
    if(directionX == 0)
    {
        return false;
    }
    return true;
}

bool Snake::down()
{
    if(directionX == 0)
    {
        return false;
    }
    return true;
}

bool Snake::left()
{
    if(directionY == 0)
    {
        return false;
    }
    return true;
}

bool Snake::right()
{
    if(directionY == 0)
    {
        return false;
    }
    return true;
}
