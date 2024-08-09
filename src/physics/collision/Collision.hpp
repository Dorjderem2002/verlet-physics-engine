#pragma once

#include "../body/PhysicsBody.hpp"
#include "collision_utils.hpp"
#include <vector>
#include <algorithm>

class Collision
{
public:
    Collision();
    void resolveCollisionSort(std::vector<PhysicsBody *> &bodies);
    void resolveCollisionNaive(std::vector<PhysicsBody *> &bodies);
    void resolveCollisionGrid(std::vector<PhysicsBody *> &bodies, int sections, sf::Vector2f w_size);
    void resolveCollisionQuad(std::vector<PhysicsBody *> &bodies, int c, sf::Vector2f min_size);
    void draw_cells(sf::RenderWindow *window);
    std::vector<Cell> cells;
};