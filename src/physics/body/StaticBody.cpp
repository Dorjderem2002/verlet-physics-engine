#include "StaticBody.hpp"

#include <cmath>

StaticBody::StaticBody()
{
    // m_shape.setRadius(10.0f);
    m_shape.setSize(sf::Vector2f(10, 10));
}

StaticBody::StaticBody(float radius)
{
    m_r = radius;
    // m_shape.setRadius(m_r);
    m_shape.setSize(sf::Vector2f(m_r * 2, m_r * 2));
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin(m_r, m_r);
    m_shape.setPosition(0, 0);
}
StaticBody::StaticBody(sf::Vector2f p, float radius)
{
    m_r = radius;
    // m_shape.setRadius(m_r);
    m_shape.setSize(sf::Vector2f(m_r * 2, m_r * 2));
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin(m_r, m_r);
    set_position(p);
}

StaticBody::StaticBody(sf::Vector2f p, float radius, sf::Color t_color)
{
    m_r = radius;
    // m_shape.setRadius(m_r);
    m_shape.setSize(sf::Vector2f(m_r * 2, m_r * 2));
    m_shape.setFillColor(t_color);
    m_shape.setOrigin(m_r, m_r);
    set_position(p);
}

void StaticBody::update(float dt)
{
    set_position(m_pos);
}

void StaticBody::resolveCollision(PhysicsBody *target)
{
    sf::Vector2f diff = m_pos - target->get_position();
    float len_diff = (float)std::sqrt(diff.x * diff.x + diff.y * diff.y);
    float len_r = m_r + target->getRadius();
    float d = len_diff - len_r;
    sf::Vector2f nVec = diff / len_diff;
    sf::Vector2f newPos = target->get_position() + nVec * (d);
    target->set_position(newPos);
}

bool StaticBody::isKinematic()
{
    return false;
}

StaticBody::~StaticBody()
{
}