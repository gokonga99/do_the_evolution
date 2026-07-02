#pragma once

#include <fstream>

class Logger
{
public:
    Logger(const std::string& filename);

    void log(
        int tick,
        int population,
        float avgEnergy,
        float avgAge,
        float avgSize,
        float avgPerception,
        float avgCooldown,
        float avgFertility);

private:
    std::ofstream file;
};