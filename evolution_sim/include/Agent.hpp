#pragma once
#include <climits>
#include "Environment.hpp"
#include "Genome.hpp"
#include "NeuralNetwork.hpp"
#include <string>
#include <fstream>

struct ScanResult
{
    bool foodVisible = false;
    int foodX = -1;
    int foodY = -1;
    int distanceToFood = INT_MAX;
    bool rareFoodVisible = false;
    int rareFoodX = -1;
    int rareFoodY = -1;
    int distanceToRareFood = INT_MAX;
    float localFoodCount = 0;
};

enum class AgentState
{
    Alive,
    Dying,
    Dead
};

class Agent
{
public:
    Agent(int height, int width, std::string &weightsFile);
    Agent(Genome &genome, int parentX, int parentY, int height, int width);

    void update(Environment &env, int mateX, int mateY, int mateDistance);
    int getX() const;
    int getY() const;
    int getSize() const;
    int getPerception() const;
    Genome getGenome() const;
    AgentState getState() const;
    int getAge() const;
    void moveTo(int newX, int newY, Environment &env);
    ScanResult scan(Environment &env);
    bool isReadyToMate();
    void mate();
    void eat(Environment &env);
    void randomWalk(Environment &env);
    void move(int dx, int dy, Environment &env);
    float getEnergy() const
{
    return energy;
}

private:
    int x, y;
    Genome genome;
    NeuralNetwork nn;
    int offspringCount = 0;
    float maxEnergy;
    int maxAge;
    int age;
    int matingCooldown;
    int dyingTimer;
    bool readyToMate;
    float energy;
    AgentState state;

};