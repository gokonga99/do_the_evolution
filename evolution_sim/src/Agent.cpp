#include "Agent.hpp"
#include <climits>

Agent::Agent(int height, int width)
{
    age = 0;
    genome.size = 1 + rand() % 3;
    genome.perception = 1 + rand() % 2;
    genome.moveCooldown = 1 + rand() % 3;
    maxAge = 250 + genome.moveCooldown * 50;
    matingCooldown = 41;

    state = AgentState::Alive;
    dyingTimer = 0;
    x = rand() % width;
    y = rand() % height;
    maxEnergy = 120 * genome.size;
    energy = maxEnergy;
    readyToMate = false;
}
Agent::Agent(Genome &genome, int parentX, int parentY, int height, int width)
{
    age = 0;
    this->genome = genome;
    maxAge = 200 + genome.moveCooldown * 200;
    x = parentX;
    y = parentY;
    matingCooldown = 41;
    maxEnergy = 120 * genome.size;
    energy = 50;
    state = AgentState::Alive;
    dyingTimer = 0;
    readyToMate = false;
}

int Agent::getX() const
{
    return x;
}
int Agent::getY() const
{
    return y;
}
int Agent::getSize() const
{
    return genome.size;
}
AgentState Agent::getState() const
{
    return state;
}
Genome Agent::getGenome() const
{
    return genome;
}
int Agent::getPerception() const
{
    return genome.perception;
}
bool Agent::isReadyToMate()
{
    return readyToMate;
}

void Agent::ableToMate()
{
    if (energy > maxEnergy / 2 && matingCooldown == 0)
    {
        readyToMate = true;
        std::cout << "agent flagged as ready enegy=" << energy << "\n";
    }
}
void Agent::mate()
{

    matingCooldown = 61;
    energy -= 25;
    readyToMate = false;
    if (energy < 0)
        energy = 0;
    std::cout << "agent is born! \n";
}

void Agent::moveTo(int newX, int newY, Environment &env)
{
    if (env.isValidPosition(newX, newY))
    {
        if (x < newX)
            x++;
        else if (x > newX)
            x--;
        if (y < newY)
            y++;
        else if (y > newY)
            y--;
        energy -= (float)genome.size;
    }
    if (env.hasFood(x, y))
    {
        env.consumeFood(x, y);
        energy += 15.0f + (float)genome.size*2.0f;
        if (energy > maxEnergy)
            energy = maxEnergy;
    }
}
void Agent::scan(Environment &env, bool &foodVisable, int &foodX, int &foodY)
{
    energy -= genome.perception - 1;
    int tempFoodX = x + genome.perception;
    int tempFoodY = y + genome.perception;
    int distanceToFood = INT_MAX;

    int startX, endX, stepX;
    int startY, endY, stepY;

    if (rand() % 2)
    {
        startX = x - genome.perception;
        endX = x + genome.perception;
        stepX = 1;
    }
    else
    {
        startX = x + genome.perception;
        endX = x - genome.perception;
        stepX = -1;
    }

    if (rand() % 2)
    {
        startY = y - genome.perception;
        endY = y + genome.perception;
        stepY = 1;
    }
    else
    {
        startY = y + genome.perception;
        endY = y - genome.perception;
        stepY = -1;
    }

    for (int i = startX; i != endX + stepX; i += stepX)
    {
        for (int j = startY; j != endY + stepY; j += stepY)
        {
            if (env.hasFood(i, j))
            {
                int distance = abs(i - x) + abs(j - y);
                if (distanceToFood > distance)
                {
                    tempFoodX = i;
                    tempFoodY = j;
                    distanceToFood = distance;

                    foodVisable = true;
                }
            }
        }
    }
    if (foodVisable == true)
    {
        foodX = tempFoodX;
        foodY = tempFoodY;
    }
}

void Agent::update(Environment &env, int mateX, int mateY)
{

    energy -= 1.0f;
    age++;
    bool mateVisable = (mateX != -1);

    if (energy < maxEnergy / 2)
        readyToMate = false;
    else
        ableToMate();

    if (state == AgentState::Dying)
    {
        dyingTimer++;
        if (dyingTimer >= 3)
            state = AgentState::Dead;
        return;
    }

    if (age > maxAge || energy <= 0)
    {
        state = AgentState::Dying;
        return;
    }
    if (age <= maxAge && energy > 0)
    {
        if (matingCooldown > 0)
            matingCooldown--;

        if (age % genome.moveCooldown == 0)
        {
            bool foodVisable = false;
            int foodX, foodY;

            scan(env, foodVisable, foodX, foodY);

            int distanceToFood = foodVisable
                                     ? abs(x - foodX) + abs(y - foodY)
                                     : INT_MAX;

            int distanceToMate = (mateVisable && readyToMate)
                                     ? abs(x - mateX) + abs(y - mateY)
                                     : INT_MAX;

            if (distanceToFood <= distanceToMate && foodVisable && energy <= maxEnergy * 0.8)
                moveTo(foodX, foodY, env);
            else if (mateVisable && readyToMate)
                moveTo(mateX, mateY, env);
            else
            {
                int rdx = (rand() % 3) - 1;
                int rdy = (rand() % 3) - 1;
                if (env.isValidPosition(x + rdx, y + rdy))
                {
                    x += rdx;
                    y += rdy;
                    if (rdx != 0 || rdy != 0)
                        energy -= (float)genome.size;

                    if (env.hasFood(x, y))
                    {
                        env.consumeFood(x, y);
                        energy += 15.0f + (float)genome.size;
                        if (energy > maxEnergy)
                            energy = maxEnergy;
                    }
                }
            }
        }
    }
}
