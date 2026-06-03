#pragma once

#include "Genome.hpp"
#include <random>

class GeneticAlgorithm
{
public:
    Genome crossOver(const Genome &parent1, const Genome &parent2);
    void mutate(Genome &genome);
};