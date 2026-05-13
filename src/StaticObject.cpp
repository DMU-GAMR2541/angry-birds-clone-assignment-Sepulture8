#include "StaticObject.h"

StaticObject::~StaticObject()
{
}

StaticObject::StaticObject(
    b2World& world,
    b2Vec2 position,
    b2Vec2 size
)
{
    bodyDef.position.Set(
        position.x / SCALE,
        position.y / SCALE
    );

    b2_body = world.CreateBody(&bodyDef);

    box.SetAsBox(
        size.x / SCALE,
        size.y / SCALE
    );

    b2_body->CreateFixture(&box, 0.0f);
}