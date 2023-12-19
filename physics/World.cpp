#include "World.hpp"
#include "KinematicBody.hpp"
#include "StaticBody.hpp"

#include <iostream>

sf::Color getRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * 3.142f);
    const float b = sin(t + 0.66f * 2.0f * 3.142f);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

World::World() {

}

void World::init() {
    frame_dt = 1.0f / 60.0f;

    //static body init
    float tr = 20.0f;
    for(int i=0;i<5;i++) {
        bodies.push_back(new StaticBody(sf::Vector2f(200 + i * tr * 2.0f, 300 - i * tr), tr));
    }
    for(int i=0;i<5;i++) {
        bodies.push_back(new StaticBody(sf::Vector2f(250 - i * tr * 2.0f, 600 - i * tr), tr));
    }
    for(int i=0;i<5;i++) {
        bodies.push_back(new StaticBody(sf::Vector2f(500 + i * tr * 2.0f, 500), tr));
    }
}

void World::update() {
    float sub_dt = frame_dt / (float)sub_steps;

    counter += frame_dt;
    t += 0.001f;
    if(counter > interval && objCounter < maxObject) {
        counter = 0.0f;
        objCounter++;
        shooterPos.x = rand() % 400 + 100;
        int tr = 5;
        KinematicBody* tBody = new KinematicBody(shooterPos, rand() % tr + tr, getRainbow(t));
        tBody->setVelocity(sf::Vector2f(rand() % 2000 - 1000,0), sub_dt);
        bodies.push_back(tBody);
    }
    for (int i = 0; i < sub_steps; i++)
    {
        applyGravity();
        applyConstraint();
        resolveCollision();
    }
    updatePosition(sub_dt);
}

void World::applyGravity() {
    for(PhysicsBody *b : bodies)
    {
        b->accelerate(gravity);
    }
}

void World::updatePosition(float dt) 
{
    for(PhysicsBody *b : bodies) 
    {
        b->update(dt);
    }
}

void World::applyConstraint() 
{
    for(PhysicsBody *b : bodies) 
    {
        b->wallCollide(800, 800);
    }
}
void World::resolveCollision() {
    for(int i=0;i<bodies.size();i++) {
        for (int j = 0; j < bodies.size(); j++)
        {
            if (i != j && bodies[i]->isColliding(bodies[j]))
            {
                if(!bodies[i]->isKinematic())
                    bodies[i]->resolveCollision(bodies[j]);
                else 
                    bodies[j]->resolveCollision(bodies[i]);
            }
        }
    }
}

void World::draw(sf::RenderWindow &window) 
{
    for (PhysicsBody *b : bodies)
    {
        b->draw(window);
    }
}

int World::getBodyCount() {
    return objCounter;
}
