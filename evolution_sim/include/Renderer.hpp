#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Simulation.hpp"

class Renderer {
public:
    Renderer(int width, int height, int cellSize);

    bool isOpen();
    void handleEvents();
    void draw(const Simulation& sim);

private:
    sf::RenderWindow window;
    int width;
    int height;
    float cellsize;
};