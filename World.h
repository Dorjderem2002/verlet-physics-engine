#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "Ball.h"

class World {
public:
    World();
    void applyGravity();
    void updatePosition(float dt);
    void applyConstraint();
    void resolveCollision();
    void update(float dt);
    void draw(sf::RenderWindow &window);
private:
    sf::Vector2f shooterPos = sf::Vector2f(200,0);
    int sub_steps = 2;
    float counter = 0, t = 0;
    float interval = 0.1f;
    sf::Vector2f gravity = sf::Vector2f(100.0f, 800.0f);
    std::vector<std::unique_ptr<Ball>> balls;
};