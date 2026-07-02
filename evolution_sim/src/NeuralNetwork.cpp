#include "NeuralNetwork.hpp"
#include <algorithm>

void NeuralNetwork ::loadWeights(const std::vector<float> &weights)
{
    int idx = 0;
    for (int i = 0; i < INPUT_SIZE; i++)
        for (int h = 0; h < HIDDEN_SIZE; h++)
            weightsIH[i][h] = weights[idx++];

    for (int h = 0; h < HIDDEN_SIZE; h++)
        for (int o = 0; o < OUTPUT_SIZE; o++)
            weightsHO[h][o] = weights[idx++];
}

float NeuralNetwork ::activate(float x)
{
    return tanh(x);
}

int NeuralNetwork ::compute(const std::vector<float> &inputs)
{

    float hidden[HIDDEN_SIZE] = {0};
    for (int h = 0; h < HIDDEN_SIZE; h++)
    {
        float sum = 0;
        for (int i = 0; i < INPUT_SIZE; i++)
            sum += inputs[i] * weightsIH[i][h];
        hidden[h] = activate(sum);
    }
    float outputs[OUTPUT_SIZE] = {0};
    for (int o = 0; o < OUTPUT_SIZE; o++)
    {
        float sum = 0;
        for (int h = 0; h < HIDDEN_SIZE; h++)
            sum += hidden[h] * weightsHO[h][o];
        outputs[o] = activate(sum);
    }
    return std::max_element(outputs, outputs + OUTPUT_SIZE) - outputs;
}