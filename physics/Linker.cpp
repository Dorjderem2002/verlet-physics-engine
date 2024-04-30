#include "Linker.hpp"
#include "KinematicBody.hpp"
#include <SFML/Graphics.hpp>

Linker::Linker(PhysicsBody *t_body_1, PhysicsBody *t_body_2, float t_dist)
    : m_body_1(t_body_1), m_body_2(t_body_2), m_target_distance(t_dist)
{
}

void Linker::update()
{
    const sf::Vector2f axis = m_body_1->getPosition() - m_body_2->getPosition();
    const float curr_dis = sqrt(axis.x * axis.x + axis.y * axis.y);
    const float diff = m_target_distance - curr_dis;
    sf::Vector2f dir = axis / curr_dis;
    dir.x *= diff * 0.5f;
    dir.y *= diff * 0.5f;
    if (KinematicBody *t = dynamic_cast<KinematicBody *>(m_body_1))
    {
        m_body_1->setPosition(m_body_1->getPosition() + dir);
    }
    if (KinematicBody *t = dynamic_cast<KinematicBody *>(m_body_2))
    {
        m_body_2->setPosition(m_body_2->getPosition() - dir);
    }
}