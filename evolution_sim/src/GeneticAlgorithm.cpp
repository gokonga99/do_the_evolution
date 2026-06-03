#include "GeneticAlgorithm.hpp"

Genome GeneticAlgorithm ::crossOver(const Genome &parent1,const Genome &parent2)
{
    Genome childGenome;
    childGenome.perception=(rand()%2)? parent1.perception : parent2.perception;
    childGenome.size=(rand()%2) ? parent1.size : parent2.size;
    childGenome.moveCooldown=(rand()%2) ? parent1.moveCooldown : parent2.moveCooldown;

    return childGenome;
}

void GeneticAlgorithm::mutate(Genome& genome){
    int mutationRate = 10;
    if(rand() % 100 < mutationRate)
        genome.moveCooldown += (rand() % 3) - 1;
    if(rand() % 100 < mutationRate)
        genome.size += (rand() % 3) - 1;
    if(rand() % 100 < mutationRate)
        genome.perception += (rand() % 3) - 1;
    
    genome.moveCooldown = std::max(1, std::min(3, genome.moveCooldown));
    genome.size = std::max(1, std::min(3, genome.size));
    genome.perception = std::max(1, std::min(2, genome.perception));
}
