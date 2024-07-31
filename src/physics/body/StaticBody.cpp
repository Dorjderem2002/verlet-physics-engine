#include "StaticBody.hpp"

#include <cmath>

StaticBody::StaticBody()
{
}

StaticBody::StaticBody(sf::Vector2f p, std::vector<sf::Vector2f> points)
{
    int n = points.size();
    m_shape.setPointCount(n);
    for (int i = 0; i < n; i++)
    {
        m_shape.setPoint(i, points[i]);
        m_shape.setFillColor(sf::Color::White);
    }
}

StaticBody::StaticBody(sf::Vector2f p, std::vector<sf::Vector2f> points, sf::Color c)
{
    int n = points.size();
    m_shape.setPointCount(n);
    for (int i = 0; i < n; i++)
    {
        m_shape.setPoint(i, points[i]);
        m_shape.setFillColor(c);
    }
}

void StaticBody::update(float dt)
{
    setPosition(m_pos);
}

void StaticBody::draw(sf::RenderWindow &win)
{
    win.draw(m_shape);
}

bool StaticBody::isColliding(PhysicsBody *target)
{
    return false;
}

void StaticBody::wallCollide(int w, int h)
{
}

bool StaticBody::contains(sf::Vector2f loc)
{
    return m_shape.getGlobalBounds().contains(loc);
}

void StaticBody::resolveCollision(PhysicsBody *target)
{

}

void StaticBody::setPosition(sf::Vector2f newPos)
{
    m_pos = newPos;
    m_shape.setPosition(newPos);
}

void StaticBody::accelerate(sf::Vector2f a)
{
}

void StaticBody::setVelocity(sf::Vector2f v, float dt)
{
}

void StaticBody::addVelocity(sf::Vector2f v, float dt)
{
}

sf::Vector2f StaticBody::getPosition()
{
    return m_pos;
}

sf::Vector2f StaticBody::getPrevPosition()
{
    return m_pos;
}

bool StaticBody::isKinematic()
{
    return false;
}

sf::Color StaticBody::getColor()
{
    return sf::Color(255, 255, 255, 255);
}

void StaticBody::setTexture(sf::Texture *t)
{
    m_shape.setTexture(t);
}

StaticBody::~StaticBody()
{
}