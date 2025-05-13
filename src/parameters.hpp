#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "particle.hpp"

#include <string>
#include <vector>
#include <cmath>

const int g_windowWidth = 800;
const int g_windowHeight = 600;
const int g_maximumParticles = 100;

const float g_damp = 0.75f;
const sf::Vector2f GRAVITY(0.f, 3.0f);