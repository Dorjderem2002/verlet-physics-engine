#include "KinematicSquare.hpp"

#include <iostream>

KinematicSquare::KinematicSquare(int x, int y, int w, int h, int radius)
{
    sf::Vector2f base_pos(x, y);
    KinematicBody *top_left = new KinematicBody(base_pos, radius, sf::Color::White);
    KinematicBody *top_right = new KinematicBody(base_pos + sf::Vector2f(w, 0), radius, sf::Color::White);
    KinematicBody *bottom_left = new KinematicBody(base_pos + sf::Vector2f(0, h), radius, sf::Color::White);
    KinematicBody *bottom_right = new KinematicBody(base_pos + sf::Vector2f(w, h), radius, sf::Color::White);

    m_bodies.push_back(top_left);
    m_bodies.push_back(top_right);
    m_bodies.push_back(bottom_left);
    m_bodies.push_back(bottom_right);

    Linker *left = new Linker(top_left, bottom_left, h);
    Linker *diag = new Linker(top_left, bottom_right, sqrtf(w * w + h * h));
    Linker *bottom = new Linker(bottom_left, bottom_right, w);
    Linker *right = new Linker(top_right, bottom_right, h);
    Linker *top = new Linker(top_left, top_right, w);

    m_linkers.push_back(left);
    m_linkers.push_back(diag);
    m_linkers.push_back(bottom);
    m_linkers.push_back(right);
    m_linkers.push_back(top);
}

KinematicSquare::KinematicSquare(int x, int y, int w, int h, int radius, sf::Color c)
{
    sf::Vector2f base_pos(x, y);
    KinematicBody *top_left = new KinematicBody(base_pos, radius, c);
    KinematicBody *top_right = new KinematicBody(base_pos + sf::Vector2f(w, 0), radius, c);
    KinematicBody *bottom_left = new KinematicBody(base_pos + sf::Vector2f(0, h), radius, c);
    KinematicBody *bottom_right = new KinematicBody(base_pos + sf::Vector2f(w, h), radius, c);

    m_bodies.push_back(top_left);
    m_bodies.push_back(top_right);
    m_bodies.push_back(bottom_left);
    m_bodies.push_back(bottom_right);

    Linker *left = new Linker(top_left, bottom_left, h);
    Linker *diag = new Linker(top_left, bottom_right, sqrtf(w * w + h * h));
    Linker *bottom = new Linker(bottom_left, bottom_right, w);
    Linker *right = new Linker(top_right, bottom_right, h);
    Linker *top = new Linker(top_left, top_right, w);

    m_linkers.push_back(left);
    m_linkers.push_back(diag);
    m_linkers.push_back(bottom);
    m_linkers.push_back(right);
    m_linkers.push_back(top);
}

KinematicSquare::KinematicSquare(sf::Vector2f pos, sf::Vector2f size, int radius)
{
    sf::Vector2f base_pos = pos;
    float w = size.x;
    float h = size.y;
    KinematicBody *top_left = new KinematicBody(base_pos, radius, sf::Color::White);
    KinematicBody *top_right = new KinematicBody(base_pos + sf::Vector2f(w, 0), radius, sf::Color::White);
    KinematicBody *bottom_left = new KinematicBody(base_pos + sf::Vector2f(0, h), radius, sf::Color::White);
    KinematicBody *bottom_right = new KinematicBody(base_pos + sf::Vector2f(w, h), radius, sf::Color::White);

    m_bodies.push_back(top_left);
    m_bodies.push_back(top_right);
    m_bodies.push_back(bottom_left);
    m_bodies.push_back(bottom_right);

    Linker *left = new Linker(top_left, bottom_left, h);
    Linker *diag = new Linker(top_left, bottom_right, sqrtf(w * w + h * h));
    Linker *bottom = new Linker(bottom_left, bottom_right, w);
    Linker *right = new Linker(top_right, bottom_right, h);
    Linker *top = new Linker(top_left, top_right, w);

    m_linkers.push_back(left);
    m_linkers.push_back(diag);
    m_linkers.push_back(bottom);
    m_linkers.push_back(right);
    m_linkers.push_back(top);
}

KinematicSquare::KinematicSquare(sf::Vector2f pos, sf::Vector2f size, int radius, sf::Color c)
{
    sf::Vector2f base_pos = pos;
    float w = size.x;
    float h = size.y;
    KinematicBody *top_left = new KinematicBody(base_pos, radius, c);
    KinematicBody *top_right = new KinematicBody(base_pos + sf::Vector2f(w, 0), radius, c);
    KinematicBody *bottom_left = new KinematicBody(base_pos + sf::Vector2f(0, h), radius, c);
    KinematicBody *bottom_right = new KinematicBody(base_pos + sf::Vector2f(w, h), radius, c);

    m_bodies.push_back(top_left);
    m_bodies.push_back(top_right);
    m_bodies.push_back(bottom_left);
    m_bodies.push_back(bottom_right);

    Linker *left = new Linker(top_left, bottom_left, h);
    Linker *diag = new Linker(top_left, bottom_right, sqrtf(w * w + h * h));
    Linker *bottom = new Linker(bottom_left, bottom_right, w);
    Linker *right = new Linker(top_right, bottom_right, h);
    Linker *top = new Linker(top_left, top_right, w);

    m_linkers.push_back(left);
    m_linkers.push_back(diag);
    m_linkers.push_back(bottom);
    m_linkers.push_back(right);
    m_linkers.push_back(top);
}
