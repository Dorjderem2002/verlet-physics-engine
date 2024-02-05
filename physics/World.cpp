#include "World.hpp"
#include "KinematicBody.hpp"
#include "StaticBody.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>
#include <thread>

#define triple_vector std::vector<std::vector<std::vector<int>>>
#define double_vector std::vector<std::vector<int>>

bool intersect(float x1, float x2, float y1, float y2)
{
    float a = std::max(x1, y1);
    float b = std::min(x2, y2);
    if (a > b)
    {
        return false;
    }
    return true;
}

sf::Color getRainbow(float t)
{
    const float r = std::sin(t);
    const float g = std::sin(t + 0.33f * 2.0f * 3.142f);
    const float b = std::sin(t + 0.66f * 2.0f * 3.142f);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

World::World()
{
}

void World::init()
{

    blur.loadFromFile("resource/circle.png");

    frame_dt = 1.0f / 60.0f;

    bodies.reserve(10000);

    // static body init
    // float tr = ballRadius;
    // for (int i = 0; i < 5; i++)
    // {
    //     StaticBody *staticTemp = new StaticBody(sf::Vector2f(200 + i * tr * 2.0f, 300 - i * tr), tr);
    //     staticTemp->setTexture(&blur);
    //     bodies.push_back(staticTemp);
    // }
    // for (int i = 0; i < 5; i++)
    // {
    //     StaticBody *staticTemp = new StaticBody(sf::Vector2f(250 - i * tr * 2.0f, 600 - i * tr), tr);
    //     staticTemp->setTexture(&blur);
    //     bodies.push_back(staticTemp);
    // }
    // for (int i = 0; i < 5; i++)
    // {
    //     StaticBody *staticTemp = new StaticBody(sf::Vector2f(500 + i * tr * 2.0f, 500), tr);
    //     staticTemp->setTexture(&blur);
    //     bodies.push_back(staticTemp);
    // }

    int diameter = ballRadius * 2;
    gridWidth = winWidth / diameter + 2;
    gridHeight = winHeight / diameter + 2;
    grid = triple_vector(gridHeight, double_vector(gridWidth));
}

void World::update()
{
    float sub_dt = frame_dt / (float)sub_steps;

    counter += frame_dt;
    t += 0.001f;
    if (counter > interval && objCounter < maxObject)
    {
        for (int i = 0; i < 2; ++i)
        {
            counter = 0.0f;
            objCounter++;
            shooterPos.x = rand() % 500 + 100;
            KinematicBody *tBody = new KinematicBody(shooterPos, ballRadius, getRainbow(t));
            tBody->setVelocity(sf::Vector2f(rand() % 2000 - 1000, 0), sub_dt);
            tBody->setTexture(&blur);
            bodies.push_back(tBody);
        }
    }
    for (int i = 0; i < sub_steps; i++)
    {
        applyGravity();
        applyConstraint();
        // resolveCollisionSort();
        // resolveCollisionNaive();

        int diameter = ballRadius * 2;
        int numberOfBody = bodies.size();
        for (int i = 0; i < gridHeight; i++)
            for (int j = 0; j < gridWidth; j++)
                grid[i][j].clear();

        for (int i = 0; i < numberOfBody; i++)
        {
            sf::Vector2f pos = bodies[i]->getPosition();
            int y = pos.y / diameter;
            int x = pos.x / diameter;
            grid[y + 1][x + 1].push_back(i);
        }

        resolveCollisionGrid();

        // int n = bodies.size();
        // std::thread left([this, n]
        //                  { this->resolveCollisionGrid(0, n / 2); });
        // std::thread right([this, n]
        //                   { this->resolveCollisionGrid(n / 2, n); });

        // left.join();
        // right.join();
    }
    updatePosition(sub_dt);
}

void World::applyGravity()
{
    for (PhysicsBody *b : bodies)
    {
        b->accelerate(gravity);
    }
}

void World::updatePosition(float dt)
{
    for (PhysicsBody *b : bodies)
    {
        b->update(dt);
    }
}

void World::applyConstraint()
{
    for (PhysicsBody *b : bodies)
    {
        b->wallCollide(800, 800);
    }
}

void World::draw(sf::RenderWindow &window)
{
    for (PhysicsBody *b : bodies)
    {
        b->draw(window);
    }
}

int World::getBodyCount()
{
    return objCounter;
}

void World::setSubStep(int count)
{
    sub_steps = count;
}

void World::resolveCollisionSort()
{
    int numberOfBody = bodies.size();
    sort(bodies.begin(), bodies.end(), [](PhysicsBody *lhs, PhysicsBody *rhs)
         { return (lhs->getPosition().x < rhs->getPosition().x); });

    for (int i = 0; i < numberOfBody; i++)
    {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            float pos_i = bodies[i]->getPosition().x;
            float pos_j = bodies[j]->getPosition().x;
            float ri = bodies[i]->getRadius();
            float rj = bodies[j]->getRadius();
            if (intersect(pos_i, pos_i + ri * 2, pos_j, pos_j + rj * 2))
            {
                if (bodies[i]->isColliding(bodies[j]))
                {
                    if (!bodies[i]->isKinematic())
                        bodies[i]->resolveCollision(bodies[j]);
                    else
                        bodies[j]->resolveCollision(bodies[i]);
                }
            }
            else
            {
                break;
            }
        }
    }
    sort(bodies.begin(), bodies.end(), [](PhysicsBody *lhs, PhysicsBody *rhs)
         { return (lhs->getPosition().y < rhs->getPosition().y); });
    for (int i = 0; i < numberOfBody; i++)
    {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            float pos_i = bodies[i]->getPosition().x;
            float pos_j = bodies[j]->getPosition().x;
            float ri = bodies[i]->getRadius();
            float rj = bodies[j]->getRadius();
            if (intersect(pos_i, pos_i + ri * 2, pos_j, pos_j + rj * 2))
            {
                if (bodies[i]->isColliding(bodies[j]))
                {
                    if (!bodies[i]->isKinematic())
                        bodies[i]->resolveCollision(bodies[j]);
                    else
                        bodies[j]->resolveCollision(bodies[i]);
                }
            }
            else
            {
                break;
            }
        }
    }
}

void World::resolveCollisionNaive()
{
    int numberOfBody = bodies.size();
    for (int i = 0; i < numberOfBody; i++)
    {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            if (bodies[i]->isColliding(bodies[j]))
            {
                if (!bodies[i]->isKinematic())
                    bodies[i]->resolveCollision(bodies[j]);
                else
                    bodies[j]->resolveCollision(bodies[i]);
            }
        }
    }
}

void World::resolveCollisionGrid()
{
    for (int i = 1; i < gridHeight - 1; i++)
    {
        for (int j = 1; j < gridWidth - 1; j++)
        {
            for (int k : grid[i][j])
            {
                handleLocalGridCollision(grid, k, i, j);
            }
        }
    }
}

void World::resolveCollisionGrid(int start, int end)
{
    for (int i = 1; i < gridHeight - 1; i++)
    {
        for (int j = 1; j < gridWidth - 1; j++)
        {
            for (int k : grid[i][j])
            {
                handleLocalGridCollision(grid, k, i, j);
            }
        }
    }
}

void World::handleLocalGridCollision(triple_vector &grid, int k, int y, int x)
{
    for (int i = -1; i <= 1; i++)
    {
        for (int j = -1; j <= 1; j++)
        {
            for (int h : grid[y + i][x + j])
            {
                if (k != h && bodies[k]->isColliding(bodies[h]))
                {
                    if (!bodies[k]->isKinematic())
                        bodies[k]->resolveCollision(bodies[h]);
                    else
                        bodies[h]->resolveCollision(bodies[k]);
                }
            }
        }
    }
}