#include "KinematicSquare.hpp"

#include <iostream>

KinematicSquare::KinematicSquare(int x, int y, int w, int h, int step)
{
    sf::Vector2f base_pos(x, y);
    for (int i = 0; i < h / step; i++)
    {
        for (int j = 0; j < w / step; j++)
        {
            sf::Vector2f offset(i * step * 3, j * step * 3);
            KinematicBody *t_body = new KinematicBody(base_pos + offset, step, sf::Color::White);
            m_bodies.push_back(t_body);
        }
    }

    for (int i = 0; i < h / step; i++)
    {
        for (int j = 0; j < w / step; j++)
        {
            int loc = i * (w / step) + j;
            int left = i * (w / step) + j - 1;
            int right = i * (w / step) + j + 1;
            int bottom = (i + 1) * (w / step) + j;
            int top = (i - 1) * (w / step) + j;
            int top_left = (i - 1) * (w / step) + j - 1;
            int top_right = (i - 1) * (w / step) + j + 1;
            int bottom_left = (i + 1) * (w / step) + j - 1;
            int bottom_right = (i + 1) * (w / step) + j + 1;

            PhysicsBody *curr = m_bodies[loc];
            float dist = step * 3;
            // Link to left side
            if (j > 0)
            {
                PhysicsBody *t_body = m_bodies[left];
                Linker *tempLink = new Linker(curr, t_body, dist);
                m_linkers.push_back(tempLink);
            }
            // Link to right side
            if (j < (w / step) - 1)
            {
                PhysicsBody *t_body = m_bodies[right];
                Linker *tempLink = new Linker(curr, t_body, dist);
                m_linkers.push_back(tempLink);
            }
            // Link to top side
            if (i > 0)
            {
                PhysicsBody *t_body = m_bodies[top];
                Linker *tempLink = new Linker(curr, t_body, dist);
                m_linkers.push_back(tempLink);
            }
            // Link to bottom side
            if (i < (h / step) - 1)
            {
                PhysicsBody *t_body = m_bodies[bottom];
                Linker *tempLink = new Linker(curr, t_body, dist);
                m_linkers.push_back(tempLink);
            }
            // Link to top left side
            if (i > 0 && j > 0)
            {
                PhysicsBody *t_body = m_bodies[top_left];
                Linker *tempLink = new Linker(curr, t_body, dist);
                m_linkers.push_back(tempLink);
            }
            // Link to top right side
            if (i > 0 && j < (w / step) - 1)
            {
                PhysicsBody *t_body = m_bodies[top_right];
                Linker *tempLink = new Linker(curr, t_body, dist);
                m_linkers.push_back(tempLink);
            }
            // Link to bottom left side
            if (i < (h / step) - 1 && j > 0)
            {
                PhysicsBody *t_body = m_bodies[bottom_left];
                Linker *tempLink = new Linker(curr, t_body, dist);
                m_linkers.push_back(tempLink);
            }
            // Link to bottom right side
            if (i < (h / step) - 1 && j < (w / step) - 1)
            {
                PhysicsBody *t_body = m_bodies[bottom_right];
                Linker *tempLink = new Linker(curr, t_body, dist);
                m_linkers.push_back(tempLink);
            }
        }
    }
}