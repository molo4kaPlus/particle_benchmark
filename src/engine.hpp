#pragma once

#include "parameters.hpp"

#include "updateOpenMP.cpp"
//#include "updateST.cpp"

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

    BenchmarkResult benchmarkData;
    bool isBenchmarkRunning = false;
    int benchmarkFrameCount = 0;
    float physicsTimeSum = 0;
    float renderTimeSum = 0;
    float fpsSum = 0;

    void handleEvents();
    void update();
    void render();
    void saveBenchmarkResults();

public:
    engine();
    void mainLoop();
};