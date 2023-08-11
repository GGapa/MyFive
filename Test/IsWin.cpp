#include "IsWin.h"
#include "Config.h"
#include<Windows.h>

const int WinNumber = 5;

int Check(byte ChessTable[100][100], int x, int y, int Addx, int Addy)
{
    int NumberofChess = 1;
    int PanelSize = ReadINI("PanelSize", 15);
    int newX = x, newY = y;
    byte Team = ChessTable[x][y];
    while (newX >= 0 && newY >= 0 && newX < PanelSize && newY < PanelSize && ChessTable[newX][newY] == Team)
    {
        newX += Addx;
        newY += Addy;
        NumberofChess++;
    }
    NumberofChess--;
    newX = x; 
    newY = y;
    Addx = -Addx;
    Addy = -Addy;
    while (newX >= 0 && newY >= 0 && newX < PanelSize && newY < PanelSize && ChessTable[newX][newY] == Team)
    {
        newX += Addx;
        newY += Addy;
        NumberofChess++;
    }
    NumberofChess--;
    return NumberofChess;
}

bool IsWin(byte ChessTable[100][100], int x, int y)
{
    int PanelSize = ReadINI("PanelSize", 15);
    return Check(ChessTable, x, y, 1, 0) == WinNumber ||
           Check(ChessTable, x, y, 0, 1) == WinNumber ||
           Check(ChessTable, x, y, 1, 1) == WinNumber ||
           Check(ChessTable, x, y, -1, 1) == WinNumber;
    
}