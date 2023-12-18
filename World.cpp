#include "World.h"

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

void World::update(float dt) {
    counter += dt;
    t += 0.001f;
    if(counter > interval) {
        counter = 0.0f;
        balls.push_back(std::make_unique<Ball>(shooterPos, 10.0f, getRainbow(t)));
    } 
    float sub_dt = dt / (float)sub_steps;
    for (int i = 0; i < sub_steps; ++i)
    {
        applyGravity();
        applyConstraint();
        resolveCollision();
        updatePosition(sub_dt);
    }
}

void World::applyGravity() {
    for(auto &b : balls)
    {
        b->accelerate(gravity);
    }
}

void World::updatePosition(float dt) 
{
    for(auto &b : balls) 
    {
        b->update(dt);
    }
}

void World::applyConstraint() 
{
    for(auto &b : balls) 
    {
        b->wallCollide(800, 800);
    }
}
void World::resolveCollision() {
    for(int i=0;i<balls.size();i++) {
        for (int j = 0; j < balls.size(); j++)
        {
            if (i != j && balls[i].get()->isColliding(balls[j].get()))
            {
                balls[i].get()->resolveCollision(balls[j].get());
            }
        }
    }
}

void World::draw(sf::RenderWindow &window) 
{
    for (auto &b : balls)
    {
        b->draw(window);
    }
}

