#include "engine.hpp"
#include "benchmark.cpp"

std::string g_windowName = "test";

engine::engine()
    :window(sf::VideoMode(g_windowWidth, g_windowHeight), g_windowName),
    spatialGrid(1.5f * g_particleRadius, g_windowWidth, g_windowHeight)
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
        else if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::S) {
                saveState(g_saveFileName);
            }
            else if (event.key.code == sf::Keyboard::L) {
                loadState(g_saveFileName);
            }
            else if (event.key.code == sf::Keyboard::Up) {
                GRAVITY = sf::Vector2f(0.f, -0.01f);
            }
            else if (event.key.code == sf::Keyboard::Down) {
                GRAVITY = sf::Vector2f(0.f, 0.01f);
            }
            else if (event.key.code == sf::Keyboard::Left) {
                GRAVITY = sf::Vector2f(-0.01f, 0.f);
            }
            else if (event.key.code == sf::Keyboard::Right) {
                GRAVITY = sf::Vector2f(0.01f, 0.f);
            }
            else if (event.key.code == sf::Keyboard::B) { 
                Benchmark benchmark;
                benchmark.executeBenchmark(*this, window); 
            }
            if (event.key.code == sf::Keyboard::Space) { 
                isSimulationRunning = !isSimulationRunning; 
            }
        }
        else if (event.type == sf::Event::MouseButtonPressed) {
            if (event.mouseButton.button == sf::Mouse::Middle) {
                isAttracting = true;
                attractionPoint = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
            }
            else if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)) {
                isAddingParticles = true;
            }
            else if (event.mouseButton.button == sf::Mouse::Right) {
                isDeletingParticles = true;
            }
        }
        else if (event.type == sf::Event::MouseButtonReleased) {
            if (event.mouseButton.button == sf::Mouse::Middle) {
                isAttracting = false;
            }
            else if (event.mouseButton.button == sf::Mouse::Left){
                isAddingParticles = false;
            }
            else if (event.mouseButton.button == sf::Mouse::Right){
                isDeletingParticles = false;
            }
        }
        else if (event.type == sf::Event::MouseMoved && isAttracting) {
            attractionPoint = sf::Vector2f(event.mouseMove.x, event.mouseMove.y);
        }
        else if (event.type == sf::Event::MouseMoved && isAddingParticles) {
            objects.push_back(particle(
                    sf::Vector2f(event.mouseMove.x, event.mouseMove.y),
                    sf::Vector2f(0.f, 0.f),
                    sf::Vector2f(0.f, 0.f),
                    g_particleRadius,
                    getColor(frameCount/100)
                ));
            particleCount++;
        }
        else if (event.type == sf::Event::MouseMoved && isDeletingParticles) {
            removeParticleAt(sf::Vector2f(event.mouseMove.x, event.mouseMove.y));
        }
    }
}

void engine::update()
{
    pipeTimer.restart();

    if (!isSimulationRunning) {
        physicsTime = pipeTimer.getElapsedTime().asMicroseconds();
        return; 
    }

    if (particleCount < g_maximumParticles)
    {
        createParticle(objects, frameCount);
        createParticle(objects, frameCount);
        particleCount++;
        particleCount++;
    }

    applyGravity(objects, GRAVITY);
    applyAttraction(objects, attractionPoint, isAttracting, g_attractionForce, g_attractionRadius);
    
    for (int i = 0; i < g_collisionCheckCount; i++)
    {
        checkBorders(objects);
        checkCollisions(objects, spatialGrid);
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

void engine::saveState(const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for saving: " << filename << std::endl;
        return;
    }

    size_t count = objects.size();
    file.write(reinterpret_cast<const char*>(&count), sizeof(count));

    for (const auto& p : objects) {
        file.write(reinterpret_cast<const char*>(&p.position), sizeof(p.position));
        file.write(reinterpret_cast<const char*>(&p.velocity), sizeof(p.velocity));
        file.write(reinterpret_cast<const char*>(&p.acceleration), sizeof(p.acceleration));
        file.write(reinterpret_cast<const char*>(&p.radius), sizeof(p.radius));
        file.write(reinterpret_cast<const char*>(&p.color), sizeof(p.color));
    }

    file.close();
    std::cout << "State saved to " << filename << std::endl;
}

void engine::loadState(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Failed to open file for loading: " << filename << std::endl;
        return;
    }

    objects.clear();
    particleCount = 0;

    size_t count;
    file.read(reinterpret_cast<char*>(&count), sizeof(count));

    for (size_t i = 0; i < count; ++i) {
        particle p({0,0}, {0,0}, {0,0}, 0, sf::Color::White);
        file.read(reinterpret_cast<char*>(&p.position), sizeof(p.position));
        file.read(reinterpret_cast<char*>(&p.velocity), sizeof(p.velocity));
        file.read(reinterpret_cast<char*>(&p.acceleration), sizeof(p.acceleration));
        file.read(reinterpret_cast<char*>(&p.radius), sizeof(p.radius));
        file.read(reinterpret_cast<char*>(&p.color), sizeof(p.color));

        objects.push_back(p);
        particleCount++;
    }

    file.close();
    std::cout << "State loaded from " << filename << std::endl;
}

void engine::clearParticles()
{
    particleCount = 0;
    objects = vector<particle>();
}

void engine::removeParticleAt(sf::Vector2f position) {
    for (auto it = objects.begin(); it != objects.end(); ) {
        float distanceSquared = (it->position.x - position.x) * (it->position.x - position.x) + 
                              (it->position.y - position.y) * (it->position.y - position.y);
        if (distanceSquared < it->radius * it->radius) {
            it = objects.erase(it);
            particleCount--;
        } else {
            ++it;
        }
    }
}