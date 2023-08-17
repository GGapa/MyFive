#include "AI_Level_1.h"
#include <ctime>
#include <random>

#include <windows.h>
#include <gdiplus.h>
#include<windowsx.h>
#include<string>
#include<vector>

extern int GridRank[100][100];
extern int GridSize;


int Check1(byte ChessTable[100][100], int x, int y, int Addx, int Addy)
{
    int NumberofChess = 1;
    int Apex = 0;
    int newX = x, newY = y;
    byte Team = ChessTable[x][y];
    while (newX >= 0 && newY >= 0 && newX < GridSize && newY < GridSize)
    {
        newX += Addx;
        newY += Addy;
        if (ChessTable[newX][newY] == Team) NumberofChess += 10;
        if (ChessTable[newX][newY] == 0)    NumberofChess += 1;
    }
    newX += Addx;
    newY += Addy;
    if (newX >= 0 && newY >= 0 && newX < GridSize && newY < GridSize && ChessTable[newX][newY] == 0) Apex++;

    NumberofChess--;
    newX = x;
    newY = y;
    Addx = -Addx;
    Addy = -Addy;
    while (newX >= 0 && newY >= 0 && newX < GridSize && newY < GridSize && ChessTable[newX][newY] == Team)
    {
        newX += Addx;
        newY += Addy;
        NumberofChess++;
    }
    if (newX >= 0 && newY >= 0 && newX < GridSize && newY < GridSize && ChessTable[newX][newY] == 0) Apex++;
    NumberofChess--;
    if (Apex >= 1 && NumberofChess == 4)NumberofChess = 1e9;
    if (Apex >= 2 && NumberofChess == 3)NumberofChess = 1e9;
    return NumberofChess;
}




step  GetStepV1(byte table[100][100], std::vector<step> steps, int GridSize, bool Side)
{
    for (int i = 0;i < GridSize;i++)
        for (int j = 0;j < GridSize;j++)
            GridRank[i][j] = 0;
    std::default_random_engine e;
    e.seed(time(0));
    std::uniform_int_distribution<int> u(0, GridSize - 1);
    step s{ s.x = GridSize / 2,s.y = GridSize / 2 };
    if (steps.size() == 0)return s;

    for (int i = 0;i < GridSize;i++)
        for (int j = 0;j < GridSize;j++)
        {
            if (table[i][j] != 0)continue;
            GridRank[i][j] += Check1(table, i, j, 1, 0);
            GridRank[i][j] += Check1(table, i, j, 0, 1);
            GridRank[i][j] += Check1(table, i, j, -1, 1);
            GridRank[i][j] += Check1(table, i, j, 1, 1);
        }

    int max = 0;
    step t;
    for (int i = 0;i < GridSize;i++)
        for (int j = 0;j < GridSize;j++)
        {
            if (GridRank[i][j] > max)
            {
                max = GridRank[i][j];
                t.x = i;t.y = j;
            }
        }



    return t;
}