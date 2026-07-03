#include "Renderer.hpp"
#include <iostream>
#include <vector>
#include <cmath>
#include "Simulation.hpp"

Renderer::Renderer(int w, int h, int cs, Simulation &sim)
    : width(w), height(h), cellsize(cs), sim(sim),
      window(sf::VideoMode(w * cs + 200, h * cs), "Evolution Sim")
{
    font.loadFromFile("assets/DejaVuSans.ttf");
    statsText.setFont(font);
    statsText.setCharacterSize(14);
    statsText.setFillColor(sf::Color::White);
}
int Renderer ::getPercentage(int value, int total)
{
    if (total == 0)
        return 0;
    if (value == 0)
        return 0;

    return (int)std::round((float)value / (float)total * 100.0f);
}

bool Renderer::isOpen()
{
    return window.isOpen();
}
int Renderer ::getSimSpeed()
{
    return simSpeed;
}
bool Renderer::isPaused()
{
    return paused;
}
void Renderer::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::S)
                sim.saveToFile("elite_genomes.txt");

            if (event.key.code == sf::Keyboard::Right)
                simSpeed = std::max(0, simSpeed - 50);

            if (event.key.code == sf::Keyboard::Left)
                simSpeed = std::min(500, simSpeed + 50);

            if (event.key.code == sf::Keyboard::Space)
                simSpeed = 0;

            if (event.key.code == sf::Keyboard::P)
                paused = !paused;
        }
    }
}

void Renderer::draw()
{
    if(sim.getCurrentTick()==19999)
        paused=true;

    sf::RectangleShape border(sf::Vector2f(width * cellsize, height * cellsize));
    sf::RectangleShape cell(sf::Vector2f(cellsize - 1, cellsize - 1));
    sf::CircleShape circle((cellsize * 0.4f));
    window.clear();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (sim.getEnv().checkObsitcleWall(x, y) == 1)
            {
                cell.setPosition(x * cellsize, y * cellsize);
                cell.setFillColor(sf::Color::White);
            }
            else
            {

                cell.setPosition(x * cellsize, y * cellsize);
                cell.setFillColor(sf::Color(50, 50, 50));
                window.draw(cell);
            }
        }
    }
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (sim.getEnv().getFoodType(x, y) == 1)
            {
                circle.setFillColor(sf::Color::Green);
                circle.setOrigin(cellsize * 0.4f, cellsize * 0.4f);
                circle.setPosition(x * cellsize + cellsize * 0.5f, y * cellsize + cellsize * 0.5f);
                circle.setOutlineThickness(-1.f);
                circle.setOutlineColor(sf::Color::Green);
                window.draw(circle);
            }
            else if (sim.getEnv().getFoodType(x, y) == 2)
            {
                circle.setFillColor(sf::Color::Blue);
                circle.setOrigin(cellsize * 0.4f, cellsize * 0.4f);
                circle.setPosition(x * cellsize + cellsize * 0.5f, y * cellsize + cellsize * 0.5f);
                circle.setOutlineThickness(-1.f);
                circle.setOutlineColor(sf::Color::Blue);
                window.draw(circle);
            }
        }
    }

    border.setFillColor(sf::Color::Transparent);
    border.setOutlineColor(sf::Color::Red);
    border.setOutlineThickness(-2.f);
    window.draw(border);

    for (auto &agent : sim.getAgents())
    {
        int x = agent.getX();
        int y = agent.getY();
        float scale = 1.f;

        if (agent.getSize() == 1)
            scale = 0.5f;
        else if (agent.getSize() == 2)
            scale = 0.7f;
        else if (agent.getSize() == 3)
            scale = 0.9f;

        sf::RectangleShape cell(sf::Vector2f(cellsize * scale, cellsize * scale));
        cell.setOrigin((cellsize * scale) * 0.5f, (cellsize * scale) * 0.5f);
        cell.setPosition(x * cellsize + cellsize * 0.5f, y * cellsize + cellsize * 0.5f);
        sf::Color moveCooldownColor;
        int moveCooldown = agent.getGenome().moveCooldown;

        if (agent.getState() == AgentState::Dying)
            cell.setFillColor(sf::Color(192, 192, 192));
        if (agent.getState() == AgentState::Alive)
        {

            if (moveCooldown == 1)
                moveCooldownColor = sf::Color(255, 200, 0); // yellow - fast
            else if (moveCooldown == 2)
                moveCooldownColor = sf::Color(180, 60, 0); // brown - medium
            else
                moveCooldownColor = sf::Color(180, 0, 0); // red -slow
            cell.setFillColor(moveCooldownColor);
        }
        window.draw(cell);

        if (agent.getGenome().hasNN)
        {
            sf::RectangleShape lens(sf::Vector2f(cellsize * 0.20f, cellsize * 0.15f));
            lens.setFillColor(sf::Color::Transparent);
            lens.setOutlineColor(sf::Color::White);
            lens.setOutlineThickness(1.50f);

            lens.setPosition(
                x * cellsize + cellsize * 0.2f,
                y * cellsize + cellsize * 0.2f);
            window.draw(lens);

            lens.setPosition(
                x * cellsize + cellsize * 0.6f,
                y * cellsize + cellsize * 0.2f);
            window.draw(lens);

            sf::RectangleShape bridge(sf::Vector2f(cellsize * 0.2f, cellsize * 0.1f));
            bridge.setFillColor(sf::Color::White);
            bridge.setPosition(
                x * cellsize + cellsize * 0.4f,
                y * cellsize + cellsize * 0.15f);
            window.draw(bridge);
        }

        sf::CircleShape eye(2);
        eye.setFillColor(sf::Color::White);

        if (agent.getGenome().perception == 1)
        {
            // one dot centered
            eye.setPosition(x * cellsize + cellsize / 2, y * cellsize + cellsize / 4);
            window.draw(eye);
        }
        else
        {
            // two dots side by side
            eye.setPosition(x * cellsize + cellsize / 3, y * cellsize + cellsize / 4);
            window.draw(eye);
            eye.setPosition(x * cellsize + cellsize * 2 / 3, y * cellsize + cellsize / 4);
            window.draw(eye);
        }
    }
    // collect stats from simulation
    int thinkerCount = 0;
    int wandererCount = 0;
    float thinkerEnergy = 0;
    float wandererEnergy = 0;

    for (auto &agent : sim.getAgents())
    {
        if (agent.getGenome().hasNN)
        {
            thinkerCount++;
            thinkerEnergy += agent.getEnergy();
        }
        else
        {
            wandererCount++;
            wandererEnergy += agent.getEnergy();
        }
    }

    float avgThinkerEnergy = thinkerCount > 0 ? thinkerEnergy / thinkerCount : 0;
    float avgWandererEnergy = wandererCount > 0 ? wandererEnergy / wandererCount : 0;

    auto &env = const_cast<Environment &>(sim.getEnv());
    auto &envStats = env.getStats();

    std::string statistics =
        "Tick: " + std::to_string(sim.getCurrentTick()) + "\n" +
        "Total agents: " + std::to_string(sim.getAgents().size()) + "\n" +
        "Average Age: " + std::to_string((int)envStats.averageAge) + "\n" +
        "Average Energy: " + std::to_string((int)envStats.averageEnergy) + "\n" +
        "Births: " + std::to_string(envStats.births) + "\n" +
        "Deaths: " + std::to_string(envStats.deaths) + "\n\n" +

        "Average Genome\n"
        "Size: " +
        std::to_string(sim.getGenomeStats().avgSize) + "\n"
                                                       "Perception: " +
        std::to_string(sim.getGenomeStats().avgPerception) + "\n"
                                                             "Cooldown: " +
        std::to_string(sim.getGenomeStats().avgMoveCooldown) + "\n"
                                                               "Fertility: " +
        std::to_string(sim.getGenomeStats().avgFertility) + "\n\n" +
        "Agent distribution \n" +
        "Wanderers: " + std::to_string(wandererCount) + "\n" +
        "Avg energy: " + std::to_string((int)avgWandererEnergy) + "\n" +

        "Thinkers: " +
        std::to_string(thinkerCount) + "\n" +
        "Avg energy: " + std::to_string((int)avgThinkerEnergy) + "\n\n" +

        "Thinker Decisions\n" +
        "Total: " + std::to_string(envStats.totalDecisions) + "\n" +

        "Move to Food: " + 
            std::to_string(getPercentage(envStats.foodDecisions, envStats.totalDecisions)) + "%\n" +

        "Move to RareFood: " + 
            std::to_string(getPercentage(envStats.rareFoodDecisions, envStats.totalDecisions)) + "%\n" +

        "Move to Mate: " + 
        std::to_string(getPercentage(envStats.mateDecisions, envStats.totalDecisions)) + "%\n" +

        "Wander randomly: " + 
            std::to_string(getPercentage(envStats.wanderDecisions, envStats.totalDecisions)) + "%\n" +

        "Stay still: " + 
            std::to_string(getPercentage(envStats.stayDecisions, envStats.totalDecisions)) + "%\n" +

        "food impossible: " + 
            std::to_string(getPercentage(envStats.foodImpossible, envStats.totalDecisions)) + "%\n" +

        "rare food impossible: " + 
            std::to_string(getPercentage(envStats.rareImpossible, envStats.totalDecisions)) + "%\n" +

        "mate impossible: " + 
            std::to_string(getPercentage(envStats.mateImpossible, envStats.totalDecisions)) + "%\n\n" +

        "Speed: " + std::to_string(simSpeed) + "ms\n\n";

    statsText.setString(statistics);
    statsText.setPosition(width * cellsize + 10, 10);
    window.draw(statsText);

    window.display();
}
