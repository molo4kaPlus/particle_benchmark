#pragma once

#include <iostream>

using namespace std;

struct BenchmarkResult {
    float avgPhysicsTime;    // Среднее время физики (мс)
    float avgRenderTime;     // Среднее время рендера (мс)
    float avgFPS;            // Средний FPS
    int particleCount;       // Количество частиц
    string mode = "OMP";            // "ST" или "OMP"
};