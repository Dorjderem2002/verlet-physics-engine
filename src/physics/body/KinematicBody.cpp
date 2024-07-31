#include "KinematicBody.hpp"

#include <cmath>

KinematicBody::KinematicBody()
{
    
}

KinematicBody::KinematicBody(sf::Vector2f p, std::vector<sf::Vector2f> points, sf::Color c)
{

}

void KinematicBody::update(float dt)
{
    sf::Vector2f vel = m_pos - m_old;
    m_old = m_pos;
    m_pos = m_pos + vel + m_acc * dt * dt;
    m_acc = {};

    setPosition(m_pos);
}

void KinematicBody::draw(sf::RenderWindow &win)
{
    win.draw(m_shape);
}

void KinematicBody::accelerate(sf::Vector2f a)
{
    m_acc += a;
}

bool KinematicBody::isColliding(PhysicsBody *target)
{
    return false;
}

bool KinematicBody::contains(sf::Vector2f loc)
{
    return m_shape.getGlobalBounds().contains(loc);
}

void KinematicBody::wallCollide(int w, int h)
{
    // if (m_pos.y > h - m_r)
    // {
    //     m_pos.y = h - m_r;
    //     m_shape.setPosition(m_pos);
    // }
    // if (m_pos.y < m_r)
    // {
    //     m_pos.y = m_r;
    //     m_shape.setPosition(m_pos);
    // }
    // if (m_pos.x > w - m_r)
    // {
    //     m_pos.x = w - m_r;
    //     m_shape.setPosition(m_pos);
    // }
    // if (m_pos.x < m_r)
    // {
    //     m_pos.x = m_r;
    //     m_shape.setPosition(m_pos);
    // }
}

void KinematicBody::resolveCollision(PhysicsBody *target)
{
    
}

void KinematicBody::setPosition(sf::Vector2f newPos)
{
    m_pos = newPos;
    m_shape.setPosition(newPos);
}

void KinematicBody::setVelocity(sf::Vector2f v, float dt)
{
    m_old = m_pos - (v * dt);
}

void KinematicBody::addVelocity(sf::Vector2f v, float dt)
{
    m_old -= (v * dt);
}

sf::Vector2f KinematicBody::getPosition()
{
    return m_pos;
}

sf::Vector2f KinematicBody::getPrevPosition()
{
    return m_old;
}

bool KinematicBody::isKinematic()
{
    return true;
}

sf::Color KinematicBody::getColor()
{
    return m_shape.getFillColor();
}

void KinematicBody::setTexture(sf::Texture *t)
{
    m_shape.setTexture(t);
}

KinematicBody::~KinematicBody()
{
}