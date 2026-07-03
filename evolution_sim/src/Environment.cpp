#include "Environment.hpp"
#include <iostream>

Environment::Environment(int height, int width)
    : height(height), width(width), rareFoodToAdd((height * width) / 10)
{

    foodRegenRate = (1 + rand() % 100) / 100.0;
    rareFoodToAdd = height * width / 20;

    foodGrid.assign(width, std::vector<int>(height, 0));
    obsticleWalls.assign(width, std::vector<int>(height, 0));
    // loadEnvironment(1);
    int i = 0;
    while (i < height * width * 0.20)
    {

        int x = rand() % width;
        int y = rand() % height;
        if (foodGrid[x][y] == 0)
        {
            foodGrid[x][y] = 1;
            i++;
        }
    }
}
int Environment::getHeight() const
{
    return height;
}
int Environment::getWidth() const
{
    return width;
}
int Environment::checkObsitcleWall(int x, int y) const
{
    return obsticleWalls[x][y];
}

bool Environment::isValidPosition(int x, int y) const
{
    if (x < 0 || x >= width || y < 0 || y >= height)
        return false;
    else if (obsticleWalls[x][y] == 1)
        return false;
    return true;
}
int Environment::getFoodType(int x, int y) const
{
    if (!isValidPosition(x, y))
        return 0;

    return foodGrid[x][y];
}

void Environment::consumeFood(int x, int y)
{
    if (foodGrid[x][y] == 1)
        foodGrid[x][y] = 0;
    else if (foodGrid[x][y] == 2)
    {
        foodGrid[x][y] = 0;
        rareFoodToAdd++;
    }
}
void Environment::loadEnvironment(int type)
{
    int corridorWidth = 4;
    int entryWidth = 2;
    int x = 6;

    bool goingDown = true;
    while (x + corridorWidth < width)
    {
        int wallX1 = x;
        int wallX2 = x + corridorWidth;

        for (int y = 0; y < height; y++)
        {
            bool isEntryway = goingDown
                                  ? (y >= height - entryWidth)
                                  : (y < entryWidth);

            if (!isEntryway)
            {
                if (wallX1 >= 0 && wallX1 < width)
                    obsticleWalls[wallX1][y] = 1;
            }
        }

        x += corridorWidth;
        goingDown = !goingDown;
    }
}

void Environment::update()
{
    currentTick++;
    if (stats.totalDecisions >= 20000)
    {
        stats.totalDecisions /= 10;
        stats.foodDecisions /= 10;
        stats.rareFoodDecisions /= 10;
        stats.mateDecisions /= 10;
        stats.wanderDecisions /= 10;
        stats.stayDecisions /= 10;
        stats.foodImpossible /= 10;
        stats.rareImpossible /= 10;
        stats.mateImpossible /= 10;


    }

    if (currentTick % 10 == 0)
    {
        int foodToAdd = (width * height / 15);
        int attempts = 0;

        while (foodToAdd > 0 && attempts < 1000)
        {
            int x = rand() % width;
            int y = rand() % height;
            if (foodGrid[x][y] == 0 && obsticleWalls[x][y] != 1)
            {

                foodGrid[x][y] = 1;
                foodToAdd--;
            }
            attempts++;
        }
        int i=0;
        while (rareFoodToAdd > 0 && i<=rareFoodToAdd/10)
        {
            i++;   
            int x = rand() % width;
            int y = rand() % height;
            if (foodGrid[x][y] == 0 && obsticleWalls[x][y] != 1)
            {
                foodGrid[x][y] = 2;
                rareFoodToAdd--;
            }
        }
    }
}
