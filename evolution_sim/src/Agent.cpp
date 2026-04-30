#include "Agent.hpp"

Agent::Agent(int height, int width)
{
    age = 0;
    maxAge = 200;
    size = 1+rand() % 3;
    state = AgentState::Alive;
    dyingTimer = 0;
    x = rand() % width;
    y = rand() % height;
    speed = 1 + rand() % 3;
    maxEnergy = 100 * size;
    energy = maxEnergy;
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
    return size;
}
AgentState Agent::getState() const
{
    return state;
}

void Agent::update(Environment &env)
{
    energy -= size;
    age++;

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
        if (age % speed == 0)
        {
            int dx = (rand() % 3) - 1;
            int dy = (rand() % 3) - 1;

            if (env.isValidPosition(x + dx, y + dy))
            {
                x += dx;
                y += dy;
                energy -= size;
            }
        }
        if (env.hasFood(x, y))
        {
            env.consumeFood(x, y);
            energy += 10;
            if (energy > maxEnergy)
                energy = maxEnergy;
        }
    }
}