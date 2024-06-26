#include <curses.h>
#include <string>
#include <vector>
#include <ctime>
using namespace std;
const int N = 4;
const int WIDTH = 5;
#define TARGET 2048
const int S_FAIL = 0;
const int S_WIN = 1;
const int S_NORMAL = 2;
const int S_QUIT = 3;
class Cpp2048 {
public:
    Cpp2048()
        : status(S_NORMAL)
    {
        setTestData();
    }
    int getStatus() { return status; }
    void processInput()
    {
        char ch = getch();
        ch = toupper(ch);
        if (status == S_NORMAL) {
            bool updated = false;
            if (ch == 'A') {
                moveLeft();
            } else if (ch == 'S') {
                rotate();
                rotate();
                rotate();
                updated = moveLeft();
                rotate();
            } else if (ch == 'D') {
                rotate();
                rotate();
                updated = moveLeft();
                rotate();
                rotate();
            } else if (ch == 'W') {
                rotate();
                updated = moveLeft();
                rotate();
                rotate();
                rotate();
            }
            if (updated) {
                randNew();
                if (isOver()) {
                    status = S_FAIL;
                }
            }
        }

        if (ch == 'Q') {
            status = S_QUIT;
        } else if (ch == 'R'||ch=='B') {
            restart();
        }
    }
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
        mvprintw(2 * N + 2, (5 * (N - 4) - 1) / 2, "B(begin),W(up),S(down),A(left),D(right),R(restart),Q(quit)");
        mvprintw(2 * N + 3, 12 + 5 * (N - 4) / 2, "By reedthink");
        if (status == S_WIN) {
            mvprintw(N, 5 * N / 2 - 1, "WIN!Press R to continue.");
        } else if (status == S_FAIL) {
            mvprintw(N, 5 * N / 2 - 1, "LOSE!Press R to continue.");
        }
    }
    void setTestData()
    {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                data[i][j] = 16 << (i + j);
            }
        }
    }

private:
    bool isOver()
    {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if ((j + 1 < N) && (data[i][j] * data[i][j + 1] == 0 || data[i][j] == data[i][j + 1]))
                    return false;
                if ((i + 1 < N) && (data[i][j] * data[i + 1][j] == 0 || data[i][j] == data[i + 1][j]))
                    return false;
            }
        }
        return true;
    }
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
    int status;
    void restart()
    {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                data[i][j] = 0;
            }
        }
        randNew();
        randNew();
        status = S_NORMAL;
    }
    bool randNew()
    {
        vector<int> emptyPos;
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++)

            {
                if (data[i][j] == 0) {
                    emptyPos.push_back(i * N + j);
                }
            }
        }
        if (emptyPos.size() == 0)
            return false;

        //随机找个空位置
        int value = emptyPos[rand() % emptyPos.size()];

        //10%概率产生4
        data[value / N][value % N] = rand() % 10 == 1 ? 4 : 2;
        return true;
    }
    bool moveLeft()
    {
        int tmp[N][N];
        for (int i = 0; i < N; i++) {
            int currentWritePos = 0;
            int lastValue = 0;
            for (int j = 0; j < N; j++) {
                tmp[i][j] = data[i][j];
                if (data[i][j] == 0) {
                    continue;
                }
                if (lastValue == 0)
                    lastValue = data[i][j];
                else {
                    if (lastValue == data[i][j]) {
                        data[i][currentWritePos] = lastValue * 2;
                        lastValue = 0;
                        if (data[i][currentWritePos] == TARGET) {
                            status = S_WIN;
                        }
                    } else {
                        data[i][currentWritePos] = lastValue;
                        lastValue = data[i][j];
                    }
                    ++currentWritePos;
                }
                data[i][j] = 0;
            }
            if (lastValue != 0) {
                data[i][currentWritePos] = lastValue;
            }
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                if (data[i][j] != tmp[i][j])
                    return true;
            }
        }
        return false;
    }
    void rotate()
    {
        int tmp[N][N] = { 0 };
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                tmp[i][j] = data[j][N - 1 - i];
            }
        }
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                data[i][j] = tmp[i][j];
            }
        }
    }
};

void initialize()
{
    //初始化curses
    initscr();
    //按键不加回车
    cbreak();
    //按键不显示
    noecho();
    //隐藏光标
    curs_set(0);
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
    
    do {
        game.draw();
        game.processInput();
    } while (S_QUIT != game.getStatus());

    shutdown();
    return 0;
}