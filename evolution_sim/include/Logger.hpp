#pragma once

#include <fstream>

class Logger
{
public:
    Logger(const std::string &filename);

    void log(
        int tick,
        int population,
        int wandererCount,
        int thinkerCount,
        float avgEnergy,
        float avgAge,
        float avgSize,
        float avgPerception,
        float avgCooldown,
        float avgFertility,
        int foodDecisions,
        int rareFoodDecisions,
        int mateDecisions,
        int wanderDecisions,
        int stayDecisions,
        int foodImpossible,
        int rareImpossible,
        int mateImpossible);

private:
    std::ofstream file;
};