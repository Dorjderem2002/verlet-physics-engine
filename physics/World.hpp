#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "PhysicsBody.hpp"

#define triple_vector std::vector<std::vector<std::vector<int>>>
#define double_vector std::vector<std::vector<int>>

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
    void resolveCollisionGrid(int start, int end);
    void handleLocalGridCollision(int k, int y, int x);
    void update();
    void draw(sf::RenderWindow &window);

    int getBodyCount();
    void setSubStep(int count);
private:
    int winWidth = 8000;
    int winHeight = 8000;
    sf::Vector2f shooterPos = sf::Vector2f(200,0);
    int sub_steps = 6;
    int ballRadius = 10;
    int objCounter = 0, maxObject = 100000;
    float counter = 0, t = 0;
    float interval = 0.001f;
    float frame_dt = 0.0f;
    sf::Vector2f gravity = sf::Vector2f(0.0f, 1000.0f);
    std::vector<PhysicsBody*> bodies;
    sf::Texture blur;
    

    int gridWidth, gridHeight;
    triple_vector grid;
};