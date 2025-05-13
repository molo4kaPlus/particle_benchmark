#pragma once

#include "parameters.hpp"

#include "updateST.cpp"

using namespace std;

class engine
{
private:
    sf::RenderWindow window;
    sf::Clock timer;

    int particleCount = 0;
    int frameCount = 0;

    vector<particle> objects;

    void handleEvents();
    void update();
    void render();
public:
    engine();
    void mainLoop();
};