#pragma once

#include "parameters.hpp"
//#include "benchmark.cpp"

#ifdef OMP
    #include "updateOpenMP.cpp"
#else
    #include "updateST.cpp"
#endif

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

    bool isAttracting = false;
    bool isSimulationRunning = true;
    bool isBenchmarkRunning = false;
    sf::Vector2f attractionPoint;
    sf::Vector2f GRAVITY = sf::Vector2f(0.f, 0.04f);

public:
    engine();
    void handleEvents();
    void update();
    void render();
    void mainLoop();

    float getPhysicsTime() const { return physicsTime; }
    float getRenderTime() const { return renderTime; }
    int getParticleCount() const { return particleCount; }
    void saveState(const std::string& filename);
    void loadState(const std::string& filename);
};