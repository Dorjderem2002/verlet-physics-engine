#include "Collision.hpp"
#include <iostream>

Collision::Collision()
{
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
    cells.clear();
    cells.push_back(Cell(0, (int)w_size.x, 0, (int)w_size.y));

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
            a = {std::min(c.x1, c.x2), mid, c.y1, c.y2};
            b = {mid, std::max(c.x1, c.x2), c.y1, c.y2};
        }
        else
        {
            int mid = std::min(c.y1, c.y2) + abs(c.y2 - c.y1) / 2;
            a = {c.x1, c.x2, std::min(c.y1, c.y2), mid};
            b = {c.x1, c.x2, mid, std::max(c.y1, c.y2)};
        }
        cells.push_back(a);
        cells.push_back(b);
    }

    for (PhysicsBody *body : bodies)
    {
        for (int j = 0; j < (int)cells.size(); j++)
        {
            if (cells[j].contain_circle(body))
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

void Collision::resolveCollisionQuad(std::vector<PhysicsBody *> &bodies, int c, sf::Vector2f min_size)
{
}

void Collision::draw_cells(sf::RenderWindow *window)
{
    for (auto i : cells)
    {
        window->draw(i.shape);
    }
}
