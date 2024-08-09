#include "collision_utils.hpp"

#include <algorithm>

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
    sf::Vector2f p_end = body->get_position() + sf::Vector2f(r, r);
    sf::Vector2f p_start = body->get_position() - sf::Vector2f(r, r);
    bool hor = x1 <= p_end.x && p_end.x <= x2;
    bool ver = y1 <= p_end.y && p_end.y <= y2;
    if (hor && ver)
    {
        return true;
    }
    else
    {
        hor = x1 <= p_start.x && p_start.x <= x2;
        ver = y1 <= p_start.y && p_start.y <= y2;
        if (hor && ver)
        {
            return true;
        }
    }
    return false;
}
