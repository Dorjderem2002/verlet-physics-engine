#pragma once

#include <vector>
#include <SFML/Graphics.hpp>
#include <physics/body/PhysicsBody.hpp>

bool intersect(float x1, float x2, float y1, float y2);

class Cell
{
public:
    Cell();
    Cell(int t_x1, int t_x2, int t_y1, int t_y2);
    // inclusive
    int x1;
    int x2;
    int y1;
    int y2;
    std::vector<PhysicsBody *> bodies;
    bool contain_circle(PhysicsBody *body);
    sf::RectangleShape shape;
};

class QuadTree : Cell
{
public:
    QuadTree(std::vector<PhysicsBody *> bodies);

private:
    int capacity;
    float min_area;
};
