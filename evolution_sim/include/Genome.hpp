#pragma once
#include <vector>

struct Genome
{
    int moveCooldown;
    int size;
    int perception;
    int fertility;

    bool hasNN;

    std::vector <float> nnWeights;
    /*
1.  energyPercent
2.  foodVisible
3.  nearestFoodDistance
4.  richFoodVisible
5.  nearestRichFoodDistance
6.  mateVisible
7.  nearestMateDistance
8.  readyToMate
9.  nearbyAgentCount
10. wallNorth
11. wallSouth
12. wallEast
13. wallWest
14. localFoodCount
15. biasX
16. biasY
    */
};
