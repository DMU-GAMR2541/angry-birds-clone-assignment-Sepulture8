#pragma once

#include "StaticObject.h"

class NonInteractable : public StaticObject
{
private:

public:

    NonInteractable() = default;

    NonInteractable(
        b2World& world,
        float xPos,
        float yPos,
        float Width,
        float Height,
        sf::Color color
    )
        :
        StaticObject(
            world,
            b2Vec2(xPos, yPos),
            b2Vec2(Width, Height)
        )
    {
        Rectangle.setSize(
            sf::Vector2f(Width * 2, Height * 2)
        );

        Rectangle.setOrigin(
            Width,
            Height
        );

        Rectangle.setFillColor(color);
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(Rectangle);
    }

    void start()
    {
        Rectangle.setPosition(
            b2_body->GetPosition().x * SCALE,
            b2_body->GetPosition().y * SCALE
        );
    }
};