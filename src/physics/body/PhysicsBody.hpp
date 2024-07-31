#pragma once

#include <SFML/Graphics.hpp>

class PhysicsBody
{
public:
    void update(float dt);
    void draw(sf::RenderWindow &win);
    bool isColliding(PhysicsBody *target);
    bool contains(sf::Vector2f loc);
    void wallCollide(int w, int h);
    void resolveCollision(PhysicsBody *target);
    void setPosition(sf::Vector2f newPos);
    void accelerate(sf::Vector2f a);
    void setVelocity(sf::Vector2f v, float dt);
    void addVelocity(sf::Vector2f v, float dt);
    sf::Vector2f getPosition();
    std::vector<sf::Vector2f> getPositionPoints();
    std::vector<sf::Vector2f> getPoints();
    sf::Vector2f getPrevPosition();
    sf::Color getColor();
    void setColor(sf::Color c);
    void setOutline(float thickness);
    bool isKinematic();

protected:
    float m_angle;
    std::vector<sf::Vector2f> m_points;
    sf::ConvexShape m_shape;
    sf::Vector2f m_pos, m_old;
};