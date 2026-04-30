#pragma once

#include "Environment.hpp"


enum class AgentState { Alive, Dying, Dead };

class Agent{
public:

    Agent(int height, int width);
    
    void update(Environment & env);
    int getX() const;
    int getY() const;
    int getSize() const;
    AgentState getState()  const;

private:
    int x, y;
    int energy;
    int maxEnergy;
    int speed;
    int age;
    int maxAge;
    int dyingTimer;
    int size;
    AgentState state;
};