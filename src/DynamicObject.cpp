#include "DynamicObject.h"
#include <iostream>
#include "Bird.h"

DynamicObject::~DynamicObject() {

}

DynamicObject::DynamicObject(b2World& b2_world, b2Vec2 b2_posIn, std::string str_SpriteLocation) {

	b2_bodyDef.position.Set(b2_posIn.x / SCALE, b2_posIn.y / SCALE);

	b2_bodyDef.type = b2_dynamicBody;

	b2_body = b2_world.CreateBody(&b2_bodyDef);

	b2_ballFixture.shape = &b2_circleShape;


		if (!te_Texture.loadFromFile(str_SpriteLocation))
			return;

	sp_sprites.setTexture(te_Texture);
	sp_sprites.setOrigin(te_Texture.getSize().x / 2.0f, te_Texture.getSize().y / 2.0f);
	sp_sprites.setPosition(b2_bodyDef.position.x * SCALE, b2_bodyDef.position.y * SCALE);


}


void DynamicObject::Update(float gravityScale, b2Vec2 b2_impulse, bool b_make)
{
	// Sync sprite position to Box2D body every frame
	sp_sprites.setPosition(
		b2_body->GetPosition().x * SCALE,
		b2_body->GetPosition().y * SCALE
	);
	// Sync rotation too
	sp_sprites.setRotation(b2_body->GetAngle() * (180.0f / PI));
}

void DynamicObject::Render(sf::RenderWindow& sf_window)
{
}

