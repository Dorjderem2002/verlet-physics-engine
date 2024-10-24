#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "utils/utils.hpp"
#include "body/PhysicsBody.hpp"
#include "linker/Linker.hpp"
#include "collision/Collision.hpp"

#include "../misc/CSVWriter.hpp"

enum ALGORITHM
{
    NAIVE,
    SORT,
    GRID,
    GRID_MULTI,
    QUAD
};

class World
{
public:
    World();
    ~World();
    void init();
    void applyGravity();
    void updatePosition(float dt);
    void applyConstraint();
    void update();
    void draw(sf::RenderWindow &window);

    int getBodyCount();
    void setSubStep(int count);
    void set_gravity(sf::Vector2f g);

    void controlBody(sf::Vector2f mousePos);
    void add_bodies(std::vector<PhysicsBody *> &t_bodies, std::vector<Linker *> &t_linkers);
    void add_body(PhysicsBody *t_body);
    void erase_linker(Linker *t_linker);
    void add_linker(Linker *t_linker);

public:
    ALGORITHM algorithm;
    int sections = 6;
    int maxObject = 100000;
    int burstRate = 2;
    bool recordPositions = false;
    sf::Vector2f m_shooterPos = sf::Vector2f(500, 200);
    bool genBodies = false;
    bool draw_lines = true;
    Collision collision;
    bool draw_grid = false;
    bool b2b_collision = true;

private:
    int m_winWidth = 1000;
    int m_winHeight = 1000;
    int m_sub_steps = 6;
    float m_counter = 0, m_t = 0;
    float m_frame_dt = 0.0f;
    sf::Vector2f m_gravity = sf::Vector2f(0.0f, 10000.0f);
    std::vector<PhysicsBody *> m_bodies;
    std::vector<Linker *> m_linkers;
    sf::Texture m_blur;
    CSVWriter m_csv;
};