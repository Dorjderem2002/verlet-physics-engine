#include "StaticBody.hpp"

#include <cmath>

StaticBody::StaticBody() {
    // m_shape.setRadius(10.0f);
    m_shape.setSize(sf::Vector2f(10,10));
}

StaticBody::StaticBody(float radius) {
    m_r = radius;
    // m_shape.setRadius(m_r);
    m_shape.setSize(sf::Vector2f(m_r * 2,m_r * 2));
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin(m_r, m_r);
    m_shape.setPosition(0,0);
}
StaticBody::StaticBody(sf::Vector2f p, float radius) {
    m_r = radius;
    // m_shape.setRadius(m_r);
    m_shape.setSize(sf::Vector2f(m_r * 2,m_r * 2));
    m_shape.setFillColor(sf::Color::White);
    m_shape.setOrigin(m_r, m_r);
    setPosition(p);
}

void StaticBody::update(float dt) {

}

void StaticBody::draw(sf::RenderWindow& win) {
    win.draw(m_shape);
}

bool StaticBody::isColliding(PhysicsBody* target) {
    sf::Vector2f diff = m_pos - target->getPosition();
    return m_r + target->getRadius() > (float)std::sqrt(diff.x * diff.x + diff.y * diff.y);
}

void StaticBody::wallCollide(int w, int h) {

}

void StaticBody::resolveCollision(PhysicsBody* target) {
    sf::Vector2f diff = m_pos - target->getPosition();
    float len_diff = (float)std::sqrt(diff.x * diff.x + diff.y * diff.y);
    float len_r = m_r + target->getRadius();
    float d = len_diff - len_r;
    sf::Vector2f nVec = diff / len_diff;
    sf::Vector2f newPos = target->getPosition() + nVec * (d);
    target->setPosition(newPos);
}

void StaticBody::setPosition(sf::Vector2f newPos) {
    m_pos = newPos;
    m_shape.setPosition(newPos);
}

void StaticBody::accelerate(sf::Vector2f a) {

}

void StaticBody::setVelocity(sf::Vector2f v, float dt) {

}

void StaticBody::addVelocity(sf::Vector2f v, float dt) {

}

sf::Vector2f StaticBody::getPosition() {
    return m_pos;
}

sf::Vector2f StaticBody::getPrevPosition() {
    return m_pos;
}

float StaticBody::getRadius() {
    return m_r;
}

bool StaticBody::isKinematic() {
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

StaticBody::~StaticBody() {
    
}