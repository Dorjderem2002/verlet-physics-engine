#pragma once

#include <SFML/Graphics.hpp>

#include "PhysicsBody.hpp"

class KinematicBody : public PhysicsBody
{
public:
    KinematicBody();
    KinematicBody(sf::Vector2f p, float radius, sf::Color c);
    void update(float dt) override;
    void draw(sf::RenderWindow &win) override;
    bool isColliding(PhysicsBody *target) override;
    void wallCollide(int w, int h) override;
    void resolveCollision(PhysicsBody *target) override;
    void setPosition(sf::Vector2f newPos) override;
    void accelerate(sf::Vector2f a) override;
    void setVelocity(sf::Vector2f v, float dt) override;
    void addVelocity(sf::Vector2f v, float dt) override;
    sf::Vector2f getPosition() override;
    sf::Vector2f getPrevPosition() override;
    float getRadius() override;
    bool isKinematic() override;
    sf::Color getColor() override;
    ~KinematicBody();

    void setTexture(sf::Texture *t);

private:
    float m_r;
    sf::RectangleShape m_shape;
    sf::Vector2f m_pos, m_old;
    sf::Vector2f m_acc;
};
