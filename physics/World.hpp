#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

#include "PhysicsBody.hpp"

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
    void handleLocalGridCollision(std::vector<std::vector<std::vector<int>>> &grid, int k, int y, int x);
    void update();
    void draw(sf::RenderWindow &window);

    int getBodyCount();
private:
    int winWidth = 800;
    int winHeight = 800;
    sf::Vector2f shooterPos = sf::Vector2f(200,0);
    int sub_steps = 6;
    int ballRadius = 5;
    int objCounter = 0, maxObject = 10000;
    float counter = 0, t = 0;
    float interval = 0.001f;
    float frame_dt = 0.0f;
    sf::Vector2f gravity = sf::Vector2f(0.0f, 1000.0f);
    std::vector<PhysicsBody*> bodies;
    sf::Texture blur;
};