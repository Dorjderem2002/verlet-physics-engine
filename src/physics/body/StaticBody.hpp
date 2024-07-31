#pragma once

#include <SFML/Graphics.hpp>
#include "PhysicsBody.hpp"

class StaticBody : public PhysicsBody {
public:
    StaticBody();
    StaticBody(sf::Vector2f p, std::vector<sf::Vector2f> points);
    StaticBody(sf::Vector2f p, std::vector<sf::Vector2f> points, sf::Color c);
    void update(float dt) override;
    void draw(sf::RenderWindow& win) override;
    bool isColliding(PhysicsBody* target) override;
    void wallCollide(int w, int h) override;
    bool contains(sf::Vector2f loc) override;
    void resolveCollision(PhysicsBody* target) override;
    void setPosition(sf::Vector2f newPos) override;
    void accelerate(sf::Vector2f a) override;
    void setVelocity(sf::Vector2f v, float dt) override;
    void addVelocity(sf::Vector2f v, float dt) override;
    sf::Vector2f getPosition() override;
    sf::Vector2f getPrevPosition() override;
    bool isKinematic() override;
    sf::Color getColor() override;

    void setTexture(sf::Texture *t);
    ~StaticBody();
private:
    std::vector<sf::Vector2f> points;
    float angle;
    std::vector<sf::Vector2f> model;
    sf::ConvexShape m_shape;
    sf::Vector2f m_pos;
};