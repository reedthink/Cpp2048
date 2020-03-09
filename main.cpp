#include <curses.h>
#include <string>
#include <vector>
using namespace std;
const int N = 4;
const int WIDTH = 5;
class Cpp2048 {
private:
    void drawItem(int row, int col, char c)
    {
        move(row, col);
        addch(c);
    }
    void drawNum(int row, int col, int num)
    {
        while (num > 0) {
            drawItem(row, col, num % 10 + '0');
            num /= 10;
            --col;
        }
    }
    int data[N][N];

public:
    Cpp2048();
    ~Cpp2048();
    void draw()
    {
        clear();
        const int offset = 12;
        for (int i = 0; i <= N; i++) {
            for (int j = 0; j <= N; j++) {
                drawItem(i * 2, 1 + j * WIDTH + offset, '+'); //绘制交点

                if (i != N)
                    drawItem(2 * i + 1, 1 + j * WIDTH + offset, '|'); //画格子
                for (int k = 1; j != N && k < WIDTH; ++k)
                    drawItem(2 * i, 1 + j * WIDTH + k + offset, '-'); //画格子

                if (i != N && j != N) {
                    drawNum(i * 2 + 1, (j + 1) * WIDTH + offset, data[i][j]); //填数字
                }
            }
        }
        mvprintw(2 * N + 2, (5 * (N - 4) - 1) / 2, "W(up),S(down),A(left),D(right),R(restart),Q(quit)");
        mvprintw(2 * N + 3, 12 + 5 * (N - 4) / 2, "reedthink");
    }
    void setTestData()
    {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                data[i][j] = 16 << i << j;
            }
        }
    }
};

Cpp2048::Cpp2048(/* args */)
{
    setTestData();
}

Cpp2048::~Cpp2048()
{
}

void initialize()
{
    initscr(); //初始化curses
    cbreak(); //按键不加回车
    noecho(); //隐藏光标
    curs_set(0); //隐藏光标
    srand(time(nullptr));
}
void shutdown()
{
    endwin();
}

int main()
{
    initialize();

    Cpp2048 game;
    char ch = 'n';
    do {
        game.draw();
        ch = getch();
    } while (ch != 'Q' && ch != 'q');
    shutdown();
    return 0;
}