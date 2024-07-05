#pragma once
#include <iostream>
#include <initializer_list>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>
#include "DenseNeuralNet.h"
#include <windows.h>
#include "Render.h"
#include <fcntl.h>
#include <io.h>

using namespace std;

bool state = 0;
int curMap[3][3];

Renderer::Screen<34, 34, 10> screen;

int curX = 0, curY = 0;

void Add(int& cur, int val, int _mod = 3)
{
    cur += val;
    if (cur >= _mod) cur -= _mod;
    if (cur < 0) cur += _mod;
}

bool releaseBut = false;
void DoInput()
{
    //check for input
    int total = 0;
    if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
        if (releaseBut) Add(curY, -1);
        total++;
    }

    if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
        if (releaseBut) Add(curY, +1);
        total++;
    }

    if (GetAsyncKeyState(VK_UP) & 0x8000) {
        if (releaseBut) Add(curX, -1);
        total++;
    }

    if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
        if (releaseBut) Add(curX, 1);
        total++;
    }

    if (GetAsyncKeyState(VK_RETURN) & 0x8000) {
        if (releaseBut)
        {
            curMap[curX][curY] = int(state) + 1;
            state = !state;
        }
        total++;
    }

    //Only check short press
    if (total > 0)
    {
        releaseBut = false;
    }
    else
    {
        releaseBut = true;
    }

}




void DrawSelection()
{
    COORD posStart = { 1 + curX * 11 ,   1 + curY * 11 };
    COORD posEnd = { 1 + curX * 11 + 9,1 + curY * 11 + 9 };
    screen.DrawRect(posStart, posEnd, 1, '*');
}

void DrawX(const COORD& coord)
{
    screen.LineTo({ short(coord.X),short(coord.Y) }, { short(coord.X + 10),short(coord.Y + 10) }, 'X');
    screen.LineTo({ short(coord.X + 9),short(coord.Y) }, { short(coord.X - 1),short(coord.Y + 10) }, 'X');
}

void DrawO(const COORD& coord)
{
    screen.DrawCircle({ short(coord.X + 4),short(coord.Y + 4) }, 4.5, 'O');
}

void DrawMap()
{
    wchar_t ch = 0x2588;
    screen.DrawRect({ 0,0 }, { 33,33 }, 1, ch);
    screen.LineTo({ 11,1 }, { 11,33 }, ch);
    screen.LineTo({ 22,1 }, { 22,33 }, ch);
    screen.LineTo({ 1,11 }, { 33,11 }, ch);
    screen.LineTo({ 1,22 }, { 33,22 }, ch);
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            if (curMap[i][j] == 1)
            {
                DrawX({ short(i * 11 + 1),short(j * 11 + 1) });
            }
            else if (curMap[i][j] == 2)
            {
                DrawO({ short(i * 11 + 1),short(j * 11 + 1) });
            }
        }
    }
}

bool DetectWin()
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 1; j < 3; j++)
        {
            if (curMap[i][j] != curMap[i][j - 1] || curMap[i][j] == 0) goto ed1;
        }
        screen.WriteAt({ 0,0 }, L"Finish");
        return 1;
    ed1:
        continue;
    }
    for (int j = 0; j < 3; j++)
    {
        for (int i = 1; i < 3; i++)
        {
            if (curMap[j][i] != curMap[j - 1][i] || curMap[i][j] == 0) goto ed2;
        }
        screen.WriteAt({ 0,0 }, L"Finish");
        return 1;
    ed2:
        continue;
    }
    for (int i = 1; i < 3; i++)
    {
        if (curMap[i][i] != curMap[i - 1][i - 1] || curMap[i][i] == 0) goto ed3;
    }
    return 1;
ed3:

    for (int i = 1; i < 3; i++)
    {
        if (curMap[3 - i - 1][i] != curMap[3 - i][i - 1] || curMap[3 - i - 1][i] == 0) goto ed4;
    }
    return 1;
ed4:
    return 0;
}

bool RunGame()
{
    //draw level
    DrawMap();
    DrawSelection();

    //handle input
    DoInput();

    if (state == 0)
    {
        DrawX({ short(curX * 11 + 1),short(curY * 11 + 1) });

    }
    else
    {
        DrawO({ short(curX * 11 + 1),short(curY * 11 + 1) });
    }

    screen.Render();
    return DetectWin();
}