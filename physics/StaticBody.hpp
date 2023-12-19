#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsBody.hpp"

class StaticBody : public PhysicsBody {
public:
    StaticBody();
    StaticBody(sf::Vector2f p, float radius);
    StaticBody(float radius);
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;
    bool isColliding(PhysicsBody* target) override;
    void wallCollide(int w, int h) override;
    void resolveCollision(PhysicsBody* target) override;
    void setPosition(sf::Vector2f newPos) override;
    void accelerate(sf::Vector2f a) override;
    void setVelocity(sf::Vector2f v, float dt) override;
    void addVelocity(sf::Vector2f v, float dt) override;
    sf::Vector2f getPosition() override;
    float getRadius() override;
    bool isKinematic() override;
    ~StaticBody();
private:
    sf::CircleShape shape;
    sf::Vector2f pos;
    float r;
};