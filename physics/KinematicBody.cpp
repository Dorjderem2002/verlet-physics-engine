#include "KinematicBody.hpp"

KinematicBody::KinematicBody() {
    pos.x = 20;
    pos.y = 20;
    old = pos;
    r = 10;
    acc = {0,0};
    shape.setPosition(sf::Vector2f(pos.x, pos.y));
    shape.setRadius(r);
    shape.setOrigin(r, r);
    shape.setFillColor(sf::Color(rand() % 255, rand() % 255, rand() % 255, 255));
}

KinematicBody::KinematicBody(sf::Vector2f p, float radius, sf::Color color)
{
    pos = p;
    old = pos;
    r = radius;
    acc = {0,0};
    shape.setPosition(pos);
    shape.setRadius(r);
    shape.setOrigin(r, r);
    shape.setFillColor(color);
}

void KinematicBody::update(float dt)
{
    sf::Vector2f vel = pos - old;
    old = pos;
    pos = pos + vel + acc * dt * dt;
    acc = {};

    setPosition(pos);
}

void KinematicBody::draw(sf::RenderWindow &win)
{
    win.draw(shape);
}

void KinematicBody::accelerate(sf::Vector2f a)
{
    acc += a;
}

bool KinematicBody::isColliding(PhysicsBody* target)
{
    sf::Vector2f diff = pos - target->getPosition();
    return r + target->getRadius() > sqrtf(diff.x * diff.x + diff.y * diff.y);
}

void KinematicBody::wallCollide(int w, int h)
{
    if (pos.y > h - r)
    {
        pos.y = h - r;
        shape.setPosition(pos);
    }
    if (pos.y < r)
    {
        pos.y = r;
        shape.setPosition(pos);
    }
    if (pos.x > w - r)
    {
        pos.x = w - r;
        shape.setPosition(pos);
    }
    if (pos.x < r)
    {
        pos.x = r;
        shape.setPosition(pos);
    }
}

void KinematicBody::resolveCollision(PhysicsBody* target)
{
    sf::Vector2f diff = pos - target->getPosition();
    float len_diff = sqrtf(diff.x * diff.x + diff.y * diff.y);
    float len_r = r + target->getRadius();
    float d = len_diff - len_r;

    sf::Vector2f nVec = diff / len_diff;

    pos -= nVec * (d / 2.0f);
    sf::Vector2f newPos = target->getPosition() + nVec * (d / 2.0f);
    setPosition(pos);
    target->setPosition(newPos);
}

void KinematicBody::setPosition(sf::Vector2f newPos)
{
    pos = newPos;
    shape.setPosition(newPos);
}

void KinematicBody::setVelocity(sf::Vector2f v, float dt) 
{
    old = pos - (v * dt);
}

void KinematicBody::addVelocity(sf::Vector2f v, float dt) 
{
    old -= (v * dt);
}

sf::Vector2f KinematicBody::getPosition()
{
    return pos;
}

float KinematicBody::getRadius() {
    return r;
}

bool KinematicBody::isKinematic() {
    return true;
}

KinematicBody::~KinematicBody() {
    
}