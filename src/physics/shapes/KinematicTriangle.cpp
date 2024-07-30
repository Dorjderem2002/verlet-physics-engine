#include "KinematicTriangle.hpp"

#include <iostream>

float dist(sf::Vector2f x, sf::Vector2f y)
{
    float a = (x.x - y.x);
    float b = (x.y - y.y);
    return sqrt(a * a + b * b);
}

KinematicTriangle::KinematicTriangle(sf::Vector2f x, sf::Vector2f y, sf::Vector2f z, float r)
{
    KinematicBody *p1 = new KinematicBody(x, r, sf::Color::White);
    KinematicBody *p2 = new KinematicBody(y, r, sf::Color::White);
    KinematicBody *p3 = new KinematicBody(z, r, sf::Color::White);

    m_bodies.push_back(p1);
    m_bodies.push_back(p2);
    m_bodies.push_back(p3);

    Linker *l1 = new Linker(p1, p2, dist(x, y));
    Linker *l2 = new Linker(p1, p3, dist(x, z));
    Linker *l3 = new Linker(p2, p3, dist(y, z));

    m_linkers.push_back(l1);
    m_linkers.push_back(l2);
    m_linkers.push_back(l3);
}