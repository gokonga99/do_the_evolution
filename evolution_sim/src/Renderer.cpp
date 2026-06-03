#include "Renderer.hpp"
#include <iostream>
#include <vector>
#include "Simulation.hpp"

Renderer::Renderer(int w, int h, int cs)
    : width(w), height(h), cellsize(cs),
      window(sf::VideoMode(w * cs, h * cs), "Grid Test") {}

bool Renderer::isOpen()
{
    return window.isOpen();
}

void Renderer::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            window.close();
    }
}
void Renderer::draw(const Simulation &sim)
{
    sf::RectangleShape border(sf::Vector2f(width * cellsize, height * cellsize));
    sf::RectangleShape cell(sf::Vector2f(cellsize - 1, cellsize - 1));
    sf::CircleShape circle((cellsize * 0.4f));
    window.clear();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            cell.setPosition(x * cellsize, y * cellsize);
            cell.setFillColor(sf::Color(50, 50, 50));
            window.draw(cell);
        }
    }
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            if (sim.getEnv().hasFood(x, y))
            {
                circle.setFillColor(sf::Color::Green);
                circle.setOrigin(cellsize * 0.4f, cellsize * 0.4f);
                circle.setPosition(x * cellsize + cellsize * 0.5f, y * cellsize + cellsize * 0.5f);
                circle.setOutlineThickness(-1.f);
                circle.setOutlineColor(sf::Color::Green);
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
                moveCooldownColor = sf::Color(180, 0, 0);
            cell.setFillColor(moveCooldownColor);
        }
        window.draw(cell);
        
        sf::CircleShape eye(2); // small dot
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

    window.display();
}
