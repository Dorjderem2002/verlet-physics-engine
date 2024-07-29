#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "body/PhysicsBody.hpp"
#include "ThreadPool.hpp"
#include "body/Linker.hpp"

#include "../misc/CSVWriter.hpp"

#define triple_vector std::vector<std::vector<std::vector<int>>>
#define double_vector std::vector<std::vector<int>>

enum ALGORITHM
{
    NAIVE,
    SORT,
    GRID,
    GRID_MULTI
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
    void resolveCollisionNaive();
    void resolveCollisionSort();
    void resolveCollisionGrid();
    void resolveCollisionMultithread();
    void solveCollisionGridInRange(int start, int end);
    void handleLocalGridCollision(int k, int y, int x);
    void update();
    void draw(sf::RenderWindow &window);

    int getBodyCount();
    void setSubStep(int count);

    void controlBody(sf::Vector2f mousePos);
    void add_body(std::vector<PhysicsBody *> &t_bodies, std::vector<Linker *> &t_linkers);

public:
    ALGORITHM type;
    int sections = 6;
    int ballRadius = 10;
    int maxObject = 100000;
    int burstRate = 2;
    bool recordPositions = false;
    sf::Vector2f m_shooterPos = sf::Vector2f(500, 200);
    bool genBodies = false;

private:
    int m_winWidth = 1000;
    int m_winHeight = 1000;
    int m_sub_steps = 6;
    float m_counter = 0, m_t = 0;
    float m_interval = 0.001f;
    float m_frame_dt = 0.0f;
    sf::Vector2f m_gravity = sf::Vector2f(0.0f, 10000.0f);
    std::vector<PhysicsBody *> m_bodies;
    std::vector<Linker *> m_linkers;
    sf::Texture m_blur;

    int m_gridWidth, m_gridHeight;
    triple_vector m_grid;

    ThreadPool *m_pool;
    CSVWriter m_csv;
};