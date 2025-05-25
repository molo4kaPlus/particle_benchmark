#pragma once
#include <iostream>

#include "parameters.hpp"
#include "engine.hpp"

using namespace std;

class engine;

struct Benchmark {
    vector<float> physicsTimes;      
    vector<float> renderTimes;       
    vector<float> fpsValues;         
    float avgPhysicsTime;           
    float avgRenderTime;            
    float avgFPS;                   
    float totalTime;                
    int particleCount;  
    int frames = 0;            
    sf::Clock benchClock;
    sf::Clock frameClock;
    string mode;           

    void executeBenchmark(engine &engine, sf::Window &window)
    {
        #ifdef OMP
            mode = "OMP";
        #else
            mode = "ST";
        #endif
        benchClock.restart();
        int frames = 0;
        
        while(window.isOpen() && frames < g_benchmarkFrames)
        {
            frameClock.restart();
            
            engine.handleEvents();
            engine.update();
            engine.render();
            
            // Collect performance data
            physicsTimes.push_back(engine.getPhysicsTime());
            renderTimes.push_back(engine.getRenderTime());
            
            float frameTime = frameClock.getElapsedTime().asMicroseconds();
            if(frameTime > 0) {
                fpsValues.push_back(1000000.0f / frameTime);
            }
            
            frames++;
            particleCount = engine.getParticleCount();
        }
        totalTime = benchClock.getElapsedTime().asMicroseconds();
        
        countAVG();
        saveResults(g_benchResultFileName);
    }

    float countAVG()
    {
        avgPhysicsTime = 0;
        for(auto t : physicsTimes) {
            avgPhysicsTime += t;
        }
        avgPhysicsTime = (physicsTimes.empty() ? 0 : avgPhysicsTime / physicsTimes.size()) / 1000.0f;
        
        avgRenderTime = 0;
        for(auto t : renderTimes) {
            avgRenderTime += t;
        }
        avgRenderTime = (renderTimes.empty() ? 0 : avgRenderTime / renderTimes.size()) / 1000.0f;
        
        avgFPS = 0;
        for(auto fps : fpsValues) {
            avgFPS += fps;
        }
        avgFPS = (fpsValues.empty() ? 0 : avgFPS / fpsValues.size());
        
        return avgFPS;
    }

    void saveResults(string filename)
    {
        bool fileExists = false;
        std::ifstream infile(filename);
        fileExists = infile.good();
        infile.close();

        std::ofstream file(filename, std::ios::app);
        if (!file.is_open()) {
            std::cerr << "Failed to open benchmark results file!" << std::endl;
            return;
        }

        if (!fileExists) {
            file << "Mode,Particles,CollisionChecks,Avg Physics (ms),Avg Render (ms),AVGfps,Total Time (s)\n";
        }

        file << mode << ","
            << particleCount << ","
            << g_collisionCheckCount << ","
            << avgPhysicsTime << ","
            << avgRenderTime << ","
            << avgFPS << ","
            << totalTime / 1000000.0f << "\n";
        
        file.close();
        std::cout << "Benchmark results saved to " << filename << std::endl;
    }
};