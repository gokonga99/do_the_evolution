#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Simulation.hpp"

class Renderer
{
public:
    Renderer(int width, int height, int cellSize, Simulation &sim);

    bool isOpen();
    void handleEvents();
    void draw();
    int getSimSpeed();
    bool isPaused();
    sf::Font font;
    sf::Text statsText;
    int getPercentage(int value, int total);
private:
    sf::RenderWindow window;
    int width;
    int height;
    float cellsize;
    Simulation &sim;
    int simSpeed=500;
    bool paused = false;
};