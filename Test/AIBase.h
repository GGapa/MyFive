#pragma once
#include "Test.h"
#include <vector>
#include <windows.h>
#include <gdiplus.h>
class AIBase
{
protected :

   virtual step GetStep(byte table[100][100], std::vector<step> steps,int GridSize, bool Side);
};

