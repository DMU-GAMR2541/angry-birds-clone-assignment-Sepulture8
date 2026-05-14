#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "DynamicObject.h"

class Pig : public DynamicObject {
private:
    float xPos;
    float yPos;
    float radius;
    int health = 100;
    bool markedForDeletion = false;
    const float SCALE = 30.0f;
    const float PI = 3.1415927f;

public:




    Pig() = default;

    Pig(
        b2World& world,
        float xPos,
        float yPos,
        float radius,
        float density,
        std::string texturePath,
        DynamicObjectType pigtype
    )
        : DynamicObject(world, b2Vec2(xPos, yPos), texturePath)  
    {
        this->xPos = xPos;
        this->yPos = yPos;
        this->radius = radius;
        b2_circleShape.m_radius = radius / SCALE;

        switch (pigtype)
        {
        case DynamicObjectType::pig:

            health = 30;

            b2_ballFixture.density = 1.0f;
            b2_ballFixture.restitution = 0.5f;
            sp_sprites.setTextureRect(sf::IntRect(53, 70, 47, 44));
            sp_sprites.setOrigin(47 / 2.0f, 44 / 2.0f);
            break;
        case DynamicObjectType::helmpig:

            health = 60;

            b2_ballFixture.density = 1.1f;
            b2_ballFixture.restitution = 0.45f;
            sp_sprites.setTextureRect(sf::IntRect(53, 210, 93, 84));
            sp_sprites.setOrigin(93 / 2.0f, 84 / 2.0f);
            sp_sprites.setScale(0.5f, 0.5f);
            break;
        case DynamicObjectType::bigpig:

            health = 90;

            b2_ballFixture.density = 1.3f;
            b2_ballFixture.restitution = 0.4f;
            sp_sprites.setTextureRect(sf::IntRect(64, 415, 110, 98));
            sp_sprites.setOrigin(110 / 2.0f, 98 / 2.0f);
            sp_sprites.setScale(0.4f, 0.4f);
            break;
        case DynamicObjectType::kingpig:

            health = 120;

            b2_ballFixture.density = 1.5f;
            b2_ballFixture.restitution = 0.3f;
            sp_sprites.setTextureRect(sf::IntRect(47, 526, 126, 153));
            sp_sprites.setOrigin(126 / 2.0f, 153 / 2.0f);
            sp_sprites.setScale(0.35f, 0.35f);
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

    void takeDamage(int damage)
    {
        health -= damage;
    }

    int getHealth()
    {
        return health;
    }

    bool checkIfPopped()
    {
        return health <= 0;
    }

    void markForDeletion()
    {
        markedForDeletion = true;
    }

    void resetDeletionMark()
    {
        markedForDeletion = false;
    }

    bool isMarkedForDeletion()
    {
        return markedForDeletion;
    }
};