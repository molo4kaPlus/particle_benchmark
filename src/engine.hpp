#pragma once

#include "parameters.hpp"

#include "updateOpenMP.cpp"

using namespace std;

class engine
{
private:
    sf::RenderWindow window;
    sf::Clock timer;
    sf::Clock pipeTimer;
    sf::Font font;
    sf::Text infoText;

    int particleCount = 0;
    int frameCount = 0;
    float physicsTime = 0;
    float renderTime = 0;

    vector<particle> objects;

    void handleEvents();
    void update();
    void render();

public:
    engine();
    void mainLoop();
};