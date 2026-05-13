#pragma once

#include <SFML/Graphics.hpp>

class GameObject {
public:
    
     GameObject() = default;
     ~GameObject() = default;

    virtual void draw(sf::RenderWindow& window) = 0;
};