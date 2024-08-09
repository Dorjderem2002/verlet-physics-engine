#include "World.hpp"
#include "body/KinematicBody.hpp"
#include "body/StaticBody.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include <utility>

#define triple_vector std::vector<std::vector<std::vector<int>>>
#define double_vector std::vector<std::vector<int>>

World::World()
{
}

void World::init()
{
    m_blur.loadFromFile("resource/circle.png");

    m_frame_dt = 1.0f / 60.0f;

    m_bodies.reserve(20000);
    m_linkers.reserve(1000);
}

void World::update()
{
    float sub_dt = m_frame_dt / (float)m_sub_steps;

    m_counter += m_frame_dt;
    m_t += 0.001f;
    for (int i = 0; i < m_sub_steps; i++)
    {
        applyGravity();
        applyConstraint();
        switch (algorithm)
        {
        case ALGORITHM::NAIVE:
            collision.resolveCollisionNaive(m_bodies);
            break;
        case ALGORITHM::SORT:
            collision.resolveCollisionSort(m_bodies);
            break;
        case ALGORITHM::GRID:
            collision.resolveCollisionGrid(m_bodies, sections, sf::Vector2f(m_winWidth, m_winHeight));
            break;
        case ALGORITHM::QUAD:
            collision.resolveCollisionQuad(m_bodies, 10, sf::Vector2f(10, 10));
        default:
            collision.resolveCollisionNaive(m_bodies);
            break;
        }
    }
    for (Linker *i_linker : m_linkers)
    {
        i_linker->update();
    }
    updatePosition(sub_dt);
}

void World::applyGravity()
{
    for (PhysicsBody *b : m_bodies)
    {
        b->accelerate(m_gravity);
    }
}

void World::updatePosition(float dt)
{
    for (PhysicsBody *b : m_bodies)
    {
        b->update(dt);
    }
}

void World::applyConstraint()
{
    for (PhysicsBody *b : m_bodies)
    {
        b->wallCollide(m_winWidth, m_winHeight);
    }
}

void World::draw(sf::RenderWindow &window)
{
    sf::VertexArray vertices = generate_vertices(m_bodies, m_blur);
    if (draw_grid)
    {
        collision.draw_cells(&window);
    }
    if (draw_lines)
    {
        for (int i = 0; i < (int)m_linkers.size(); i++)
        {
            sf::Vector2f p1 = m_linkers[i]->m_body_1->get_position();
            sf::Vector2f p2 = m_linkers[i]->m_body_2->get_position();
            sf::Color c1 = m_linkers[i]->m_body_1->getColor();
            sf::Color c2 = m_linkers[i]->m_body_2->getColor();
            sf::Vertex a(p1, (c1 == sf::Color::Transparent) ? sf::Color::White : c1);
            sf::Vertex b(p2, (c2 == sf::Color::Transparent) ? sf::Color::White : c2);
            sf::Vertex l1[2] = {a, b};
            window.draw(l1, 2, sf::Lines);
        }
    }
    window.draw(vertices, &m_blur);

    // old slow code
    // for (PhysicsBody *b : m_bodies)
    // {
    //     b->set_texture(&m_blur);
    //     b->draw(window);
    // }
}

int World::getBodyCount() { return (int)m_bodies.size(); }

void World::setSubStep(int count) { m_sub_steps = count; }

void World::set_gravity(sf::Vector2f g)
{
    m_gravity = g;
}

void World::controlBody(sf::Vector2f mousePos)
{
    for (PhysicsBody *t_body : m_bodies)
    {
        if (t_body->contains(mousePos))
        {
            t_body->set_position(mousePos);
            break;
        }
    }
}

void World::add_bodies(std::vector<PhysicsBody *> &t_bodies, std::vector<Linker *> &t_linkers)
{
    for (auto t_body : t_bodies)
    {
        m_bodies.push_back(t_body);
    }
    for (auto t_link : t_linkers)
    {
        m_linkers.push_back(t_link);
    }
}

void World::add_body(PhysicsBody *t_body)
{
    m_bodies.push_back(t_body);
}

void World::add_linker(Linker *t_linker)
{
    m_linkers.push_back(t_linker);
}

World::~World()
{
    // m_csv.writeToFile("data.csv");
}