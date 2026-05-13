#pragma once
#include "DynamicObject.h"

class Catapult : public DynamicObject {
private:
	float xPos = 150.0f;
	float yPos = 500.0f;
	float HalfX = 7.0f;
	float HalfY = 60.0f; 
		sf::Vector2f shotPos;

public:
	Catapult() = default;

	// passing Catapult parameters into DynamicObject.h / DynamicObject.cpp for the creation of the Catapult.
	Catapult(b2World& world, float xPos, float yPos, float HalfX, float HalfY, std::string Texture) : DynamicObject(world, b2Vec2(xPos, yPos), Texture) {

		// gives default values to parameters
		this->xPos = xPos;
		this->yPos = yPos;
		this->HalfX = HalfX;
		this->HalfY = HalfX;
		this->shotPos = sf::Vector2f(xPos - 5.0f, yPos - 50.0f);

		//No fixture to stop birds from colliding with it.

		// Catapult wont move so bird does not collide with it.
		b2_bodyDef.type = b2_staticBody;


		// sets its position in meters for box2d
		b2_bodyDef.position.Set(xPos / SCALE, yPos / SCALE);

		// creates it into the world.
		b2_body = world.CreateBody(&b2_bodyDef);

		// sets its width and heigh in meters for Box2d
		b2_polygonShape.SetAsBox(HalfX / SCALE, HalfY / SCALE);


	}
	// getter for birds shooting position.
	sf::Vector2f getShotPos() { return shotPos; }

	// draw function from virtual class gameObject
	void draw(sf::RenderWindow& window) {
		window.draw(sp_sprites);
	}

	// gives start position of the catapult.
	void update() {
		sp_sprites.setPosition(b2_body->GetPosition().x * SCALE, b2_body->GetPosition().y * SCALE);
		sp_sprites.setRotation(b2_body->GetAngle() * (180.0f / PI));
	}

};