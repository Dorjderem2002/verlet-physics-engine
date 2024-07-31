#include "KinematicBody.hpp"

#include <cmath>

KinematicBody::KinematicBody()
{
}

KinematicBody::KinematicBody(sf::Vector2f p, int n, int l)
{
    m_pos = p;
}

KinematicBody::KinematicBody(sf::Vector2f p, int n, int l, sf::Color c)
{
    m_pos = p;
}

void KinematicBody::update(float dt)
{
    sf::Vector2f vel = m_pos - m_old;
    m_old = m_pos;
    m_pos = m_pos + vel + m_acc * dt * dt;
    m_acc = {};

    setPosition(m_pos);
}

void KinematicBody::accelerate(sf::Vector2f a)
{
    m_acc += a;
}

void KinematicBody::resolveCollision(PhysicsBody *target)
{
}

void KinematicBody::setVelocity(sf::Vector2f v, float dt)
{
    m_old = m_pos - (v * dt);
}

void KinematicBody::addVelocity(sf::Vector2f v, float dt)
{
    m_old -= (v * dt);
}

bool KinematicBody::isKinematic()
{
    return true;
}

void KinematicBody::setTexture(sf::Texture *t)
{
    m_shape.setTexture(t);
}

KinematicBody::~KinematicBody()
{
}
