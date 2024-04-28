#include "KinematicBody.hpp"

#include <cmath>

KinematicBody::KinematicBody() {
    m_pos.x = 20;
    m_pos.y = 20;
    m_old = m_pos;
    m_r = 10;
    m_acc = {0,0};
    m_shape.setPosition(sf::Vector2f(m_pos.x, m_pos.y));
    // m_shape.setRadius(m_r);
    m_shape.setSize(sf::Vector2f(m_r * 2, m_r * 2));
    m_shape.setOrigin(m_r, m_r);
    m_shape.setFillColor(sf::Color(255, 255, 255, 255));
}

KinematicBody::KinematicBody(sf::Vector2f p, float radius, sf::Color color)
{
    m_pos = p;
    m_old = m_pos;
    m_r = radius;
    m_acc = {0,0};
    m_shape.setPosition(m_pos);
    // m_shape.setRadius(m_r);
    m_shape.setSize(sf::Vector2f(m_r * 2, m_r * 2));
    m_shape.setOrigin(m_r, m_r);
    m_shape.setFillColor(color);
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

bool KinematicBody::isColliding(PhysicsBody* target)
{
    sf::Vector2f diff = m_pos - target->getPosition();
    return m_r + target->getRadius() > (float)std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

void KinematicBody::wallCollide(int w, int h)
{
    if (m_pos.y > h - m_r)
    {
        m_pos.y = h - m_r;
        m_shape.setPosition(m_pos);
    }
    if (m_pos.y < m_r)
    {
        m_pos.y = m_r;
        m_shape.setPosition(m_pos);
    }
    if (m_pos.x > w - m_r)
    {
        m_pos.x = w - m_r;
        m_shape.setPosition(m_pos);
    }
    if (m_pos.x < m_r)
    {
        m_pos.x = m_r;
        m_shape.setPosition(m_pos);
    }
}

void KinematicBody::resolveCollision(PhysicsBody* target)
{
    sf::Vector2f diff = m_pos - target->getPosition();
    float len_diff = (float)std::sqrt(diff.x * diff.x + diff.y * diff.y);
    float len_r = m_r + target->getRadius();
    float d = len_diff - len_r;

    sf::Vector2f nVec = diff / len_diff;

    m_pos -= nVec * (d / 2.0f);
    sf::Vector2f newPos = target->getPosition() + nVec * (d / 2.0f);
    setPosition(m_pos);
    target->setPosition(newPos);
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

float KinematicBody::getRadius() {
    return m_r;
}

bool KinematicBody::isKinematic() {
    return true;
}

sf::Color KinematicBody::getColor()
{
    return m_shape.getFillColor();
}

void KinematicBody::setTexture(sf::Texture* t) {
    m_shape.setTexture(t);
}

KinematicBody::~KinematicBody() {
    
}