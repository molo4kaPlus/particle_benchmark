#pragma once

struct particle
{
    sf::Vector2f position;
    sf::Vector2f velocity;      
    sf::Vector2f acceleration;
    float radius = 10.0f;

    particle(sf::Vector2f position_, sf::Vector2f velocity_, sf::Vector2f acceleration_, float radius_)
        : position{position_}, velocity{velocity_}, acceleration{acceleration_}, radius{radius_}
    {}
};