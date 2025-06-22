#include "particle.hpp"
#include "parameters.hpp"
#include "grid.hpp"
#include <vector>

#include <omp.h>

using namespace std;

static sf::Color getColor(float t) {
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * M_PI);
    const float b = sin(t + 0.66f * 2.0f * M_PI);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

inline void applyGravity(vector<particle> &objects, sf::Vector2f GRAVITY)
{
    #pragma omp parallel for schedule(dynamic) 
    for(auto& obj : objects)
    {
        obj.acceleration += GRAVITY;
    }
}

inline void createParticle(vector<particle> &objects, int &frameCount)
{
    objects.push_back(particle(
            sf::Vector2f{100.f, 200.f},    
            sf::Vector2f{0.1f, 0.1f}, 
            sf::Vector2f{0.f, 0.f},
            g_particleRadius,                  
            getColor(frameCount/100)
        ));
}

inline void checkBorders(vector<particle> &objects)
{
    for(auto& obj : objects)
    {
        obj.velocity += obj.acceleration;
        obj.position += obj.velocity;
        
        obj.acceleration = {0.f, 0.f};
        
        if (obj.position.y + obj.radius > g_windowHeight) // вниз
        {
            obj.position.y = g_windowHeight - obj.radius;
            obj.velocity.y *= -g_damp;
            if(abs(obj.velocity.y) < 0.2f) obj.velocity.y = -0.2f;
        }
        if (obj.position.y - obj.radius < 0) // вверх
        {
            obj.position.y = obj.radius;
            obj.velocity.y *= -g_damp;
        }
        if (obj.position.x + obj.radius > g_windowWidth) // вправо
        {
            obj.position.x = g_windowWidth - obj.radius;
            obj.velocity.x *= -g_damp;
        }
        if (obj.position.x - obj.radius < 0) // влево
        {
            obj.position.x = obj.radius;
            obj.velocity.x *= -g_damp;
        }
    }
}

inline void checkCollisions(vector<particle> &objects, SpatialGrid& grid)
{
    grid.clear();
    for (auto& obj : objects) {
        grid.insert(obj);
    }

    //#pragma omp parallel for schedule(dynamic) 
    for(size_t i = 0; i < objects.size(); ++i)
    {
        particle &a = objects[i];
        auto nearbyParticles = grid.getNearbyParticles(a);
        
        for(particle* bPtr : nearbyParticles)
        {
            if (bPtr <= &a) continue;
            
            particle &b = *bPtr;
            sf::Vector2f delta = b.position - a.position;
            float distanceSquared = delta.x * delta.x + delta.y * delta.y;
            float minDistance = a.radius + b.radius;
            float minDistanceSquared = minDistance * minDistance;
            
            if(distanceSquared < minDistanceSquared && distanceSquared > 0.1f)
            {
                float distance = fastSqrt(distanceSquared);

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

inline void applyAttraction(vector<particle> &objects, const sf::Vector2f& point, bool isActive, float force, float radius)
{
    if (!isActive) return;
    
    //#pragma omp parallel for schedule(dynamic) 
    for (auto& obj : objects)
    {
        sf::Vector2f delta = point - obj.position;
        float distanceSquared = delta.x * delta.x + delta.y * delta.y;
        
        if (distanceSquared < radius * radius && distanceSquared > 10.f) 
        {
            float distance = fastSqrt(distanceSquared); 
            sf::Vector2f direction = delta / distance;
            float strength = force * (1.f - distance / radius); 
            obj.velocity += direction * strength;
        }
    }
}