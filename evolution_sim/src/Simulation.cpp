#include "Simulation.hpp"
#include <algorithm>
#include <iostream>
#include <climits>
#include <fstream>

Simulation::Simulation(int numAgents, int height, int width, std::string &weightsFile)
    : currentTick(0), env(height, width),
      numAgents(numAgents), weightsFile(weightsFile)
{
    agents.reserve(numAgents);

    for (int i = 0; i < numAgents; i++)
    {
        agents.emplace_back(height, width, weightsFile);
    }
}
int Simulation ::getPercentage(int value, int total)
{
    if (total == 0)
        return 0;
    if (value == 0)
        return 0;

    return (int)std::round((float)value / (float)total * 100.0f);
}
int Simulation::getCurrentTick() const
{
    return currentTick;
}
void Simulation ::saveToFile(const std::string &fileName)
{
    weightsFile = fileName;
    std::ofstream file(fileName);
    std::vector<Agent *> sorted;
    for (auto &agent : agents)
        sorted.push_back(&agent);
    std::sort(sorted.begin(), sorted.end(),
              [](Agent *a, Agent *b)
              { return a->getEnergy() > b->getEnergy(); });

    int count = std::min(1, (int)sorted.size());
    for (int i = 0; i < count; i++)
    {
        const auto &genome = sorted[i]->getGenome();
        file << genome.nnWeights.size() << '\n';

        for (float w : genome.nnWeights)
            file << w << ' ';

        file << '\n';
    }
    std::cout << "saved to file";
}
void Simulation::updateGenomeStats()
{
    genomeStats = {};

    if (agents.empty())
        return;

    for (const auto &agent : agents)
    {
        Genome g = agent.getGenome();

        genomeStats.avgEnergy += agent.getEnergy();
        if (agent.getGenome().hasNN)
            genomeStats.thinkerCount++;
        else if (!agent.getGenome().hasNN)
            genomeStats.wandererCount++;

        genomeStats.avgEnergy = genomeStats.avgEnergy / agents.size();
        genomeStats.avgSize += g.size;
        genomeStats.avgPerception += g.perception;
        genomeStats.avgMoveCooldown += g.moveCooldown;
        genomeStats.avgFertility += g.fertility;
    }

    float n = static_cast<float>(agents.size());

    genomeStats.avgSize /= n;
    genomeStats.avgSize = std::round(genomeStats.avgSize * 100.0f) / 100.0f;
    genomeStats.avgPerception /= n;
    genomeStats.avgPerception = std::round(genomeStats.avgPerception * 100.0f) / 100.0f;
    genomeStats.avgMoveCooldown /= n;
    genomeStats.avgMoveCooldown = std::round(genomeStats.avgMoveCooldown * 100.0f) / 100.0f;
    genomeStats.avgFertility /= n;
    genomeStats.avgFertility = std::round(genomeStats.avgFertility * 100.0f) / 100.0f;
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
                agents[i].mate();
                agents[j].mate();
                Genome childGenome = ga.crossOver(agents[i].getGenome(), agents[j].getGenome());
                ga.mutate(childGenome, weightsFile);
                Agent newborn(childGenome, agents[i].getX(), agents[i].getY(), env.getHeight(), env.getWidth());
                newborns.emplace_back(newborn);
                env.getStats().births++;
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

            int distance = std::max(abs(agents[i].getX() - agents[j].getX()), abs(agents[i].getY() - agents[j].getY()));
            if (distance <= agents[i].getPerception())
            {
                if (distance < nearestAgent)
                    for (auto &agent : agents)
                    {
                        nearestAgent = distance;
                        mateX = agents[j].getX();
                        mateY = agents[j].getY();
                    }
            }
        }

        agents[i].update(env, mateX, mateY, nearestAgent);
    }
    for (auto &child : newborns)
        agents.emplace_back(child);

    env.update();

    for (auto &agent : agents)
    {
        if (agent.getState() == AgentState::Dead)
        {
            if (agent.getGenome().hasNN)
                genomeStats.thinkerCount--;
            else if (agent.getGenome().hasNN)
                genomeStats.wandererCount--;
        }
    }

    agents.erase(
        std::remove_if(agents.begin(), agents.end(), [](const Agent &a)
                       { return a.getState() == AgentState::Dead; }),
        agents.end());

    updateGenomeStats();
    float totalEnergy = 0.0f;
    float totalAge = 0.0f;
    int aliveAgents = 0;

    for (const auto &agent : agents)
    {
        if (agent.getState() == AgentState::Alive)
        {
            totalAge += agent.getAge();
            totalEnergy += agent.getEnergy();
            aliveAgents++;
        }
    }

    if (aliveAgents > 0)
    {
        env.getStats().averageAge = totalAge / aliveAgents;
        env.getStats().averageEnergy = totalEnergy / aliveAgents;
    }

    else
    {
        env.getStats().averageAge = 0;
        env.getStats().averageEnergy = 0;
    }
    // logging
    if (currentTick % 100 == 0)
    {
        logger.log(
            currentTick,
            agents.size(),
            genomeStats.wandererCount,
            genomeStats.thinkerCount,
            env.getStats().averageEnergy,
            env.getStats().averageAge,
            genomeStats.avgSize,
            genomeStats.avgPerception,
            genomeStats.avgMoveCooldown,
            genomeStats.avgFertility,
            env.getStats().foodDecisions,
            env.getStats().rareFoodDecisions,
            env.getStats().mateDecisions,
            env.getStats().wanderDecisions,
            env.getStats().stayDecisions,
            env.getStats().foodImpossible,
            env.getStats().rareImpossible,
            env.getStats().mateImpossible);
    }
}
void Simulation ::logFinalStats()
{
    finalLog.log(
        currentTick,
        agents.size(),
        genomeStats.wandererCount,
        genomeStats.thinkerCount,
        env.getStats().averageEnergy,
        env.getStats().averageAge,
        genomeStats.avgSize,
        genomeStats.avgPerception,
        genomeStats.avgMoveCooldown,
        genomeStats.avgFertility,
        env.getStats().foodDecisions,
        env.getStats().rareFoodDecisions,
        env.getStats().mateDecisions,
        env.getStats().wanderDecisions,
        env.getStats().stayDecisions,
        env.getStats().foodImpossible,
        env.getStats().rareImpossible,
        env.getStats().mateImpossible);
}
const Environment &Simulation::getEnv() const
{
    return env;
}

const std::vector<Agent> &Simulation ::getAgents() const
{
    return agents;
}