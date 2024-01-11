#pragma once
#include <iostream>
#include <initializer_list>
#include <vector>
#include <random>
#include <ctime>
#include <cstdlib>
const double e = 2.71828182846;
struct DenseNeuralNet
{
private:
    //sigmoid function
    double sigmoid(double x)
    {
        return 1. / (1. + pow(e, -x));
    }


    //devirative of sigmoid
    double desigmoid(double y)
    {
        return y * (1 - y);
    }
    int max_size = 0;
public:
    std::vector<int> all_layer_count;
    int out_count = 0;
    int in_count = 0;
    double*** weights = 0;
    double** bias = 0;
    DenseNeuralNet(std::initializer_list<int> layers)
    {
        if (layers.size() == 0) return;
        all_layer_count = layers;
        weights = new double** [layers.size() - 1];
        bias = new double* [layers.size()];
        bias[0] = new double[all_layer_count[0]];
        for (int layer = 0; layer < int(layers.size()) - 1; layer++)
        {

            weights[layer] = new double* [all_layer_count[layer + 1]];
            bias[layer + 1] = new double[all_layer_count[layer + 1]];
            for (int i = 0; i < all_layer_count[layer + 1]; i++)
            {
                weights[layer][i] = new double[all_layer_count[layer]];

                bias[layer + 1][i] = static_cast <double> (rand()) / static_cast <double> (RAND_MAX) * 2 - 1;
                for (int j = 0; j < all_layer_count[layer]; j++)
                {
                    weights[layer][i][j] = static_cast <double> (rand()) / static_cast <double> (RAND_MAX) * 2 - 1;
                }

            }
        }

        for (int layer = 0; layer < int(layers.size()); layer++)
        {
            max_size = std::max(max_size, all_layer_count[layer]);
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
    void forward(double* inp, double* out)
    {
        double* cur = new double[max_size];
        double* pre = new double[max_size];

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
                    cur[i] += pre[j] * weights[layer - 1][i][j];
                }
                cur[i] = sigmoid(cur[i] + bias[layer][i]);
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
    void backward(double* inp, double* out, double learning_rate, double& loss)
    {
        //forward set up
        double** cur = new double* [all_layer_count.size()];
        double** errors = new double* [all_layer_count.size()];
        for (int layer = 0; layer < all_layer_count.size(); layer++)
        {
            cur[layer] = new double[all_layer_count[layer]];
            errors[layer] = new double[all_layer_count[layer]];
            //set inp to current
            if (!layer)
            {
                for (int i = 0; i < all_layer_count[0]; i++)
                {
                    cur[0][i] = inp[i];
                }
                continue;
            }

            //process
            for (int i = 0; i < all_layer_count[layer]; i++)
            {
                cur[layer][i] = 0;
                for (int j = 0; j < all_layer_count[layer - 1]; j++)
                {
                    cur[layer][i] += cur[layer - 1][j] * weights[layer - 1][i][j];
                }
                cur[layer][i] = sigmoid(cur[layer][i] + bias[layer][i]);
            }

        }


        //back propagation



        loss = 0;
        for (int layer = all_layer_count.size() - 1; layer >= 1; layer--)
        {
            if (layer == all_layer_count.size() - 1)
            {
                //calculate errors in the output
                for (int i = 0; i < all_layer_count.back(); i++)
                {
                    errors[layer][i] = out[i] - cur[layer][i];
                    loss += errors[layer][i] * errors[layer][i];
                }
            }
            else
            {
                //calculate errors for the rest
                for (int i = 0; i < all_layer_count[layer]; i++)
                {
                    errors[layer][i] = 0;
                    for (int j = 0; j < all_layer_count[layer + 1]; j++)
                    {
                        errors[layer][i] += weights[layer][j][i] * errors[layer + 1][j];
                    }
                }
            }

            //fix the weights and bias:

            for (int i = 0; i < all_layer_count[layer]; i++)
            {
                for (int j = 0; j < all_layer_count[layer - 1]; j++)
                {
                    weights[layer - 1][i][j] += learning_rate * errors[layer][i] * cur[layer - 1][j] * desigmoid(cur[layer][i]);
                }
                bias[layer][i] += learning_rate * errors[layer][i];
            }

        }

        //free data
        for (int i = 0; i < all_layer_count.size(); i++)
        {
            delete[] cur[i];
            delete[] errors[i];
        }
        delete[] cur;
        delete[] errors;

    }

    void debug()
    {
        std::cout << "BRAIN DEBUG:\n";
        std::cout << "Weights:\n";
        for (int layer = 0; layer < all_layer_count.size() - 1; layer++)
        {
            std::cout << "Layer " << layer << "  ->  Layer " << layer + 1 << ":\n";
            for (int i = 0; i < all_layer_count[layer]; i++)
            {
                for (int j = 0; j < all_layer_count[layer + 1]; j++)
                {
                    std::cout << weights[layer][j][i] << " ";
                }
                std::cout << "\n";
            }
            std::cout << "\n";
        }

        std::cout << "Bias:\n";
        for (int layer = 1; layer < all_layer_count.size(); layer++)
        {
            std::cout << "Layer " << layer << ":\n";
            for (int i = 0; i < all_layer_count[layer]; i++)
            {
                std::cout << bias[layer][i] << " ";
                std::cout << "\n";
            }
            std::cout << "\n";
        }
    }

};