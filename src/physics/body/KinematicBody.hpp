#pragma once

#include <SFML/Graphics.hpp>

#include "PhysicsBody.hpp"

class KinematicBody : public PhysicsBody
{
public:
    KinematicBody();
    KinematicBody(sf::Vector2f p, float radius, sf::Color c);
    void update(float dt);
    void wallCollide(int w, int h);
    void resolveCollision(PhysicsBody *target);
    void accelerate(sf::Vector2f a);
    void setVelocity(sf::Vector2f v, float dt);
    void addVelocity(sf::Vector2f v, float dt);
    bool isKinematic();
    ~KinematicBody();

protected:
    sf::Vector2f m_acc;
};
