#pragma once
#include <vector>
#include <cmath>

class NeuralNetwork
{
public:
    static const int INPUT_SIZE = 9;
    static const int HIDDEN_SIZE = 6;
    static const int OUTPUT_SIZE = 5;
    static const int TOTAL_WEIGHTS = INPUT_SIZE * HIDDEN_SIZE + HIDDEN_SIZE * OUTPUT_SIZE;

    void loadWeights(const std::vector<float> &weights);
    int compute(const std::vector<float> &inputs);

private:
    float weightsIH[INPUT_SIZE][HIDDEN_SIZE];
    float weightsHO[HIDDEN_SIZE][OUTPUT_SIZE];

    float activate(float x);
};