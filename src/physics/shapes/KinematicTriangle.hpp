#pragma once

#include <vector>
#include "../body/KinematicBody.hpp"
#include "../body/PhysicsBody.hpp"
#include "../linker/Linker.hpp"

class KinematicTriangle
{
public:
    KinematicTriangle(sf::Vector2f x, sf::Vector2f y, sf::Vector2f z, float r);
    std::vector<PhysicsBody *> m_bodies;
    std::vector<Linker *> m_linkers;
};