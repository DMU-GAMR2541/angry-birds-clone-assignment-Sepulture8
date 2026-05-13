#pragma once

#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>

class StaticObject
{
protected:

    static constexpr float SCALE = 30.0f;

    b2BodyDef bodyDef;
    b2Body* b2_body = nullptr;

    b2PolygonShape box;

    sf::RectangleShape Rectangle;

public:

    StaticObject() = default;

    StaticObject(
        b2World& world,
        b2Vec2 position,
        b2Vec2 size
    );

    virtual ~StaticObject();

    virtual void draw(sf::RenderWindow& window) = 0;
};