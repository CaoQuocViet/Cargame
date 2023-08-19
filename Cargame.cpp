#include <iostream>
#include <conio.h>
#include <dos.h>
#include <windows.h>
#include <time.h>

#define SCREEN_WIDTH 90
#define SCREEN_HEIGHT 26
#define WIN_WIDTH 70

using namespace std;

HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);
COORD CursorPosition;

int enemyY[3];
int enemyX[3];
int enemyFlag[3];
char car[4][4] = {' ', 'O', 'O', ' ',
                  'O', 'O', 'O', 'O',
                  ' ', '0', '0', ' ',
                  'O', 'O', 'O', 'O'};
int carPos = (WIN_WIDTH / 2);
int score = 0;

void SET_COLOR(int color) {
    WORD wColor;
    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    if (GetConsoleScreenBufferInfo(hStdOut, &csbi)) {
        wColor = (csbi.wAttributes & 0xF0) + (color & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
}

void gotoxy(int x, int y) {
    CursorPosition.X = x;
    CursorPosition.Y = y;
    SetConsoleCursorPosition(console, CursorPosition);
}

void setcursor(bool visible, DWORD size) {
    if (size == 0)
        size = 20;
    CONSOLE_CURSOR_INFO lpCursor;
    lpCursor.bVisible = visible;
    lpCursor.dwSize = size;
    SetConsoleCursorInfo(console, &lpCursor);
}

void drawBorder() {
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < 5; j++) {
            gotoxy(0 + j, i);
            cout << "±";
            gotoxy(WIN_WIDTH - j, i);
            cout << "±";
        }
    }
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        gotoxy(SCREEN_WIDTH, i);
        cout << "±";
    }
}

void genEnemy(int ind) {
    enemyX[ind] = 6 + rand() % (56);
}

void drawEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyX[ind], enemyY[ind]);
        cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 1);
        cout << " 00 ";
        gotoxy(enemyX[ind], enemyY[ind] + 2);
        cout << "****";
        gotoxy(enemyX[ind], enemyY[ind] + 3);
        cout << " ** ";
    }
}

void eraseEnemy(int ind) {
    if (enemyFlag[ind] == true) {
        gotoxy(enemyX[ind], enemyY[ind]);
        cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 1);
        cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 2);
        cout << "    ";
        gotoxy(enemyX[ind], enemyY[ind] + 3);
        cout << "    ";
    }
}

void resetEnemy(int ind) {
    eraseEnemy(ind);
    enemyY[ind] = 0;
    genEnemy(ind);
}

void drawCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carPos, i + 22);
            cout << car[i][j];
        }
    }
}

void eraseCar() {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            gotoxy(j + carPos, i + 22);
            cout << " ";
        }
    }
}

int collision() {
    if (enemyY[0] + 4 >= 23) {
        if (enemyX[0] + 4 - carPos >= 0 && enemyX[0] + 4 - carPos < 9) {
            return 1;
        }
    }
    return 0;
}

void gameover() {
    system("cls");
    cout << endl;
    cout << "\n\n\n\n\n\n\n\n\n\n\t\t\t\t\t\t--------------------------" << endl;
    cout << "\t\t\t\t\t\t-------- Game Over -------" << endl;
    cout << "\t\t\t\t\t\t--------------------------" << endl << endl;
    cout << "\t\t\t\t\t\t       Your Score: " << score << endl;
    cout << "\n\t\t\t\t\t     Press any key to go back to menu.";
    getch();
}

void updateScore() {
    gotoxy(WIN_WIDTH + 7, 5 + 3);
    cout << "Score: " << score << endl;
}

void instructions() {
    system("cls");
    cout << "\n\n\n\n\n\n\n\n                                       Instructions";
    cout << "\n                                   -----------------";
    cout << "\n\n                                 + Control the car by moving left or right.";
    cout << "\n                                 + Press 'z' to move left.";
    cout << "\n                                 + Press 'c' to move right.";
    cout << "\n                                 + Press 'escape' to exit.";
    cout << "\n                                 + Press any key to return to the menu.";
    getch();
}

void play() {
    carPos = -1 + WIN_WIDTH / 2;
    score = 0;
    enemyFlag[0] = 1;
    enemyFlag[1] = 0;
    enemyY[0] = enemyY[1] = 1;

    system("cls");
    SET_COLOR(3);
    drawBorder();
    updateScore();
    SET_COLOR(5);
    genEnemy(0);
    SET_COLOR(6);
    genEnemy(1);

    gotoxy(WIN_WIDTH + 7, 2 + 3);
    cout << "Car Game";
    gotoxy(WIN_WIDTH + 6, 4 + 3);
    cout << "----------";
    gotoxy(WIN_WIDTH + 7, 6 + 3);
    cout << "----------";
    gotoxy(WIN_WIDTH + 8, 12 + 3);
    cout << "Control ";
    gotoxy(WIN_WIDTH + 7, 13 + 3);
    cout << "-  --------";
    gotoxy(WIN_WIDTH + 4.5, 14 + 3);
    cout << " A Key - Left";
    gotoxy(WIN_WIDTH + 4.5, 15 + 3);
    cout << " D Key - Right";

    gotoxy(25, 12);
    cout << "Press any key to start";
    getch();
    gotoxy(25, 12);
    cout << "                      ";

    while (1) {
        if (kbhit()) {
            char ch = getch();
            if (ch == 'a' || ch == 'A') {
                if (carPos > 6)
                    carPos -= 4;
            }
            if (ch == 'd' || ch == 'D') {
                if (carPos < 62)
                    carPos += 4;
            }
            if (ch == 27) {
                break;
            }
        }

        srand(time(0));
        SET_COLOR((rand() % 14) + 2);
        drawCar();
        SET_COLOR((rand() % 14) + 2);
        drawEnemy(0);
        SET_COLOR((rand() % 14) + 2);
        drawEnemy(1);
        SET_COLOR((rand() % 14) + 2);

        if (collision() == 1) {
            gameover();
            return;
        }
        Sleep(30);
        eraseCar();
        eraseEnemy(0);
        eraseEnemy(1);
        if (enemyY[0] == 10)
            if (enemyFlag[1] == 0)
                enemyFlag[1] = 1;
        if (enemyFlag[0] == 1)
            enemyY[0] += 1;
        if (enemyFlag[1] == 1)
            enemyY[1] += 1;
        if (enemyY[0] > SCREEN_HEIGHT - 4) {
            resetEnemy(0);
            score++;
            updateScore();
        }
        if (enemyY[1] > SCREEN_HEIGHT - 4) {
            resetEnemy(1);
            score++;
            updateScore();
        }
    }
}

int main() {
    setcursor(0, 0);
    srand((unsigned)time(NULL));
    do {
        system("cls");
        gotoxy(45, 9);
        cout << " -------------------------- ";
        gotoxy(45, 10);
        cout << " |        Car Game        | ";
        gotoxy(45, 11);
        cout << " --------------------------";
        gotoxy(46, 13);
        cout << "1. Start Game";
        gotoxy(46, 14);
        cout << "2. Instructions";
        gotoxy(46, 15);
        cout << "3. Quit";
        gotoxy(46, 17);
        cout << "Select option: ";
        char op = getche();
        if (op == '1')
            play();
        else if (op == '2')
            instructions();
        else if (op == '3')
            exit(0);
    } while (1);
    return 0;
}
