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
#include "TicTacToe.h"

using namespace std;



int main()
{

    srand((unsigned)time(0));
    screen.consoleHandle = GetStdHandle(STD_INPUT_HANDLE);
    SetConsoleMode(screen.consoleHandle, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

    

    while (true)
    {
        //clear previos screen
        screen.Clear();

        if (RunGame()) return 0;
        Sleep(10);
    }


}
