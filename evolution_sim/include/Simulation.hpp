#pragma once
#include <vector>
#include "Agent.hpp"
#include "Environment.hpp"
#include "GeneticAlgorithm.hpp"
#include "Logger.hpp"
#include <string>


struct GenomeStats
{
    float avgSize = 0;
    float avgPerception = 0;
    float avgMoveCooldown = 0;
    float avgFertility = 0;
};

class Simulation
{
public:
    Simulation(int numAgents, int height, int width, std::string &weightsFile);
    // void run(int maxTicks);
    void tick();
    const Environment &getEnv() const;
    const std::vector<Agent> &getAgents() const;
    int getCurrentTick() const;
    void saveToFile(const std::string &fileName);
    void updateGenomeStats();
    void logFinalStats();
    GenomeStats getGenomeStats(){
        return genomeStats;
    }

private:
    int currentTick;
    int numAgents;
    Environment env;
    std::vector<Agent> agents;
    std::string &weightsFile;
    GeneticAlgorithm ga;
    GenomeStats genomeStats;
    Logger logger{"simulation.csv"};
    Logger finalLog{"simulationFinalLog.csv"};
};