#include "AI_Level_2.h"
#include <ctime>
#include <random>
#include "sys/timeb.h"
#include <windows.h>
#include <gdiplus.h>
#include<windowsx.h>
#include<string>
#include<vector>






int getWeights(byte table[100][100], int GridSize, bool Side, int x, int y)
{
    int Value = 0;
    
    return Value;
}

step  GetStepV2(byte table[100][100], std::vector<step> steps, int GridSize, bool Side)
{
    struct timeb timeSeed;
    ftime(&timeSeed);
    srand(timeSeed.millitm);
    int Weights[100][100];

    for (int i = 0; i < GridSize; i++)
    {
        for (int j = 0; j < GridSize; i++)
        {
            Weights[i][j] = getWeights(table, GridSize, Side, i, j);
        }
    }
    return steps[0];
}