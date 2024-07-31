#pragma once

#include "../body/PhysicsBody.hpp"

class Linker
{
public:
    Linker(PhysicsBody *t_body_1, PhysicsBody *t_body_2, float t_dist);
    void update();

private:
    PhysicsBody *m_body_1;
    PhysicsBody *m_body_2;
    float m_target_distance = 0;
};