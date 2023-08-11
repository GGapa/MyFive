#include<iostream>
#include <windows.h>
#include <gdiplus.h>
#include<windowsx.h>
#include<string>
#include<vector>
#include "Config.h"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")
const int PanelWidth = 15; //棋盘宽度
int WindowWidth = 768; //窗口大小
int SideWidth = 30; //棋盘边框与窗体距离
bool ShowNum = false; //是否在棋子上显示步数
int xPos, yPos;
bool ChessColor = false;
int ChessSize = 35;
int Data[PanelWidth][PanelWidth] = { 0 };

struct step
{
    int x, y;
    bool Team;
};


std::vector<step> steps;

bool RecChess(int x, int y)
{
    //得到在当前窗体大小下，每个格子的宽度，这个宽度是动态的
    int Scale = (WindowWidth - SideWidth - SideWidth) / PanelWidth;
    ChessSize = Scale - 12;
    int xP = ((x - SideWidth) / Scale);
    int yP = ((y - SideWidth) / Scale);
    if (Data[xP][yP] == 0)
    {
        if (ChessColor)
            Data[xP][yP] = 1;
        else
            Data[xP][yP] = 2;

        step t;
        t.x = xP;
        t.y = yP;
        t.Team = ChessColor;
        steps.push_back(t);
        ChessColor = !ChessColor;
        return true;
    }
    return false;
}

VOID OnPaint(HDC hdc)
{
    Graphics graphics(hdc);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    Pen      pen(Color(255, 0, 0, 255));

    int Scale = (WindowWidth - SideWidth - SideWidth) / PanelWidth;
    //画棋盘。。
    for (int i = 0; i <= PanelWidth; i++)
    {
        int Point = SideWidth + Scale * i;
        graphics.DrawLine(&pen, Point, SideWidth, Point, WindowWidth - SideWidth - 3);//-3是为了消除小数误差

        graphics.DrawLine(&pen, SideWidth, Point, WindowWidth - SideWidth - 3, Point);

        std::cout << Point << std::endl;
    }
    //画棋子


    for (int i = 0; i < steps.size(); i++)
    {
        int xP = steps[i].x * Scale + SideWidth + 6;
        int yP = steps[i].y * Scale + SideWidth + 6;
        Color c;
        //if (Data[i][j] == 0)continue;
        if (steps[i].Team) c = Color(255, 255, 255, 255);
        else  c = Color(255, 0, 0, 0);
        SolidBrush brush(c);
        graphics.DrawEllipse(&pen, xP, yP, ChessSize, ChessSize);
        graphics.FillEllipse(&brush, xP, yP, ChessSize, ChessSize);
        if (ShowNum)
        {
            WCHAR  t[4];
            FontFamily  fontFamily(L"Times New Roman");
            Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
            wsprintfW(t, L"%d", i);
            PointF pointF(xP + (i < 10 ? 7 : 2), yP + 3);
            if (steps[i].Team)c = Color(255, 0, 0, 0);
            else c = Color(255, 255, 255, 255);
            SolidBrush brush1(c);
            graphics.DrawString(t, -1, &font, pointF, &brush1);
        }
    }

    /*
    for(int i=0;i< PanelWidth;i++)
        for (int j = 0; j < PanelWidth; j++)
        {
            int xP = i * Scale + SideWidth + 6;
            int yP = j * Scale + SideWidth + 6;
            Color c;
            if (Data[i][j] == 0)continue;
            else if (Data[i][j] == 1) c = Color(255, 255, 255, 255);
            else  c = Color(255, 0, 0, 0);
            SolidBrush brush(c);
            graphics.DrawEllipse(&pen, xP, yP, ChessSize, ChessSize);
            graphics.FillEllipse(&brush, xP, yP, ChessSize, ChessSize);

        }
        */
        //delete& graphics;

}



LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)
{
    HWND                hWnd;
    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;

    // Initialize GDI+.
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

    wndClass.style = CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WndProc;
    wndClass.cbClsExtra = 0;
    wndClass.cbWndExtra = 0;
    wndClass.hInstance = hInstance;
    wndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndClass.lpszMenuName = NULL;
    wndClass.lpszClassName = TEXT("MyFive");

    RegisterClass(&wndClass);

    hWnd = CreateWindow(
        TEXT("MyFive"),   // window class name
        TEXT("五子棋"),  // window caption
        WS_OVERLAPPEDWINDOW,      // window style
        CW_USEDEFAULT,            // initial x position
        CW_USEDEFAULT,            // initial y position
        WindowWidth,            // initial x size
        //CW_USEDEFAULT,            // initial y size
        WindowWidth + 30,//+30抵消窗标题栏的影响
        NULL,                     // parent window handle
        NULL,                     // window menu handle
        hInstance,                // program instance handle
        NULL);                    // creation parameters

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return msg.wParam;
}  // WinMain

LRESULT CALLBACK WndProc(HWND hWnd, UINT message,
    WPARAM wParam, LPARAM lParam)
{
    HDC          hdc;
    PAINTSTRUCT  ps;

    switch (message)
    {
    case WM_PAINT:
        hdc = BeginPaint(hWnd, &ps);
        OnPaint(hdc);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);
        if (RecChess(xPos, yPos))
        {
            InvalidateRect(hWnd, NULL, TRUE);
            UpdateWindow(hWnd);
        }
        SetWindowTextW(hWnd, L"asdasdas");
        //MessageBox(hWnd, (LPCWSTR)L"落子", (LPCWSTR)L"提示", 0);
        return 0;

    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}