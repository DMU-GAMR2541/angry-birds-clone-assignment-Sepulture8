#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "DynamicObject.h"

class Bird : public DynamicObject {
private:
    float xPos = 100.0f;
    float yPos = 500.0f;
    float radius = 15.0f;
    float shotPower = 5.0f;
    bool isDragging = false;
    bool abilityUsed = false;
    sf::Vector2f startPos;
    DynamicObjectType birdType;
    const float SCALE = 30.0f;
    const float PI = 3.1415927f;

public:
    Bird() = default;

    bool getDragging() const
    {
        return isDragging;
    }

    bool isAbilityUsed() const
    {
        return abilityUsed;
    }

    void setAbilityUsed(bool val)
    {
        abilityUsed = val;
    }

    Bird(
        b2World& world,
        float xPos,
        float yPos,
        float radius,
        float shotPower,
        std::string birdTexture,
        DynamicObjectType birdtype
    )
        : DynamicObject(world, b2Vec2(xPos, yPos), birdTexture)
    {
        this->xPos = xPos;
        this->yPos = yPos;
        this->radius = radius;
        this->shotPower = shotPower;
        this->birdType = birdtype;
        startPos = sf::Vector2f(xPos, yPos);
        b2_circleShape.m_radius = radius / SCALE;

        switch (birdtype)
        {
        case DynamicObjectType::redbird:
            b2_ballFixture.density = 1.0f;
            b2_ballFixture.restitution = 0.5f;
            sp_sprites.setScale(0.65f, 0.65f);
            break;
        case DynamicObjectType::blackbird:
            b2_ballFixture.density = 1.4f;
            b2_ballFixture.restitution = 0.3f;
            sp_sprites.setScale(0.040f, 0.040f);
            break;
        case DynamicObjectType::yellowbird:
            b2_ballFixture.density = 0.8f;
            b2_ballFixture.restitution = 0.6f;
            sp_sprites.setScale(0.040f, 0.040f);
            break;
        case DynamicObjectType::bluebird:
            b2_ballFixture.density = 0.7f;
            b2_ballFixture.restitution = 0.7f;
            sp_sprites.setScale(0.040f, 0.040f);
            break;
        }

        b2_ballFixture.shape = &b2_circleShape;
        b2_body->CreateFixture(&b2_ballFixture);
    }

    void draw(sf::RenderWindow& window) override
    {
        window.draw(sp_sprites);
    }

    void update()
    {
        sp_sprites.setPosition(
            b2_body->GetPosition().x * SCALE,
            b2_body->GetPosition().y * SCALE
        );
        sp_sprites.setRotation(
            b2_body->GetAngle() * (180.0f / PI)
        );
    }

    void dragging()
    {
        isDragging = true;
        b2_body->SetGravityScale(0);
        b2_body->SetLinearVelocity(b2Vec2(0, 0));
        b2_body->SetAngularVelocity(0);
    }

    void launch(sf::Vector2f shotPos)
    {
        isDragging = false;
        abilityUsed = false;  // reset on each launch
        b2_body->SetGravityScale(1.0f);
        sf::Vector2f targetPos(
            b2_body->GetPosition().x * SCALE,
            b2_body->GetPosition().y * SCALE
        );
        sf::Vector2f launchVector = (targetPos - shotPos) / SCALE;
        b2Vec2 impulse(
            -launchVector.x * shotPower,
            -launchVector.y * shotPower
        );
        b2_body->ApplyLinearImpulseToCenter(impulse, true);
        std::cout << "Bird launched!\n";
    }

    void yellowBirdAbility(b2Vec2 impulse)
    {
        b2_body->ApplyLinearImpulseToCenter(impulse, true);
    }

    DynamicObjectType getBirdType()
    {
        return birdType;
    }
};