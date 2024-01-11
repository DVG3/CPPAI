#include <iostream>
#include <initializer_list>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>
#include "DenseNeuralNet.h"

double inp[4][2] = { {0,0},{1,1},{0,1}, {1,0}  };
double out[4][1] = { {1},{1},{0},{0} };
DenseNeuralNet Brain({ 2,3,1 });
using namespace std;

void training()
{
    //Brain.debug();


    for (int i = 0; i <= 50000; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            double loss = 0;
            Brain.backward(inp[j], out[j], 0.02f, loss);
        }
    }

    //Brain.debug();
}

int main()
{
    srand((unsigned)time(0));
    double a[] = {0,1};
    double b[] = {0};
    Brain.forward(a, b);
    std::cout << "First: " << b[0] << "\n";
    
    training();


    
    for (int j = 0; j < 4; j++)
    {
        cout << inp[j][0] << " " << inp[j][1] << " ";
        Brain.forward(inp[j], b);
        std::cout << b[0] << "\n";
        b[0] = 0;
    }
}
