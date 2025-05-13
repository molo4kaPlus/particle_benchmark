#include "engine.hpp"

std::string g_windowName = "test";

engine::engine()
    :window(sf::VideoMode(g_windowWidth, g_windowHeight), g_windowName)
{
    timer.restart();
    window.setFramerateLimit(g_fpsLimit);
    if (!font.loadFromFile("Arial.ttf")) {
        cout << "[SFML]: Could not load font!" << endl;
        window.close();
    }
    infoText.setFont(font);
    infoText.setCharacterSize(20); 
    infoText.setFillColor(sf::Color::White);
    infoText.setPosition(10, 10); 
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
    if (particleCount < g_maximumParticles)
    {
        createParticle(objects, frameCount);
        createParticle(objects, frameCount);
        particleCount++;
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
    // Обновление и отрисовка текста
    infoText.setString("Particles: " + std::to_string(particleCount) + 
                      "\nFPS: " + std::to_string(static_cast<int>(1.f / timer.restart().asSeconds())));
    window.draw(infoText);
    // 
    window.display();
    frameCount++;
}