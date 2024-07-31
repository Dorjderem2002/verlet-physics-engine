#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsBody.hpp"

class StaticBody : public PhysicsBody
{
public:
    StaticBody();
    StaticBody(sf::Vector2f p, float radius);
    StaticBody(sf::Vector2f p, float radius, sf::Color t_color);
    StaticBody(float radius);
    void update(float dt);
    void resolveCollision(PhysicsBody *target);
    bool isKinematic();
    ~StaticBody();
};