#pragma once

#include "parameters.hpp"

#include "updateST.cpp"

using namespace std;

class engine
{
private:
    sf::RenderWindow window;
    sf::Clock timer;
    
    vector<particle> objects;

    void handleEvents();
    void update();
    void render();
public:
    engine();
    void mainLoop();
};