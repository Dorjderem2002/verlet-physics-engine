#pragma once

#include <SFML/Graphics.hpp>

class PhysicsBody {
public:
    virtual void update(float dt) = 0;
    virtual void draw(sf::RenderWindow& win) = 0;
    virtual bool isColliding(PhysicsBody* target) = 0;
    virtual bool contains(sf::Vector2f loc) = 0;
    virtual void wallCollide(int w, int h) = 0;
    virtual void resolveCollision(PhysicsBody* target) = 0;
    virtual void setPosition(sf::Vector2f newPos) = 0;
    virtual void accelerate(sf::Vector2f a) = 0;
    virtual void setVelocity(sf::Vector2f v, float dt) = 0;
    virtual void addVelocity(sf::Vector2f v, float dt) = 0;
    virtual sf::Vector2f getPosition() = 0;
    virtual sf::Vector2f getPrevPosition() = 0;
    virtual bool isKinematic() = 0;
    virtual sf::Color getColor() = 0;
};