#include "AI_Level_1.h"
#include <ctime>
#include <random>

#include <windows.h>
#include <gdiplus.h>
#include<windowsx.h>
#include<string>
#include<vector>
step  GetStepV1(byte table[100][100], std::vector<step> steps, int GridSize, bool Side)
{
    std::default_random_engine e;
    e.seed(time(0));
    std::uniform_int_distribution<int> u(0, GridSize-1);

    while (true)
    {
        int x = u(e);
        int y = u(e);
        if (table[x][y] == 0)
        {
            step s;
            s.x = x;
            s.y = y;
            return s;
        }
    }
    step t;
    t.x = -1;
    t.y = -1;

    return t;
}