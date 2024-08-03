#pragma once

#include <SFML/Graphics.hpp>

class PhysicsBody
{
public:
    virtual void update(float dt);
    virtual void draw(sf::RenderWindow &win);
    virtual bool isColliding(PhysicsBody *target);
    virtual bool contains(sf::Vector2f loc);
    virtual void wallCollide(int w, int h);
    virtual void resolveCollision(PhysicsBody *target);
    virtual void setPosition(sf::Vector2f new_pos);
    virtual void set_position_old(sf::Vector2f new_pos);
    virtual void accelerate(sf::Vector2f a);
    virtual void setVelocity(sf::Vector2f v, float dt);
    virtual void addVelocity(sf::Vector2f v, float dt);
    virtual sf::Vector2f get_position();
    virtual sf::Vector2f getPrevPosition();
    virtual float getRadius();
    virtual bool isKinematic();
    virtual sf::Color getColor();
    void set_texture(sf::Texture* t);

protected:
    sf::RectangleShape m_shape;
    sf::Vector2f m_old;
    sf::Vector2f m_pos;
    float m_r;
};