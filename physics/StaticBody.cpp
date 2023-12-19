#include "StaticBody.hpp"

StaticBody::StaticBody() {
    shape.setRadius(10.0f);
}

StaticBody::StaticBody(float radius) {
    r = radius;
    shape.setRadius(r);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(r, r);
    shape.setPosition(0,0);
}
StaticBody::StaticBody(sf::Vector2f p, float radius) {
    r = radius;
    shape.setRadius(r);
    shape.setFillColor(sf::Color::White);
    shape.setOrigin(r, r);
    setPosition(p);
}

void StaticBody::update(float dt) {

}

void StaticBody::draw(sf::RenderWindow& win) {
    win.draw(shape);
}

bool StaticBody::isColliding(PhysicsBody* target) {
    sf::Vector2f diff = pos - target->getPosition();
    return r + target->getRadius() > sqrtf(diff.x * diff.x + diff.y * diff.y);
}

void StaticBody::wallCollide(int w, int h) {

}

void StaticBody::resolveCollision(PhysicsBody* target) {
    sf::Vector2f diff = pos - target->getPosition();
    float len_diff = sqrtf(diff.x * diff.x + diff.y * diff.y);
    float len_r = r + target->getRadius();
    float d = len_diff - len_r;
    sf::Vector2f nVec = diff / len_diff;
    sf::Vector2f newPos = target->getPosition() + nVec * (d);
    target->setPosition(newPos);
}

void StaticBody::setPosition(sf::Vector2f newPos) {
    pos = newPos;
    shape.setPosition(newPos);
}

void StaticBody::accelerate(sf::Vector2f a) {

}

void StaticBody::setVelocity(sf::Vector2f v, float dt) {

}

void StaticBody::addVelocity(sf::Vector2f v, float dt) {

}

sf::Vector2f StaticBody::getPosition() {
    return pos;
}

float StaticBody::getRadius() {
    return r;
}

bool StaticBody::isKinematic() {
    return false;
}

StaticBody::~StaticBody() {
    
}