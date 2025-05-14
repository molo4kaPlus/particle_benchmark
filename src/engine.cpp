#include "engine.hpp"
#include "benchmark.cpp"

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
    #ifdef BENCH
        Benchmark benchmark;
        benchmark.executeBenchmark(*this, window);
    #else
        while (window.isOpen())
        {
            handleEvents();
            update();
            render();
        }
    #endif
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
    pipeTimer.restart();

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

    physicsTime = pipeTimer.getElapsedTime().asMicroseconds();
}

void engine::render()
{
    pipeTimer.restart();

    sf::CircleShape shape(30.f);
    shape.setFillColor(sf::Color::Blue);

    window.clear(sf::Color::Black);
    
    // Render particles
    for(const auto& obj : objects)
    {
        shape.setRadius(obj.radius);
        shape.setPosition(obj.position.x - obj.radius, obj.position.y - obj.radius);
        shape.setFillColor(obj.color);
        window.draw(shape);
    }

    // Render performance info
    float currentFPS = 0.0f;
    if (renderTime > 0 && physicsTime > 0) {
        currentFPS = 1000000.0f / (renderTime + physicsTime);
    }

    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);
    oss << "FPS: " << currentFPS << "\n"
        << "Particles: " << particleCount << "\n"
        << "Physics: " << physicsTime / 1000.0f << " ms\n"
        << "Render: " << renderTime / 1000.0f << " ms";

    infoText.setString(oss.str());
    window.draw(infoText);

    window.display();
    frameCount++;

    renderTime = pipeTimer.getElapsedTime().asMicroseconds();
}