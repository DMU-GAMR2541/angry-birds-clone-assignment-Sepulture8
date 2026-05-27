#pragma once

#include <SFML/Graphics.hpp>

class GameObject {
public:
    
     GameObject() = default;

     static inline int gameObjectDestroyedCount = 0;

     virtual ~GameObject()
     {
         gameObjectDestroyedCount++;
     }

    virtual void draw(sf::RenderWindow& window) = 0;
};