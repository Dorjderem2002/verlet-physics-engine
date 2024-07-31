#include "StaticBody.hpp"

#include <cmath>
#include <iostream>

StaticBody::StaticBody()
{
}

StaticBody::StaticBody(sf::Vector2f p, int n, int l)
{
    m_shape.setPointCount(n);
    m_points.resize(n);
    float theta = 2 * M_PI / n;
    for (int i = 0; i < n; i++)
    {
        m_points[i] = sf::Vector2f(l * cos(i * theta), l * sin(i * theta));
        m_shape.setPoint(i, m_points[i]);
    }
    m_shape.setFillColor(sf::Color::White);
    m_pos = p;
}

StaticBody::StaticBody(sf::Vector2f p, int n, int l, sf::Color c)
{
    m_shape.setPointCount(n);
    m_points.resize(n);
    float theta = 2 * M_PI / n;
    for (int i = 0; i < n; i++)
    {
        m_points[i] = sf::Vector2f(l * cos(i * theta), l * sin(i * theta));
        m_shape.setPoint(i, m_points[i]);
    }
    m_shape.setFillColor(c);
    m_pos = p;
}

void StaticBody::update(float dt)
{
    setPosition(m_pos);
}

void StaticBody::wallCollide(int w, int h)
{
}

void StaticBody::resolveCollision(PhysicsBody *target)
{
}

bool StaticBody::isKinematic()
{
    return false;
}

void StaticBody::setTexture(sf::Texture *t)
{
    m_shape.setTexture(t);
}

StaticBody::~StaticBody()
{
}