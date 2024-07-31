#include "PhysicsBody.hpp"

void PhysicsBody::update(float dt)
{
}

void PhysicsBody::draw(sf::RenderWindow &win)
{
    win.draw(m_shape);
}

bool PhysicsBody::isColliding(PhysicsBody *target)
{
    int n = this->getPoints().size(), m = target->getPoints().size();
    std::vector<sf::Vector2f> point_a = this->getPositionPoints();
    std::vector<sf::Vector2f> point_b = target->getPositionPoints();
    for (int step = 0; step < 2; step++)
    {
        if (step == 1)
        {
            point_a = target->getPositionPoints();
            point_b = this->getPositionPoints();
        }
        // ---------------DIAG-----------------

        // ---------------SAT-----------------
        for (int i = 0; i < n; i++)
        {
            int j = (i + 1) % n;
            sf::Vector2f axis_proj = {-(point_a[j].y - point_a[i].y), point_a[j].x - point_a[i].x};

            float min_r1 = INFINITY, max_r1 = -INFINITY;
            for (int p = 0; p < n; p++)
            {
                float q = (point_a[p].x * axis_proj.x + point_a[p].y * axis_proj.y);
                min_r1 = std::min(min_r1, q);
                max_r1 = std::max(max_r1, q);
            }

            float min_r2 = INFINITY, max_r2 = -INFINITY;
            for (int p = 0; p < m; p++)
            {
                float q = (point_b[p].x * axis_proj.x + point_b[p].y * axis_proj.y);
                min_r2 = std::min(min_r2, q);
                max_r2 = std::max(max_r2, q);
            }
            if (!(max_r2 >= min_r1 && max_r1 >= min_r2))
            {
                m_shape.setFillColor(sf::Color::White);
                target->setColor(sf::Color::White);
                return false;
            }
        }
    }
    m_shape.setFillColor(sf::Color::Red);
    target->setColor(sf::Color::Red);
    return true;
}

sf::Vector2f PhysicsBody::getPrevPosition()
{
    return m_pos;
}

sf::Color PhysicsBody::getColor()
{
    return m_shape.getFillColor();
}

void PhysicsBody::setColor(sf::Color c)
{
    m_shape.setFillColor(c);
}

void PhysicsBody::setOutline(float r)
{
    m_shape.setOutlineThickness(r);
}

bool PhysicsBody::isKinematic()
{
    return false;
}

sf::Vector2f PhysicsBody::getPosition()
{
    return m_pos;
}

std::vector<sf::Vector2f> PhysicsBody::getPoints()
{
    return m_points;
}

std::vector<sf::Vector2f> PhysicsBody::getPositionPoints()
{
    std::vector<sf::Vector2f> copy_pos = m_points;
    for (int i = 0; i < (int)m_points.size(); i++)
    {
        copy_pos[i] += m_pos;
    }
    return copy_pos;
}

void PhysicsBody::resolveCollision(PhysicsBody *target)
{
}

void PhysicsBody::setPosition(sf::Vector2f newPos)
{
    m_pos = newPos;
    m_shape.setPosition(newPos);
}

void PhysicsBody::accelerate(sf::Vector2f a)
{
}

void PhysicsBody::setVelocity(sf::Vector2f v, float dt)
{
}

void PhysicsBody::addVelocity(sf::Vector2f v, float dt)
{
}

bool PhysicsBody::contains(sf::Vector2f loc)
{
    return m_shape.getGlobalBounds().contains(loc);
}

void PhysicsBody::wallCollide(int w, int h)
{
}
