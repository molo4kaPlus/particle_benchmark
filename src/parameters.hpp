#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "particle.hpp"
//#include "benchmark.cpp"
#include "qsqrt.cpp"

#include <string>
#include <vector>
#include <cmath>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <fstream>

#define OMP

const int g_windowWidth = 800;
const int g_windowHeight = 600;
const int g_fpsLimit = 120;

const int g_maximumParticles = 1000;
const float g_particleRadius = 5.f;
const int g_collisionCheckCount = 20;

const float g_particleCollisionStrength = 1.55f;
const float g_damp = 0.75f;

const float g_attractionForce = 0.3f; 
const float g_attractionRadius = 200.f; 

const int g_benchmarkFrames = 1000; 
const std::string g_benchResultFileName = "results.csv"; 
const std::string g_saveFileName = "particles_state.bin";
  