#pragma once

#include "particle.hpp"
#include "parameters.hpp"
#include <vector>

using namespace std;

inline void applyGravity(vector<particle> &objects)
{
    for(auto& obj : objects)
    {
        obj.acceleration += GRAVITY;
    }
}

inline void checkBorders(vector<particle> &objects)
{
    for(auto& obj : objects)
    {
        obj.velocity += obj.acceleration;
        obj.position += obj.velocity;
        
        obj.acceleration = {0.f, 0.f};
        
        if (obj.position.y + obj.radius > g_windowHeight) // down
        {
            obj.position.y = g_windowHeight - obj.radius;
            obj.velocity.y *= -g_damp;
            if(abs(obj.velocity.y) < 0.2f) obj.velocity.y = -0.2f;
        }
        if (obj.position.y - obj.radius < 0) // up
        {
            obj.position.y = obj.radius;
            obj.velocity.y *= -g_damp;
        }
        if (obj.position.x + obj.radius > g_windowWidth) // right
        {
            obj.position.x = g_windowWidth - obj.radius;
            obj.velocity.x *= -g_damp;
        }
        if (obj.position.x - obj.radius < 0) // left
        {
            obj.position.x = obj.radius;
            obj.velocity.x *= -g_damp;
        }
    }
}

inline void checkCollisions(vector<particle> &objects)
{
    for(size_t i = 0; i < objects.size(); ++i)
    {
        for(size_t j = i + 1; j < objects.size(); ++j)
        {
            particle &a = objects[i];
            particle &b = objects[j];
            
            sf::Vector2f delta = b.position - a.position;
            float distanceSquared = delta.x * delta.x + delta.y * delta.y;
            float minDistance = a.radius + b.radius;
            float minDistanceSquared = minDistance * minDistance;
            
            
            if(distanceSquared < minDistanceSquared && distanceSquared > 0.001f)
            {
                float distance = sqrt(distanceSquared);

                sf::Vector2f normal = delta / distance;
                
                sf::Vector2f relativeVelocity = b.velocity - a.velocity;
                
                float impulse = (g_particleCollisionStrength) * (relativeVelocity.x * normal.x + relativeVelocity.y * normal.y);
                impulse /= (1.0f / a.radius + 1.0f / b.radius);
                
                a.velocity += (impulse / a.radius) * normal;
                b.velocity -= (impulse / b.radius) * normal;
                
                float overlap = minDistance - distance;
                sf::Vector2f separation = normal * overlap * g_particleCollisionStrength;
                a.position -= separation * (b.radius / (a.radius + b.radius));
                b.position += separation * (a.radius / (a.radius + b.radius));
            }
        }
    }
}