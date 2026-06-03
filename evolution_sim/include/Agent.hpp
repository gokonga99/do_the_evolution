#pragma once

#include "Environment.hpp"
#include "Genome.hpp"

enum class AgentState
{
    Alive,
    Dying,
    Dead
};

class Agent
{
public:
    Agent(int height, int width);
    Agent(Genome &genome, int parentX, int parentY, int height, int width);

    void update(Environment &env, int mateX, int mateY);
    int getX() const;
    int getY() const;
    int getSize() const;
    int getPerception() const;
    Genome getGenome() const;
    AgentState getState() const;
    void moveTo(int newX, int newY, Environment &env);
    void scan(Environment &env, bool &foodFound, int &foodX, int &foodY);
    bool isReadyToMate();
    void ableToMate();
    void mate();

    float energy;

private:
    int x, y;
    Genome genome;
    int offspringCount = 0;
    float maxEnergy;
    int maxAge;
    int age;
    int matingCooldown;
    int dyingTimer;
    bool readyToMate;
    AgentState state;
};