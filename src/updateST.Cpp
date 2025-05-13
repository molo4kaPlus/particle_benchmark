#pragma once

#include "particle.hpp"
#include "parameters.hpp"
#include <vector>

using namespace std;

inline void checkBorders(vector<particle> &objects)
{
    for(auto& obj : objects)
    {
        obj.acceleration += GRAVITY;
        obj.velocity += obj.acceleration;
        obj.position += obj.velocity;
        if (obj.position.y + obj.radius > g_windowHeight) // down
        {
            obj.position.y = g_windowHeight - obj.radius * 2;
            obj.velocity.y *= -g_damp;
        }
        if (obj.position.y - obj.radius < 0) // up
        {
            obj.position.y = 0;
            obj.velocity.y *= -g_damp;
        }
        if (obj.position.x + obj.radius > g_windowWidth) // right
        {
            obj.position.x = g_windowWidth - obj.radius * 2;
            obj.velocity.x *= -g_damp;
        }
        if (obj.position.x + obj.radius < 0) // left
        {
            obj.position.x = 0;
            obj.velocity.x *= -g_damp;
        }
    }
}

inline void checkCollisions(vector<particle> &objects)
{
    for(auto& obj : objects)
    {

    }
}