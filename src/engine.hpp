#pragma once

#include <SFML/Window.hpp>
#include <string>

using namespace std;

class engine
{
private:
    sf::Window window;

    void handleEvents();
    void update();
    void render();
public:
    engine();
    void mainLoop();
};