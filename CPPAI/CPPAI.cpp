#include <iostream>
#include <initializer_list>
#include <vector>
#include <random>
struct DenseNeuralNet
{
private:
    //sigmoid function
    float sigmoid(float x)
    {
        return 1. / (1. + exp(-x));
    }


    //devirative of sigmoid
    float desigmoid(float y)
    {
        return y * (1 - y);
    }
    int max_size = 0;
public:
    std::vector<int> all_layer_count;
    int out_count = 0;
    int in_count = 0;
    float*** weights = 0;
    float** bias = 0;
    DenseNeuralNet(std::initializer_list<int> layers)
    {
        if (layers.size() == 0) return;
        all_layer_count = layers;
        weights = new float**[layers.size() - 1];
        bias = new float* [layers.size()];
        for (int layer = 0; layer < int(layers.size()) - 1; layer++)
        {

            weights[layer] = new float*[all_layer_count[layer + 1]];
            bias[layer + 1] = new float[all_layer_count[layer + 1]];
            for (int i = 0; i < all_layer_count[layer + 1]; i++)
            {
                weights[layer][i] = new float[all_layer_count[layer]];
                
                bias[layer + 1][i] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                for (int j = 0; j < all_layer_count[layer]; j++)
                {
                    weights[layer][i][j] = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
                }

            }
        }

        for (int layer = 0; layer < int(layers.size()); layer++)
        {
            max_size = std::max(max_size,all_layer_count[layer]);
        }
    }
    ~DenseNeuralNet()
    {
        for (int layer = 0; layer < all_layer_count.size() - 1; layer++)
        {
            for (int i = 0; i < all_layer_count[layer + 1]; i++)
            {
                delete[] weights[layer][i];
            }
            delete[] weights[layer];
            delete[] bias[layer + 1];
        }
        delete[] weights;
        delete[] bias;
    }

    //give the inp and get result in out
    void forward(float* inp, float* out)
    {
        float* cur = new float[max_size];
        float* pre = new float[max_size];

        for (int layer = 0; layer < all_layer_count.size(); layer++)
        {

            //set inp to current
            if (!layer)
            {
                for (int i = 0; i < all_layer_count[0]; i++)
                {
                    cur[i] = inp[i];
                }
                continue;
            }

            //process
            std::swap(cur, pre);
            for (int i = 0; i < all_layer_count[layer]; i++)
            {
                cur[i] = 0;
                for (int j = 0; j < all_layer_count[layer - 1]; j++)
                {
                    cur[i] += pre[j] * weights[layer - 1][i][j] + bias[layer][i];
                }
                cur[i] = sigmoid(cur[i]);
            }

        }

        //set out to cur
        for (int i = 0; i < all_layer_count.back(); i++)
        {
            out[i] = cur[i];
        }

        //free data
        delete[] cur;
        delete[] pre;
    }

    //give the inp and correct out -> get the loss and train the brain
    void backward(float* inp, float* out, float& loss)
    {
        //forward set up
        float** cur = new float*[all_layer_count.size()];

        for (int layer = 0; layer < all_layer_count.size(); layer++)
        {
            cur[layer] = new float[all_layer_count[layer]];
            //set inp to current
            if (!layer)
            {
                for (int i = 0; i < all_layer_count[0]; i++)
                {
                    cur[layer][i] = inp[i];
                }
                continue;
            }

            //process
            for (int i = 0; i < all_layer_count[layer]; i++)
            {
                cur[i] = 0;
                for (int j = 0; j < all_layer_count[layer - 1]; j++)
                {
                    cur[layer][i] += cur[layer - 1][j] * weights[layer - 1][i][j] + bias[layer][i];
                }
                cur[layer][i] = sigmoid(cur[layer][i]);
            }

        }

        //back propagation




        //free data
        for (int i = 0; i < all_layer_count.size(); i++)
        {
            delete[] cur[i];
        }
        delete[] cur;

        
    }

};

DenseNeuralNet Brain({ 2,3,5,2,1 });

int main()
{
    srand(123);
    float a[] = {2,1};
    float b[] = {0};
    Brain.forward(a,b);
    std::cout << b[0];
}
