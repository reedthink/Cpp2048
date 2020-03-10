
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <curses.h>
#include <string>
#include <vector>
using namespace std; // 格子数
#define N 4 // 每个格子的字符长度
#define WIDTH 5 // 游戏状态
#define S_FAIL 0
#define S_WIN 1
#define S_NORMAL 2
#define S_QUIT 3
class Game2048 {
public:
    Game2048()
        : status(S_NORMAL)
    {
        setTestData();
    }
    int getStatus() { return status; } // 处理按键
    void processInput()
    {
        char ch = getch(); // 转化成大写
        if (ch >= 'a' && ch <= 'z') {
            ch -= 32;
        }
        if (ch == 'Q') {
            status = S_QUIT;
        } else { // 在各种状态中间遍历，测试界面
            status = (status + 1) % 3;
        }
    } // 绘制游戏界面
    void draw()
    { // 清理屏幕
        clear();
        // 居中偏移
        const int offset = 12;
        for (int i = 0; i <= N; ++i) {
            for (int j = 0; j <= N; ++j) { // 相交点
                drawItem(i * 2, 1 + j * WIDTH + offset, '+'); // 竖线
                if (i != N) {
                    drawItem(i * 2 + 1, 1 + j * WIDTH + offset, '|');
                } // 横线
                for (int k = 1; j != N && k < WIDTH; ++k) {
                    drawItem(i * 2, 1 + j * WIDTH + k + offset, '-');
                }
                // 每个格子里的数
                if (i != N && j != N) {
                    drawNum(i * 2 + 1, (j + 1) * WIDTH + offset, data[i][j]);
                }
            }
        } // 提示文字
        mvprintw(2 * N + 2, (5 * (N - 4) - 1) / 2, "W(UP),S(DOWN),A(LEFT),D(RIGHT),R(RESTART),Q(QUIT)");
        mvprintw(2 * N + 3, 12 + 5 * (N - 4) / 2, "https://www.nowcoder.com");
        if (status == S_WIN) {
            mvprintw(N, 5 * N / 2 - 1, " YOU WIN,PRESS R TO CONTINUE ");
        } else if (status == S_FAIL) {
            mvprintw(N, 5 * N / 2 - 1, " YOU LOSE,PRESS R TO CONTINUE ");
        }
    } // 方便调试, 随时设置数据
    void setTestData()
    {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                data[i][j] = 16 << i << j; /*                 data[i][0] = 2;                 data[i][1] = 4;                 data[i][2] = 8;                 data[i][3] = 16;*/
            }
        }
    }

private: // 左上角为（0，0），在指定的位置画一个字符
    void drawItem(int row, int col, char c)
    {
        move(row, col);
        addch(c);
    } // 游戏里的数字是右对齐，row, col是数字最后一位所在的位置
    void drawNum(int row, int col, int num)
    {
        while (num > 0) {
            drawItem(row, col, num % 10 + '0');
            num /= 10;
            --col;
        }
    }

private:
    int data[N][N];
    int status;
};
void initialize()
{ // ncurses初始化
    initscr();
    // 按键不需要输入回车直接交互
    cbreak();
    // 按键不显示
    noecho();
    // 隐藏光标
    curs_set(0);
    // 随机数
    srand(time(NULL));
}
void shutdown()
{ // ncurses清理
    endwin();
}
int main()
{
    initialize();
    Game2048 game;
    do {
        game.draw();
        game.processInput();
    } while (S_QUIT != game.getStatus());
    shutdown();
    return 0;
}
