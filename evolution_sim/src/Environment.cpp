#include "Environment.hpp"
#include <iostream>

Environment::Environment(int height, int width)
    : height(height), width(width)
{

    
    foodRegenRate = (1 + rand() % 100) / 100.0;

    foodGrid.assign(width, std::vector<int>(height, 0));
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

bool Environment::isValidPosition(int x, int y) const
{
    return x >= 0 && x < width && y >= 0 && y < height;
}
bool Environment::hasFood(int x, int y) const
{
    if (isValidPosition(x, y))
    {
        return foodGrid[x][y] > 0;
    }
    return false;
}

void Environment::consumeFood(int x, int y)
{
    foodGrid[x][y] = 0;
}

void Environment::update()
{
    currentTick++;

    if (currentTick % 10 == 0)
    {
        int foodToAdd=(width*height/30);
        int attempts = 0;

        while (foodToAdd >0 && attempts<100)
        {
            int x = rand() % width;
            int y = rand() % height;
            if (foodGrid[x][y] == 0)
            {
                foodGrid[x][y] = 1;
                foodToAdd--;
            }
            attempts++;
        }
    }
}
