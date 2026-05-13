#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <map>
#include <vector>
#include <list>
#include <set>
#include <iostream>

#include "DynamicObject.h"
#include "StaticObject.h"
#include "Bird.h"
#include "Plank.h"
#include "Pig.h"
#include "NonInteractable.h"
#include "Catapault.h"
#include "ContactListener.h"

int main() {

    // --- 1. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(800, 600), "Annoyed_Flocks");
    window.setFramerateLimit(60);

    const sf::Vector2f CATAPULT_POS(150.0f, 520.0f);
    const sf::Vector2f PLANK_POS(400.0f, 550.0f);
    bool abilityActivated = false;
    bool birdLaunched = false;

    sf::Clock birdTimer;

    const float SCALE = 30.0f;
    const float PI = 3.1415927;

    b2Vec2 b2_gravity(0.0f, 9.8f);
    b2World world(b2_gravity);
    ContactListener contactlister;
    world.SetContactListener(&contactlister);

    Catapult catapult(world, 150.0f, 520.0f, 10.0f, 60.0f, "../assets/Ang_Birds/Slingshot.png");
    Plank plank(world, 500.0f, 550.0f, 10.0f, 60.0f, "../assets/Ang_Birds/plank.jpg");

    std::vector<std::string> birdTextures = {
        "../assets/Ang_Birds/BlueBird.png",
        "../assets/Ang_Birds/MainBird.png",
        "../assets/Ang_Birds/YellowBird.png",
        "../assets/Ang_Birds/BlackBird.png"
    };

    std::vector<std::shared_ptr<Pig>> pigPtr;
    std::vector<std::shared_ptr<NonInteractable>> Noninteractable;
    std::list<std::shared_ptr<Bird>> birdPtr;

    DynamicObject::DynamicObjectType birdtype;

    Noninteractable.push_back(std::make_shared<NonInteractable>(world, 750.0f, 500.0f, 10.0f, 80.0f, sf::Color::Red));
    Noninteractable.push_back(std::make_shared<NonInteractable>(world, 400.0f, 590.0f, 400.0f, 10.0f, sf::Color(34, 139, 34)));

    for (int i = 0; i < 4; i++) {
        if (i == 0) { birdtype = DynamicObject::DynamicObjectType::bluebird; }
        else if (i == 2) { birdtype = DynamicObject::DynamicObjectType::yellowbird; }
        else if (i == 3) { birdtype = DynamicObject::DynamicObjectType::blackbird; }
        else { birdtype = DynamicObject::DynamicObjectType::redbird; }

        birdPtr.push_back(std::make_shared<Bird>(world, 100.0f - (i * -20.0f), 500.0f, 15.0f, 5.0f, birdTextures[i], birdtype));
    }

    for (int i = 0; i < 4; i++) {
        DynamicObject::DynamicObjectType pigtype;

        if (i == 0) { pigtype = DynamicObject::DynamicObjectType::pig; }
        else if (i == 1) { pigtype = DynamicObject::DynamicObjectType::helmpig; }
        else if (i == 2) { pigtype = DynamicObject::DynamicObjectType::bigpig; }
        else { pigtype = DynamicObject::DynamicObjectType::kingpig; }

        auto& pig = pigPtr.emplace_back(std::make_shared<Pig>(world, (500.0f + (i * 45.0f)), 400.0f, (15.0f + (i * 3)), (10 + (i * 2)), "../assets/Ang_Birds/Pigs.png", pigtype));
        pig->getBody()->GetUserData().pointer = 3 + i;
    }

    while (window.isOpen()) {

        // --- EVENT HANDLING ---
        sf::Event event;
        while (window.pollEvent(event)) {

            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!birdPtr.empty())
                        birdPtr.front()->dragging();
                }
            }

            if (event.type == sf::Event::MouseButtonReleased) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    if (!birdPtr.empty()) {
                        birdPtr.front()->launch(catapult.getShotPos());
                        abilityActivated = false;
                        birdLaunched = true;
                        birdTimer.restart();
                    }
                }
            }

            if (event.type == sf::Event::KeyPressed) {

                if (event.key.code == sf::Keyboard::L) {
                    if (!birdPtr.empty()) {
                        world.DestroyBody(birdPtr.front()->getBody());
                        birdPtr.pop_front();

                        for (auto& pig : pigPtr)
                            pig->resetDeletionMark();

                        contactlister.s_ptr.clear();
                    }
                }

                if (event.key.code == sf::Keyboard::Space) {
                    if (!birdPtr.empty() && !birdPtr.front()->getDragging()) {
                        abilityActivated = true;
                    }
                }
            }
        }

        // --- UPDATE MOUSE DRAG ---
        if (!birdPtr.empty() && birdPtr.front()->getDragging()) {

            sf::Vector2i mousePxl = sf::Mouse::getPosition(window);
            sf::Vector2f mouseWorld(mousePxl.x, mousePxl.y);
            sf::Vector2f dragVec(mouseWorld - catapult.getShotPos());

            if (dragVec.x > 0) dragVec.x = 0;
            if (dragVec.y < 0) dragVec.y = 0;

            float length = std::sqrt(dragVec.x * dragVec.x + dragVec.y * dragVec.y);
            float maxDrag = 100.0f;

            if (length > maxDrag) {
                dragVec /= length;
                dragVec *= maxDrag;
            }

            sf::Vector2f finalPos = catapult.getShotPos() + dragVec;
            birdPtr.front()->getBody()->SetTransform(b2Vec2(finalPos.x / SCALE, finalPos.y / SCALE), 0);
        }

        // --- ABILITY ACTIVATION ---
        if (!birdPtr.empty() && abilityActivated)
        {
            auto& bird = birdPtr.front();
            auto  type = bird->getBirdType();
            b2Body* body = bird->getBody();

            if (!bird->isAbilityUsed())
            {
                if (type == DynamicObject::DynamicObjectType::yellowbird)
                {
                    b2Vec2 vel = body->GetLinearVelocity();
                    vel.x *= 2.5f;
                    vel.y *= 2.5f;
                    body->SetLinearVelocity(vel);
                    bird->setAbilityUsed(true);
                }

                if (type == DynamicObject::DynamicObjectType::bluebird)
                {
                    b2Vec2 pos = body->GetPosition();
                    b2Vec2 vel = body->GetLinearVelocity();

                    float angles[] = { -0.3f, 0.0f, 0.3f };
                    float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);

                    for (float angleOffset : angles)
                    {
                        auto newBird = std::make_shared<Bird>(
                            world,
                            pos.x * SCALE,
                            pos.y * SCALE,
                            15.0f,
                            5.0f,
                            "../assets/Ang_Birds/BlueBird.png",
                            DynamicObject::DynamicObjectType::bluebird
                        );

                        b2Vec2 newVel(
                            vel.x + std::cos(angleOffset) * 0.5f,
                            vel.y + std::sin(angleOffset) * speed * 0.3f
                        );
                        newBird->getBody()->SetLinearVelocity(newVel);
                        newBird->setAbilityUsed(true);
                        birdPtr.push_back(newBird);
                    }

                    world.DestroyBody(body);
                    birdPtr.pop_front();
                }

                if (type == DynamicObject::DynamicObjectType::blackbird)
                {
                    b2Vec2 bombPos = body->GetPosition();
                    float  blastRadius = 5.0f;
                    float  blastPower = 30.0f;

                    for (int i = 0; i < pigPtr.size(); i++)
                    {
                        b2Vec2 pigPos = pigPtr[i]->getBody()->GetPosition();
                        b2Vec2 diff = pigPos - bombPos;
                        float  dist = diff.Length();

                        if (dist < blastRadius && dist > 0.0f)
                        {
                            float falloff = 1.0f - (dist / blastRadius);
                            diff.Normalize();

                            b2Vec2 impulse = blastPower * falloff * diff;
                            pigPtr[i]->getBody()->ApplyLinearImpulseToCenter(impulse, true);
                            pigPtr[i]->takeDamage(static_cast<int>(50 * falloff));

                            if (pigPtr[i]->checkIfPopped())
                            {
                                world.DestroyBody(pigPtr[i]->getBody());
                                pigPtr.erase(pigPtr.begin() + i);
                                i--;
                            }
                        }
                    }

                    bird->setAbilityUsed(true);
                }
            }

            abilityActivated = false;
        }

        // --- BIRD TIMER (auto remove after 3s) ---
        if (birdLaunched) {
            if (birdTimer.getElapsedTime().asSeconds() >= 3.0f) {
                if (!birdPtr.empty()) {
                    world.DestroyBody(birdPtr.front()->getBody());
                    birdPtr.pop_front();

                    for (auto& pig : pigPtr)
                        pig->resetDeletionMark();

                    contactlister.s_ptr.clear();
                }
                birdLaunched = false;
            }
        }

        // --- DAMAGE LOOP ---
        std::set<uintptr_t> s_p = contactlister.getPointer();
        for (int i = 0; i < pigPtr.size(); i++)
        {
            uintptr_t currentPigID = pigPtr[i]->getBody()->GetUserData().pointer;

            if (s_p.find(currentPigID) != s_p.end())
            {
                pigPtr[i]->takeDamage(10);
                std::cout << "Pig has: " << pigPtr[i]->getHealth() << " Health" << std::endl;

                if (pigPtr[i]->checkIfPopped())
                {
                    world.DestroyBody(pigPtr[i]->getBody());
                    pigPtr.erase(pigPtr.begin() + i);
                    i--;
                }
            }
        }

        // --- PHYSICS STEP ---
        world.Step(1.0f / 60.0f, 8, 3);

        for (auto& bird : birdPtr)
            bird->getBody()->GetUserData().pointer = 100;

        // --- UPDATE ---
        catapult.update();
        plank.Update();

        for (auto& pig : pigPtr)   pig->update();
        for (auto& bird : birdPtr) bird->update();
        for (auto& ni : Noninteractable) ni->start();

        // --- DRAW ---
        window.clear(sf::Color(135, 206, 235));

        for (auto& pig : pigPtr)   pig->draw(window);
        for (auto& bird : birdPtr) bird->draw(window);
        for (auto& ni : Noninteractable) ni->draw(window);

        catapult.draw(window);
        plank.draw(window);
        window.display();
    }

    return 0;
}