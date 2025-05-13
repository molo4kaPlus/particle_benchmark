#include "engine.hpp"

std::string g_windowName = "test";

engine::engine()
    :window(sf::VideoMode(g_windowWidth, g_windowHeight), g_windowName)
{
    timer.restart();
    window.setFramerateLimit(30);
}

void engine::mainLoop()
{
    while (window.isOpen())
    {
        handleEvents();
        update();
        render();
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
    checkBorders(objects);
    checkCollisions(objects);
    
    objects.push_back(particle(
    sf::Vector2f{100.f, 300.f},   // Позиция
    sf::Vector2f{50.f, 100.f},    // Начальная скорость (вверх)
    sf::Vector2f{0.f, 0.f},       // Начальное ускорение
    10.f                          // Радиус
    ));
}

void engine::render()
{
    // setting up the circle that will be rendered thouthands of times!
    sf::CircleShape shape(30.f);
    shape.setFillColor(sf::Color::Blue);

    window.clear(sf::Color::Black);
    // actual render here
    for(particle obj : objects)
    {
        shape.setRadius(obj.radius);
        shape.setPosition(obj.position);
        window.draw(shape);
    }
    // 
    window.display();
}