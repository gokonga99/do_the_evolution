#pragma once
#include <iostream>
#include <vector>

struct SimulationStats
{
    int totalDecisions = 1;

    int foodDecisions = 0;
    int rareFoodDecisions = 0;
    int mateDecisions = 0;
    int wanderDecisions = 0;
    int stayDecisions = 0;
    int foodImpossible = 0;
    int rareImpossible = 0;
    int mateImpossible = 0;
    
    int births = 0;
    int deaths = 0;

    float averageAge = 0.0f;
    float averageEnergy=0.0f;
};

class Environment
{
public:
    Environment(int height, int width);

    int getWidth() const;
    int getHeight() const;
    bool isValidPosition(int x, int y) const;
    int getFoodType(int x, int y) const;
    bool hasAgent(int x, int y) const;
    void consumeFood(int x, int y);
    void loadEnvironment(int type);
    void update();
    int checkObsitcleWall(int x, int y) const;
    SimulationStats &getStats()
    {
        return stats;
    }

private:
    int height, width;
    int rareFoodToAdd;
    double foodRegenRate;
    int currentTick;
    int type;
    std::vector<std::vector<int>> foodGrid;
    std::vector<std::vector<int>> obsticleWalls;
    SimulationStats stats;
};