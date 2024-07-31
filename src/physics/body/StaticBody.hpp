#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsBody.hpp"

class StaticBody : public PhysicsBody
{
public:
    StaticBody();
    StaticBody(sf::Vector2f pos, int n, int l);
    StaticBody(sf::Vector2f pos, int n, int l, sf::Color c);
    void update(float dt);
    void wallCollide(int w, int h);
    void resolveCollision(PhysicsBody *target);
    bool isKinematic();
    void setTexture(sf::Texture *t);
    ~StaticBody();
};