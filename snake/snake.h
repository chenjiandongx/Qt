#ifndef SNAKE_H
#define SNAKE_H
const int MAX_X = 24;
const int MAX_Y = 20;
const int BLOCK_SIZE = 32;
// label类型
enum Label
{
    NORMAL_LABEL,   // 普通类型，即背景
    BORDER_LABEL,   // 边界类型
    SNAKE_LABEL,    // 蛇结点类型
    FOOD_LABEL      // 食物类型
};

#include <QMainWindow>
#include <QLabel>
#include <QList>
#include <QTimer>
#include <QKeyEvent>
#include <ctime>
#include <QPushButton>
#include <QFont>

// 蛇结点
struct snakeNode
{
    QLabel *label;
    int type;
    int x;
    int y;
};

class Snake : public QMainWindow
{
    Q_OBJECT

public:
    explicit Snake(QWidget *parent = 0);
    ~Snake();
    void init();            // 初始化游戏
    void drawBorder();      // 画边界
    void drawViewArea();    // 画显示区域
    void showViewArea();    // 显示分数和等级
    void gameOver();        // 游戏结束
    void initSnake();       // 初始化蛇
    void moveSnake();       // 移动蛇
    void getHeadTail();     // 获取蛇头和蛇尾
    bool left();            // 判断能否向上下左右移动
    bool right();
    bool up();
    bool down();
    void createFood();  // 创建食物
    QTimer timer;
    QPushButton *startGame;

protected:
    void keyPressEvent(QKeyEvent *e);   // 键盘事件

private slots:
    void snakeMoveSlots();      // 移动操作
    void startGameSlots();      // 游戏开始

private:
    snakeNode *matrix[MAX_X][MAX_Y];// 储存游戏画面
    QList<snakeNode*> snake;        // 蛇身

    int directionX,directionY;      // 移动方向
    int foodCount,moveSpeed;        // 食物数和速度
    int level,score;                // 分数和等级
    snakeNode *head;                // 蛇头和蛇尾指针
    snakeNode *tail;
    QLabel *mLabel;
    QString viewText;               // 显示区域文本

};

#endif // SNAKE_H
