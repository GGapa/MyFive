#include <windows.h>
#include <gdiplus.h>
#include<windowsx.h>
#include<string>
#include<vector>

#include "Config.h"
#include "Test.h"
#include "IsWin.h"
#include "AI_Level_1.h"
using namespace Gdiplus;
#pragma comment (lib,"Gdiplus.lib")

int GridSize = 15; //棋盘宽度
int WindowWidth =768; //窗口大小
int SideWidth = 30; //棋盘边框与窗体距离
bool ShowNum = true; //是否在棋子上显示步数
bool ShowTag = true; //是否在棋盘上显示坐标标记
bool ShowRank = true;//是否在棋盘上显示权重
int xPos, yPos;
bool ChessColor = false; //false=黑棋 ，true=白棋
int ChessSize = 35; //棋子默认尺寸
byte Data[100][100] = { 0 };
int GridRank[100][100] = { 0 };
bool IsOver = false; //棋局是否结束
bool IsProc = false; //是否正在后台处理数据
int black = 0, white = 0;
std::vector<step> steps;
HWND hWnd;

void DoEvents()
{
    //return;
    MSG msg;
    while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    {
        if (IsOver) return;
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}

bool PutChess(int x, int y)
{
    if (IsOver) return false;
    int xP = x,yP = y;
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
        if (IsWin(Data, xP, yP))
        {
            InvalidateRect(hWnd, NULL, false);
            MessageBox(NULL, (ChessColor == 1 ? L"白棋胜利" : L"黑棋胜利"), L"胜利提示", 0);
            IsOver = true;
            //exit(0);
        }
        ChessColor = !ChessColor;
        return true;
    }
    return false;
}

bool RecChess(int x, int y )
{
    //手工下棋
    if (IsOver) return false;
    int Scale = (WindowWidth - SideWidth - SideWidth) / GridSize;

    int xP = ((x - SideWidth) / Scale);
    int yP = ((y - SideWidth) / Scale);

    return PutChess(xP, yP);
}

//决定下棋策略
void ProcChess()
{
    if (!ChessColor && black == 0)return ;
    if (ChessColor && white == 0)return ;
    while (true)
    {
        if (IsOver) return;
        InvalidateRect(hWnd, NULL, false);
        UpdateWindow(hWnd);
        DoEvents();
        Sleep( 500);
        DoEvents();

        if (!ChessColor && black != 0)
        {
            step s;
            if (black == 1) s = GetStepV1(Data, steps, GridSize, ChessColor);

            PutChess(s.x, s.y);
        }

        InvalidateRect(hWnd, NULL, false);
        UpdateWindow(hWnd);
        DoEvents();
        Sleep(1 * 500);
        DoEvents();
        if (ChessColor && white != 0)
        {
            step s;
            if (white == 1) s = GetStepV1(Data, steps, GridSize, ChessColor);

            PutChess(s.x, s.y);
        }

        InvalidateRect(hWnd, NULL, false);
        UpdateWindow(hWnd);
        DoEvents();
        Sleep(500);
        DoEvents();

        if (!ChessColor && black == 0)return;
        if (ChessColor && white == 0)return;
    }

}


VOID OnPaint(HDC hdc, HWND hWnd)
{
    
    //Graphics graphics(hdc);
    //双缓冲
    HBITMAP hBackbuffer = CreateCompatibleBitmap(hdc, WindowWidth, WindowWidth);
    HDC hBackbufferDC = CreateCompatibleDC(hdc);
    SelectObject(hBackbufferDC, hBackbuffer);


    //Bitmap bmp(WindowWidth + 30, WindowWidth + 30);
    //Graphics *graphics = Graphics::FromImage(&bmp);

    Graphics graphics(hBackbufferDC);
    graphics.Clear(Color::White);
    graphics.SetSmoothingMode(Gdiplus::SmoothingModeHighQuality);
    Pen pen(Color(255, 0, 0, 255));

    int Scale = (WindowWidth - SideWidth - SideWidth) / GridSize;
    //画棋盘。。
    for (int i = 0; i <= GridSize; i++)
    {
        int Point = SideWidth + Scale * i;
        graphics.DrawLine(&pen, Point, SideWidth, Point, WindowWidth - SideWidth - 3);//-3是为了消除小数误差

        graphics.DrawLine(&pen, SideWidth, Point, WindowWidth - SideWidth - 3, Point);


        //画坐标轴标记
        if (ShowTag)
        {
            if (i + 1 > GridSize) continue;
            WCHAR  t[4];
            FontFamily  fontFamily(L"Consolas");
            Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
            wsprintfW(t, L"%d", i + 1);
            PointF pointF(Point + (i < 9 ? 13 : 5), 3);
            SolidBrush brush(Color(255, 0, 0, 255));
            graphics.DrawString(t, -1, &font, pointF, &brush);
            PointF pointF1(3, Point + 13);
            t[0] = 'A' + i; t[1] = 0;
            graphics.DrawString(t, -1, &font, pointF1, &brush);
        }
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
            FontFamily  fontFamily(L"Consolas");
            Font font(&fontFamily, 24, FontStyleRegular, UnitPixel);
            wsprintfW(t, L"%d", i + 1);
            PointF pointF(xP + (i < 9 ? 7 : 2), yP + 3);
            if (steps[i].Team)c = Color(255, 0, 0, 0);
            else c = Color(255, 255, 255, 255);
            SolidBrush brush1(c);
            graphics.DrawString(t, -1, &font, pointF, &brush1);
        }
    }

    //画权重
    for(int i =0 ;i<GridSize;i++)
        for (int j= 0;j < GridSize;j++)
        {
            if (Data[i][j] != 0) continue;
            int xP = i * Scale + SideWidth + 6;
            int yP = j* Scale + SideWidth + 6;
            WCHAR  t[40];
            Color c= Color(255, 128, 128, 128);
            FontFamily  fontFamily(L"Consolas");
            Font font(&fontFamily, 12, FontStyleRegular, UnitPixel);
            wsprintfW(t, L"%d", GridRank[i][j]);
            PointF pointF(xP + (i < 9 ? 7 : 2), yP + 3);
            
            SolidBrush brush1(c);
            graphics.DrawString(t, -1, &font, pointF, &brush1);

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

        //Graphics g01(hdc);
        //g01.DrawImage(&bmp, 0, 0, WindowWidth + 30, WindowWidth + 30);

    BitBlt(hdc, 0, 0, WindowWidth, WindowWidth,
        hBackbufferDC, 0, 0,
        SRCCOPY);
    ReleaseDC(hWnd, hdc);
    DeleteDC(hBackbufferDC);
    DeleteObject(hBackbuffer);
}

VOID init()
{
    GridSize = ReadINI("GridSize", 15);
    ShowNum= ReadINI("ShowNum", 1);
    ShowTag= ReadINI("ShowTag", 1);
    WindowWidth = ReadINI("WindowWidth", 768);
    black = ReadINI("black",0);
    white = ReadINI("white", 0);
    ShowRank = ReadINI("ShowRank", 1);

    //得到在当前窗体大小下，每个格子的宽度，这个宽度是动态的
    int Scale = (WindowWidth - SideWidth - SideWidth) / GridSize;
    ChessSize = Scale - 12;
}


LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);


INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, PSTR, INT iCmdShow)//不用管，Main
{

    MSG                 msg;
    WNDCLASS            wndClass;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR           gdiplusToken;
    
    init();

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

    //开始下棋

    ShowWindow(hWnd, iCmdShow);
    UpdateWindow(hWnd);

    ProcChess();
    
    while (GetMessage(&msg, NULL, 0, 0))
    //while (PeekMessage( & msg, hWnd, 0, 0, PM_REMOVE))
    {
        //if (IsOver) return 0;
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
        OnPaint(hdc, hWnd);
        EndPaint(hWnd, &ps);
        return 0;
    case WM_DESTROY:
        IsOver = true;
        PostQuitMessage(0);
        return 0;
    case WM_LBUTTONDOWN:
        xPos = GET_X_LPARAM(lParam);
        yPos = GET_Y_LPARAM(lParam);
        if (!IsProc)
        {
            IsProc = true;
            if(RecChess(xPos, yPos))
            {
                InvalidateRect(hWnd, NULL, false);
                //UpdateWindow(hWnd);
            }
            IsProc = false;
            //决定下一步
            ProcChess();
        }
        //SetWindowTextW(hWnd, L"asdasdas");
        //MessageBox(hWnd, (LPCWSTR)L"落子", (LPCWSTR)L"提示", 0);
        return 0;
    case WM_ICONERASEBKGND:
        return 0;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
}