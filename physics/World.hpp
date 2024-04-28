#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "PhysicsBody.hpp"
#include "ThreadPool.hpp"

#define triple_vector std::vector<std::vector<std::vector<int>>>
#define double_vector std::vector<std::vector<int>>

enum ALGORITHM {
    NAIVE,
    SORT,
    GRID,
    GRID_MULTI
};

class World {
public:
    World();
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

public:
    ALGORITHM type;
    int sections = 6;
    int ballRadius = 10;
    int maxObject = 100000;
    int burstRate = 2;
private:
    int winWidth = 8000;
    int winHeight = 8000;
    sf::Vector2f shooterPos = sf::Vector2f(500,200);
    int sub_steps = 6;
    int objCounter = 0;
    float counter = 0, t = 0;
    float interval = 0.001f;
    float frame_dt = 0.0f;
    sf::Vector2f gravity = sf::Vector2f(0.0f, 10000.0f);
    std::vector<PhysicsBody*> bodies;
    sf::Texture blur;

    int gridWidth, gridHeight;
    triple_vector grid;

    ThreadPool *pool;
};