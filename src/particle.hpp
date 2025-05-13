#pragma once

#include <SFML/Graphics.hpp>

struct particle
{
    sf::Vector2f position;
    sf::Vector2f velocity;      
    sf::Vector2f acceleration;
    float radius;
    sf::Color color;

    particle(sf::Vector2f position_, sf::Vector2f velocity_, sf::Vector2f acceleration_, float radius_, sf::Color color_)
        : position{position_}, velocity{velocity_}, acceleration{acceleration_}, radius{radius_}, color{color_}
    {}
};