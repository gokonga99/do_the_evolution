#pragma once
#include <iostream>
#include <vector>

class Environment {
public:
    Environment(int height, int width);
    
    int getWidth() const;
    int getHeight() const;
    bool isValidPosition(int x, int y) const;
    bool hasFood(int x,int y) const;
    void consumeFood(int x,int y);
    void update();
private:
    int height,width;
    double foodRegenRate;
    int currentTick;
    std::vector<std::vector<int>> foodGrid;
};