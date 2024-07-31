#pragma once

#include <SFML/Graphics.hpp>

#include "PhysicsBody.hpp"

class KinematicBody : public PhysicsBody
{
public:
    KinematicBody();
    KinematicBody(sf::Vector2f p, int n, int l);
    KinematicBody(sf::Vector2f p, int n, int l, sf::Color c);
    void update(float dt);
    void resolveCollision(PhysicsBody *target);
    void accelerate(sf::Vector2f a);
    void setVelocity(sf::Vector2f v, float dt);
    void addVelocity(sf::Vector2f v, float dt);
    bool isKinematic();
    ~KinematicBody();
    void setTexture(sf::Texture *t);

private:
    sf::Vector2f m_acc;
};
