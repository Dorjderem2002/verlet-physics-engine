#include "collision_utils.hpp"

#include <algorithm>
#include <iostream>

bool intersect(float x1, float x2, float y1, float y2)
{
    float a = std::max(x1, y1);
    float b = std::min(x2, y2);
    if (a > b)
    {
        return false;
    }
    return true;
}

std::vector<PhysicsBody *> only_contains(sf::FloatRect f, std::vector<PhysicsBody *> &b)
{
    std::vector<PhysicsBody *> res;
    for (auto body : b)
    {
        float r = body->getRadius();
        sf::Vector2f p_start = body->get_position() - sf::Vector2f(r, r);
        sf::FloatRect rect(p_start.x, p_start.y, r, r);
        if (f.intersects(rect))
        {
            res.push_back(body);
        }
    }
    return res;
}

Cell::Cell()
{
    bodies = std::vector<PhysicsBody *>();
    shape.setFillColor(sf::Color::Transparent);
}

Cell::Cell(int t_x1, int t_x2, int t_y1, int t_y2)
{
    x1 = t_x1;
    x2 = t_x2;
    y1 = t_y1;
    y2 = t_y2;
    bodies = std::vector<PhysicsBody *>();
    shape.setPosition(x1, y1);
    shape.setSize(sf::Vector2f(abs(x2 - x1), abs(y2 - y1)));
    shape.setFillColor(sf::Color::Transparent);
    shape.setOutlineColor(sf::Color(255, 0, 0, 100));
    shape.setOutlineThickness(1.0f);
}

bool Cell::contain_circle(PhysicsBody *body)
{
    float r = body->getRadius();
    sf::Vector2f p_start = body->get_position() - sf::Vector2f(r, r);
    sf::FloatRect rect(p_start.x, p_start.y, r, r);
    if (shape.getGlobalBounds().intersects(rect))
    {
        return true;
    }
    return false;
}

QuadTree::QuadTree(sf::FloatRect f, std::vector<PhysicsBody *> &t_bodies, int capacity, float area)
{
    if ((int)t_bodies.size() + (int)bodies.size() <= capacity || f.getSize().x * f.getSize().y <= area)
    {
        for (auto t : t_bodies)
        {
            bodies.push_back(t);
        }
        shape.setPosition(f.getPosition());
        shape.setSize(f.getSize());
        shape.setFillColor(sf::Color::Transparent);
        shape.setOutlineColor(sf::Color(255, 0, 0, 100));
        shape.setOutlineThickness(1.0f);
    }
    else
    {
        float x = f.getPosition().x, y = f.getPosition().y;
        float w = f.getSize().x, h = f.getSize().y;

        // left top
        sf::FloatRect f1 = sf::FloatRect(x, y, w / 2, h / 2);
        std::vector<PhysicsBody *> lt = only_contains(f1, t_bodies);
        QuadTree *left_top = new QuadTree(f1, lt, capacity, area);

        // right top
        sf::FloatRect f2 = sf::FloatRect(x + w / 2, y, w / 2, h / 2);
        std::vector<PhysicsBody *> rt = only_contains(f2, t_bodies);
        QuadTree *right_top = new QuadTree(f2, rt, capacity, area);

        // left bottom
        sf::FloatRect f3 = sf::FloatRect(x, y + h / 2, w / 2, h / 2);
        std::vector<PhysicsBody *> lb = only_contains(f3, t_bodies);
        QuadTree *left_bottom = new QuadTree(f3, lb, capacity, area);

        // right bottom
        sf::FloatRect f4 = sf::FloatRect(x + w / 2, y + h / 2, w / 2, h / 2);
        std::vector<PhysicsBody *> rb = only_contains(f4, t_bodies);
        QuadTree *right_bottom = new QuadTree(f4, rb, capacity, area);

        cells.push_back(left_top);
        cells.push_back(right_top);
        cells.push_back(left_bottom);
        cells.push_back(right_bottom);
    }
}

std::vector<Cell> QuadTree::generate()
{
    if (cells.size() == 0)
    {
        return {*this};
    }
    std::vector<Cell> res;
    for (int i = 0; i < (int)cells.size(); i++)
    {
        std::vector<Cell> t = cells[i]->generate();
        for (auto j : t)
        {
            res.push_back(j);
        }
    }
    return res;
}
