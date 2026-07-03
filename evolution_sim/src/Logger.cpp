#include "Logger.hpp"

Logger::Logger(const std::string &filename)
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
        << "AverageFertility,"
        << "foodDecisions,"
        << "rareFoodDecisions,"
        <<" mateDecisions,"
        << "wanderDecisions,"
        << "stayDecisions,"
        << "foodImpossible,"
        << "rareImpossible,"
        << "mateImpossible \n";
}

void Logger::log(
    int tick,
    int population,
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
    int mateImpossible)
{
    file
        << tick << ","
        << population << ","
        << avgEnergy << ","
        << avgAge << ","
        << avgSize << ","
        << avgPerception << ","
        << avgCooldown << ","
        << avgFertility << ","
        << foodDecisions << ","
        << rareFoodDecisions << ","
        << mateDecisions << ","
        << wanderDecisions << ","
        << stayDecisions << ","
        << foodImpossible << ","
        << rareImpossible << ","
        << mateImpossible << "\n";
}