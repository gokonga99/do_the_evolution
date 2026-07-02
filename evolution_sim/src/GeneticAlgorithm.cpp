#include "GeneticAlgorithm.hpp"

Genome GeneticAlgorithm ::crossOver(const Genome &parent1, const Genome &parent2)
{
    Genome childGenome;
    childGenome.perception = (rand() % 2) ? parent1.perception : parent2.perception;
    childGenome.size = (rand() % 2) ? parent1.size : parent2.size;
    childGenome.moveCooldown = (rand() % 2) ? parent1.moveCooldown : parent2.moveCooldown;
    childGenome.fertility = (rand() % 2) ? parent1.fertility : parent2.fertility;
    childGenome.hasNN = (rand() % 2) ? parent1.hasNN : parent2.hasNN;

    if (parent1.hasNN && parent2.hasNN)
    {
        int crossOverPoint = rand() % NeuralNetwork ::TOTAL_WEIGHTS;
        childGenome.nnWeights.resize(NeuralNetwork::TOTAL_WEIGHTS);
        for (int i = 0; i < NeuralNetwork::TOTAL_WEIGHTS; i++)
            childGenome.nnWeights[i] = i < crossOverPoint ? parent1.nnWeights[i] : parent2.nnWeights[i];
    }
    else if (parent1.hasNN)
        childGenome.nnWeights = parent1.nnWeights;
    else if (parent2.hasNN)
        childGenome.nnWeights = parent2.nnWeights;

    return childGenome;
}

void GeneticAlgorithm::mutate(Genome &genome, std::string &weightsFile)
{
    int mutationRate = 10;
    if (rand() % 100 < mutationRate)
        genome.moveCooldown += (rand() % 3) - 1;
    if (rand() % 100 < mutationRate)
        genome.size += (rand() % 3) - 1;
    if (rand() % 100 < mutationRate)
        genome.perception += (rand() % 3) - 1;
    if (rand() % 100 < mutationRate)
        genome.fertility += (rand() % 3) - 1;

    genome.moveCooldown = std::max(1, std::min(3, genome.moveCooldown));
    genome.size = std::max(1, std::min(3, genome.size));
    genome.perception = std::max(1, std::min(2, genome.perception));
    genome.fertility = std ::max(1, std::min(3, genome.fertility));

    // if (rand() % 100 < 1)
    // {
    //     genome.hasNN = !genome.hasNN;
    //     if (genome.hasNN)
    //     {
    //         std::ifstream file(weightsFile);

    //         size_t weightCount;
    //         file >> weightCount;

    //         genome.nnWeights.resize(weightCount);

    //         for (size_t j = 0; j < weightCount; j++)
    //             file >> genome.nnWeights[j];
    //     }
    // }
    if (genome.hasNN)
    {
        for (auto &w : genome.nnWeights)
        {
            if (rand() % 100 < mutationRate)
                w += ((rand() % 200) - 100) / 100.0f;
            w = std::max(-1.0f, std::min(1.0f, w));
        }
    }
}