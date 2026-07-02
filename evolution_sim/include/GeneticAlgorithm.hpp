#pragma once

#include "Genome.hpp"
#include "NeuralNetwork.hpp"
#include <random>
#include <string>
#include <fstream>

class GeneticAlgorithm
{
public:
    Genome crossOver(const Genome &parent1, const Genome &parent2);
    void mutate(Genome &genome, std::string & weightsFile);
};