#include "Simulation.hpp"
#include <algorithm>
#include <iostream>
#include <climits>

Simulation::Simulation(int numAgents, int height, int width)
    : currentTick(0), env(height, width), numAgents(numAgents)
{
    agents.reserve(numAgents);

    for (int i = 0; i < numAgents; i++)
    {
        agents.emplace_back(height, width);
    }
}

void Simulation::tick()
{
    currentTick++;
    std::vector<Agent> newborns;

    for (int i = 0; i < agents.size(); i++)
    {
        for (int j = i + 1; j < agents.size(); j++)
        {
            if (agents[i].getX() == agents[j].getX() &&
                agents[i].getY() == agents[j].getY() &&
                agents[i].isReadyToMate() && agents[j].isReadyToMate())
            {
                std::cout << "agents are on the same cell ";

                agents[i].mate();
                agents[j].mate();
                Genome childGenome = ga.crossOver(agents[i].getGenome(), agents[j].getGenome());
                ga.mutate(childGenome);
                // ga.mutate(childGenome);
                Agent newborn(childGenome, agents[i].getX(), agents[i].getY(), env.getHeight(), env.getWidth());
                newborns.emplace_back(newborn);
                break;
            }
        }
    }
    for (size_t i = 0; i < agents.size(); i++)
    {
        int nearestAgent = INT_MAX;
        int mateX = -1;
        int mateY = -1;
        for (size_t j = 0; j < agents.size(); j++)
        {
            if (i == j)
                continue;

            int distance = abs(agents[i].getX() - agents[j].getX()) + abs(agents[i].getY() - agents[j].getY());

            if (distance < nearestAgent && distance <= agents[i].getPerception())
            {
                nearestAgent = distance;
                mateX = agents[j].getX();
                mateY = agents[j].getY();
            }
        }
        agents[i].update(env, mateX, mateY);
    }
    for (auto &child : newborns)
        agents.emplace_back(child);

    env.update();

    agents.erase(
        std::remove_if(agents.begin(), agents.end(), [](const Agent &a)
                       { return a.getState() == AgentState::Dead; }),
        agents.end());

    // clean up agents

    float avgEnergy = 0;
    for (auto &agent : agents)
    {
        avgEnergy += agent.energy;
    }
    avgEnergy = avgEnergy / agents.size();

    std::cout << "Tick: " << currentTick
              << "| Agents: " << agents.size()
              << "| Agents average energy " << avgEnergy
              << std::endl;
}
const Environment &Simulation::getEnv() const
{
    return env;
}

const std::vector<Agent> &Simulation ::getAgents() const
{
    return agents;
}