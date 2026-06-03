#pragma once
#include <vector>
#include "Agent.hpp"
#include "Environment.hpp"
#include "GeneticAlgorithm.hpp"

class Simulation {
public:
    Simulation(int numAgents,int height, int width);
    // void run(int maxTicks);
    void tick();
    const Environment& getEnv() const;
    const std::vector<Agent>& getAgents() const;

private:
    int currentTick;
    int numAgents;
    Environment env;
    std::vector <Agent> agents;
    GeneticAlgorithm ga;


};