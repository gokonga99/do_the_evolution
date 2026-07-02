#include "Logger.hpp"

Logger::Logger(const std::string& filename)
{
    file.open(filename);

    file
    << "Tick,"
    << "Population,"
    << "AverageEnergy,"
    << "AverageAge,"
    << "AverageSize,"
    << "AveragePerception,"
    << "AverageCooldown,"
    << "AverageFertility\n";
}

void Logger::log(
    int tick,
    int population,
    float avgEnergy,
    float avgAge,
    float avgSize,
    float avgPerception,
    float avgCooldown,
    float avgFertility)
{
    file
    << tick << ","
    << population << ","
    << avgEnergy << ","
    << avgAge << ","
    << avgSize << ","
    << avgPerception << ","
    << avgCooldown << ","
    << avgFertility
    << "\n";
}