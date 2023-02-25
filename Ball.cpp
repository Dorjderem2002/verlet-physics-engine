#include "Ball.h"

Ball::Ball(float x, float y, float radius)
{
    pos.x = x;
    pos.y = y;
    r = radius;
    shape.setPosition(Vector2f(x, y));
    shape.setRadius(radius);
    shape.setOrigin(radius, radius);
    shape.setFillColor(Color(rand() % 255, rand() % 255, rand() % 255, 255));
}

void Ball::update(float dt)
{
    vel += acc * dt;
    vel.y = std::min(vel.y, 3000.0f);
    pos += vel * dt;
    shape.setPosition(pos);
    acc = Vector2f(0,0);
}

void Ball::draw(RenderWindow &win)
{
    win.draw(shape);
}

bool Ball::isColliding(Ball target)
{
    Vector2f diff = pos - target.getPosition();
    return r + target.r > sqrtf(diff.x * diff.x + diff.y * diff.y);
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

void Ball::resolveCollision(Ball &target)
{
    Vector2f diff = pos - target.getPosition();
    float len_diff = sqrtf(diff.x * diff.x + diff.y * diff.y);
    float len_r = r + target.r;
    float d = len_diff - len_r;

    Vector2f nVec = diff / len_diff;

    pos -= nVec * (d / 2.0f);
    Vector2f newPos = target.getPosition() + nVec * (d / 2.0f);
    shape.setPosition(pos);
    target.setPosition(newPos);
}

void Ball::setPosition(Vector2f newPos)
{
    pos = newPos;
    shape.setPosition(newPos);
}

void Ball::accelerate(Vector2f a)
{
    acc = a;
}

Vector2f Ball::getPosition()
{
    return pos;
}
