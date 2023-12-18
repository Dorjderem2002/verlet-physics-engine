#include "Ball.h"

#include <iostream>

Ball::Ball() {
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

Ball::Ball(sf::Vector2f p, float radius, sf::Color color)
{
    pos = p;
    old = pos;
    r = radius;
    acc = {0,0};
    shape.setPosition(sf::Vector2f(pos.x, pos.y));
    shape.setRadius(r);
    shape.setOrigin(r, r);
    shape.setFillColor(color);
}

void Ball::update(float dt)
{
    vel = pos - old;
    old = pos;
    pos = pos + vel + acc * dt * dt;
    acc = {};

    setPosition(pos);
}

void Ball::draw(sf::RenderWindow &win)
{
    win.draw(shape);
}

void Ball::accelerate(sf::Vector2f a)
{
    acc += a;
}

bool Ball::isColliding(Ball* target)
{
    sf::Vector2f diff = pos - target->getPosition();
    return r + target->r > sqrtf(diff.x * diff.x + diff.y * diff.y);
}

void Ball::wallCollide(int w, int h)
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

void Ball::resolveCollision(Ball* target)
{
    sf::Vector2f diff = pos - target->getPosition();
    float len_diff = sqrtf(diff.x * diff.x + diff.y * diff.y);
    float len_r = r + target->r;
    float d = len_diff - len_r;

    sf::Vector2f nVec = diff / len_diff;

    pos -= nVec * (d / 2.0f);
    sf::Vector2f newPos = target->getPosition() + nVec * (d / 2.0f);
    setPosition(pos);
    target->setPosition(newPos);
}

void Ball::setPosition(sf::Vector2f newPos)
{
    pos = newPos;
    shape.setPosition(newPos);
}

sf::Vector2f Ball::getPosition()
{
    return pos;
}
