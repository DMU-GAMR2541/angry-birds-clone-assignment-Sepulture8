#include <gtest/gtest.h>
#include "Enemy.h"
#include "Slingshot.h"
#include "Bird.h"

#include <box2d/box2d.h>
#include <memory>
#include <vector>
#include "DynamicObject.h"
#include "StaticObject.h"

#include "Plank.h"
#include "Pig.h"
#include "NonInteractable.h"
#include "Catapault.h"
#include "ContactListener.h"

// ENEMY FIXTURE

class EnemyTest : public testing::Test
{
protected:
    std::unique_ptr<Enemy> enemy;

    void SetUp() override
    {
        enemy = std::make_unique<Enemy>(50);
    }
};


// SLINGSHOT FIXTURE

class SlingshotTest : public testing::Test
{
protected:
    std::unique_ptr<Slingshot> slingshot;

    void SetUp() override
    {
        slingshot = std::make_unique<Slingshot>(100, "Red");
    }
};


// BIRD FIXTURE

class BirdTest : public testing::Test
{
protected:
    std::unique_ptr<b2World> world;
    std::unique_ptr<Bird> bird;

    void SetUp() override
    {
        world = std::make_unique<b2World>(b2Vec2(0.0f, 9.8f));

        bird = std::make_unique<Bird>(
            *world,
            100.f, 500.f,
            15.f,
            5.f,
            "../assets/Ang_Birds/RedBird.png",
            DynamicObject::DynamicObjectType::redbird
        );
    }
};

//STATIC OBJECT TESTS

TEST(StaticObjectPlacement, CorrectPlacementInWindow)
{
    b2World world(b2Vec2(0.0f, 9.8f));

    float x = 300.f;
    float y = 200.f;

    NonInteractable obj(world,
        x, y,
        50.f, 20.f,
        sf::Color::Red
    );

    b2Vec2 pos = obj.getBody()->GetPosition();

    EXPECT_NEAR(pos.x, x / 30.0f, 0.01f);
    EXPECT_NEAR(pos.y, y / 30.0f, 0.01f);
}

// ENEMY TESTS

TEST(Enemy, HealthSetCorrectly)
{
    Enemy e(100);
    EXPECT_EQ(e.getHealth(), 100);
}

TEST(Enemy, ZeroHealthEnemy)
{
    Enemy e(0);
    EXPECT_EQ(e.getHealth(), 0);
}


// SLINGSHOT TESTS

TEST(Slingshot, TensionWithinLimit)
{
    Slingshot s(99, "Red");
    EXPECT_LT(s.getTension(), 100);
}

TEST_F(SlingshotTest, BirdColourStoredCorrectly)
{
    slingshot->loadBird("Red");
    EXPECT_EQ(slingshot->getBirdType(), "Red");
}


// ENEMY DAMAGE TEST

TEST_F(EnemyTest, EnemyDiesAfterDamage)
{
    enemy->takeDamage(60);
    EXPECT_TRUE(enemy->checkIfPopped());
}


// BIRD MOVEMENT TEST 

TEST_F(BirdTest, BirdMovesAfterLaunch)
{
    bird->launch(sf::Vector2f(50.f, 500.f));

    for (int i = 0; i < 10; i++)
        world->Step(1.0f / 60.0f, 8, 3);

    EXPECT_NE(bird->getBody()->GetLinearVelocity().Length(), 0.0f);
}


// BIRD MOVEMENT TEST (SPREAD OF VALUES)

TEST_F(BirdTest, BirdMovesAcrossDifferentLaunchPositions)
{
    std::vector<sf::Vector2f> shots =
    {
        {50.f, 500.f},
        {80.f, 520.f},
        {120.f, 480.f},
        {160.f, 600.f},
        {200.f, 450.f}
    };

    for (auto pos : shots)
    {
        bird = std::make_unique<Bird>(
            *world,
            100.f, 500.f,
            15.f,
            5.f,
            "../assets/Ang_Birds/RedBird.png",
            DynamicObject::DynamicObjectType::redbird
        );

        bird->launch(pos);

        for (int i = 0; i < 10; i++)
            world->Step(1.0f / 60.0f, 8, 3);

        EXPECT_NE(bird->getBody()->GetLinearVelocity().Length(), 0.0f);
    }
}
TEST(RelativePositioning, PigsAreCorrectlySpaced)
{
    b2World world(b2Vec2(0.0f, 9.8f));

    std::vector<std::shared_ptr<Pig>> pigs;

    for (int i = 0; i < 4; i++)
    {
        pigs.push_back(std::make_shared<Pig>(
            world,
            300.0f + (i * 50.0f),
            400.0f,
            15.0f,
            10.0f,
            "",   
            DynamicObject::DynamicObjectType::pig
        ));
    }

  
    world.Step(1.0f / 60.0f, 8, 3);

    EXPECT_LT(pigs[0]->getBody()->GetPosition().x,
        pigs[1]->getBody()->GetPosition().x);

    EXPECT_LT(pigs[1]->getBody()->GetPosition().x,
        pigs[2]->getBody()->GetPosition().x);

    EXPECT_LT(pigs[2]->getBody()->GetPosition().x,
        pigs[3]->getBody()->GetPosition().x);
}


TEST(TextureLoading, ValidTextureLoadsSuccessfully)
{
    sf::Texture texture;

    bool loaded = texture.loadFromFile("../assets/Ang_Birds/RedBird.png");

    EXPECT_TRUE(loaded);
    EXPECT_GT(texture.getSize().x, 0);
    EXPECT_GT(texture.getSize().y, 0);
}


// MAIN

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}