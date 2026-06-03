#include <iostream>
#include "Renderer.hpp"
#include "Simulation.hpp"
#include <cstdlib>
#include <ctime>
#include <thread>

int main() {
    //replace rand with
    // std::mt19937
    // std::uniform_int_distribution
    std::srand(static_cast<unsigned int>(std::time(0)));

    int height=30;
    int width=30;
    float cellsize=25.f;
    int numberOfAgents=20;

    Simulation sim(numberOfAgents,height,width);
    Renderer renderer(width, height, cellsize);

    while (renderer.isOpen()) {
        renderer.handleEvents();
        sim.tick();
        renderer.draw(sim);
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
}