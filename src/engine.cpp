#include "engine.hpp"

std::string g_windowName = "test";

static sf::Color getColor(float t) {
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * M_PI);
    const float b = sin(t + 0.66f * 2.0f * M_PI);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

engine::engine()
    :window(sf::VideoMode(g_windowWidth, g_windowHeight), g_windowName)
{
    timer.restart();
    window.setFramerateLimit(g_fpsLimit);
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
    float cTime = timer.getElapsedTime().asSeconds();

    if (particleCount < g_maximumParticles)
    {
        objects.push_back(particle(
            sf::Vector2f{100.f, 300.f},    
            sf::Vector2f{-1.f, 1.f}, 
            sf::Vector2f{0.f, 0.f},
            g_particleRadius,                  
            getColor(frameCount/100)
        ));
        particleCount++;
    }

    applyGravity(objects);
    for (int i = 0; i < g_collisionCheckCount; i++)
    {
        checkBorders(objects);
        checkCollisions(objects);
    }
}

void engine::render()
{
    sf::CircleShape shape(30.f);
    shape.setFillColor(sf::Color::Blue);

    window.clear(sf::Color::Black);
    // actual render here
    for(const auto& obj : objects)
    {
        shape.setRadius(obj.radius);
        shape.setPosition(obj.position.x - obj.radius, obj.position.y - obj.radius);
        shape.setFillColor(obj.color);
        window.draw(shape);
    }
    // 
    window.display();
    frameCount++;
}