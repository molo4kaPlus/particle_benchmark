#include "engine.hpp"

int g_windowWidth = 800;
int g_windowHeight = 600;
std::string g_windowName = "test";

engine::engine()
    :window(sf::VideoMode(g_windowWidth, g_windowHeight), g_windowName)
{
    
}

void engine::mainLoop()
{
    while (window.isOpen())
    {
        handleEvents();
    }
}

void engine::handleEvents()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        window.close();
    }
}

void engine::update()
{
    // TODO
}

void engine::render()
{
    // TODO
}