#include "PhysicsBody.hpp"

#include <cmath>

void PhysicsBody::update(float dt)
{
    set_position(m_pos);
}

void PhysicsBody::draw(sf::RenderWindow &win)
{
    win.draw(m_shape);
}

bool PhysicsBody::isColliding(PhysicsBody *target)
{
    sf::Vector2f diff = m_pos - target->get_position();
    return m_r + target->getRadius() > (float)std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

void PhysicsBody::wallCollide(int w, int h)
{
}

bool PhysicsBody::contains(sf::Vector2f loc)
{
    return m_shape.getGlobalBounds().contains(loc);
}

void PhysicsBody::resolveCollision(PhysicsBody *target)
{
    sf::Vector2f diff = m_pos - target->get_position();
    float len_diff = (float)std::sqrt(diff.x * diff.x + diff.y * diff.y);
    float len_r = m_r + target->getRadius();
    float d = len_diff - len_r;
    sf::Vector2f nVec = diff / len_diff;
    sf::Vector2f newPos = target->get_position() + nVec * (d);
    target->set_position(newPos);
}

void PhysicsBody::set_position(sf::Vector2f newPos)
{
    m_pos = newPos;
    m_shape.setPosition(newPos);
}

void PhysicsBody::set_position_old(sf::Vector2f new_pos)
{
    m_pos = new_pos;
    m_old = new_pos;
    m_shape.setPosition(new_pos);
}

void PhysicsBody::accelerate(sf::Vector2f a)
{
}

void PhysicsBody::setVelocity(sf::Vector2f v, float dt)
{
}

void PhysicsBody::addVelocity(sf::Vector2f v, float dt)
{
}

sf::Vector2f PhysicsBody::get_position()
{
    return m_pos;
}

sf::Vector2f PhysicsBody::getPrevPosition()
{
    return m_old;
}

float PhysicsBody::getRadius()
{
    return m_r;
}

bool PhysicsBody::isKinematic()
{
    return false;
}

sf::Color PhysicsBody::getColor()
{
    return m_shape.getFillColor();
}

void PhysicsBody::set_texture(sf::Texture *t)
{
    m_shape.setTexture(t);
}