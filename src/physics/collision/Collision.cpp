#include "Collision.hpp"

#include <iostream>

struct Cell
{
    // inclusive
    int x1;
    int x2;
    int y1;
    int y2;
    std::vector<PhysicsBody *> bodies;
};

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

void Collision::resolveCollisionSort(std::vector<PhysicsBody *> &bodies)
{
    int numberOfBody = bodies.size();
    sort(bodies.begin(), bodies.end(), [](PhysicsBody *lhs, PhysicsBody *rhs)
         { return (lhs->get_position().x < rhs->get_position().x); });

    for (int i = 0; i < numberOfBody; i++)
    {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            float pos_i = bodies[i]->get_position().x;
            float pos_j = bodies[j]->get_position().x;
            float ri = bodies[i]->getRadius();
            float rj = bodies[j]->getRadius();
            if (intersect(pos_i, pos_i + ri * 2, pos_j, pos_j + rj * 2))
            {
                if (bodies[i]->isColliding(bodies[j]))
                {
                    if (!bodies[i]->isKinematic())
                        bodies[i]->resolveCollision(bodies[j]);
                    else
                        bodies[j]->resolveCollision(bodies[i]);
                }
            }
            else
            {
                break;
            }
        }
    }
    sort(bodies.begin(), bodies.end(), [](PhysicsBody *lhs, PhysicsBody *rhs)
         { return (lhs->get_position().y < rhs->get_position().y); });
    for (int i = 0; i < numberOfBody; i++)
    {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            float pos_i = bodies[i]->get_position().x;
            float pos_j = bodies[j]->get_position().x;
            float ri = bodies[i]->getRadius();
            float rj = bodies[j]->getRadius();
            if (intersect(pos_i, pos_i + ri * 2, pos_j, pos_j + rj * 2))
            {
                if (bodies[i]->isColliding(bodies[j]))
                {
                    if (!bodies[i]->isKinematic())
                        bodies[i]->resolveCollision(bodies[j]);
                    else
                        bodies[j]->resolveCollision(bodies[i]);
                }
            }
            else
            {
                break;
            }
        }
    }
}

void Collision::resolveCollisionNaive(std::vector<PhysicsBody *> &bodies)
{
    int numberOfBody = bodies.size();
    for (int i = 0; i < numberOfBody; i++)
    {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            if (bodies[i]->isColliding(bodies[j]))
            {
                if (!bodies[i]->isKinematic())
                    bodies[i]->resolveCollision(bodies[j]);
                else
                {
                    bodies[j]->resolveCollision(bodies[i]);
                }
            }
        }
    }
}

void Collision::resolveCollisionGrid(std::vector<PhysicsBody *> &bodies, int sections, sf::Vector2f w_size)
{
    int sec_left = sections - 1;
    std::vector<Cell> cells;
    cells.push_back(Cell{0, (int)w_size.x, 0, (int)w_size.y, std::vector<PhysicsBody *>()});

    while (sec_left--)
    {
        int mx_area = 0, mx_index = 0;
        for (int i = 0; i < (int)cells.size(); i++)
        {
            int t_area = abs(cells[i].x2 - cells[i].x1) * abs(cells[i].y2 - cells[i].y1);
            if (t_area > mx_area)
            {
                mx_area = t_area;
                mx_index = i;
            }
        }
        Cell c = cells[mx_index];
        cells.erase(cells.begin() + mx_index);
        Cell a, b;
        if (abs(c.x2 - c.x1) >= abs(c.y2 - c.y1))
        {
            int mid = std::min(c.x1, c.x2) + abs(c.x2 - c.x1) / 2;
            a = {std::min(c.x1, c.x2), mid, c.y1, c.y2, std::vector<PhysicsBody *>()};
            b = {mid, std::max(c.x1, c.x2), c.y1, c.y2, std::vector<PhysicsBody *>()};
        }
        else
        {
            int mid = std::min(c.y1, c.y2) + abs(c.y2 - c.y1) / 2;
            a = {c.x1, c.x2, std::min(c.y1, c.y2), mid, std::vector<PhysicsBody *>()};
            b = {c.x1, c.x2, mid, std::max(c.y1, c.y2), std::vector<PhysicsBody *>()};
        }
        cells.push_back(a);
        cells.push_back(b);
    }

    for (PhysicsBody *body : bodies)
    {
        float r = body->getRadius();
        sf::Vector2f p = body->get_position() + sf::Vector2f(r, r);
        for (int j = 0; j < (int)cells.size(); j++)
        {
            bool hor = cells[j].x1 <= p.x&& p.x <= cells[j].x2;
            bool ver = cells[j].y1 <= p.y && p.y <= cells[j].y2;
            if (hor && ver)
            {
                cells[j].bodies.push_back(body);
            }
        }
    }

    for (int j = 0; j < (int)cells.size(); j++)
    {
        resolveCollisionNaive(cells[j].bodies);
    }
}
