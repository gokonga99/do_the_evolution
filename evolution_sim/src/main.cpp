#include <iostream>
#include "Renderer.hpp"
#include "Simulation.hpp"
#include <cstdlib>
#include <ctime>
#include <thread>
#include <string>

int main()
{
    std::srand(static_cast<unsigned int>(std::time(0)));

    int height = 40;
    int width = 60;
    float cellsize = 25.f;
    int numberOfAgents =60;
    std::string weightsfile="elite_genomes.txt";
    Simulation sim(numberOfAgents, height, width,weightsfile);
    Renderer renderer(width, height, cellsize, sim);

    while (renderer.isOpen())
    {
        renderer.handleEvents();
        if (!renderer.isPaused())
        {
            sim.tick();
        }
        renderer.draw();
        std::this_thread::sleep_for(std::chrono::milliseconds(renderer.getSimSpeed()));
    }
    sim.logFinalStats();
}