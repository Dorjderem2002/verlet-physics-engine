#include "World.hpp"
#include "KinematicBody.hpp"
#include "StaticBody.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
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
    pool = new ThreadPool(sections);

    blur.loadFromFile("resource/circle.png");

    frame_dt = 1.0f / 60.0f;

    bodies.reserve(20000);

    // static body init

    float tr = ballRadius;
    for (int cnt = 0; cnt < 6; cnt++)
    {
        sf::Vector2f basePos = sf::Vector2f(rand() % winWidth, rand() % winHeight);
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 15; j++)
            {
                StaticBody *staticTemp = new StaticBody(sf::Vector2f(basePos.x + j * tr * 2.0f, basePos.y + i * tr), tr);
                staticTemp->setTexture(&blur);
                bodies.push_back(staticTemp);
            }
        }
    }

    int diameter = ballRadius * 2;
    gridWidth = winWidth / diameter + 1;
    gridHeight = winHeight / diameter + 1;
    grid = triple_vector(gridHeight + 2, double_vector(gridWidth + 2));
}

void World::update()
{
    float sub_dt = frame_dt / (float)sub_steps;

    counter += frame_dt;
    t += 0.001f;
    if (counter > interval && objCounter < maxObject)
    {
        for (int i = 0; i < burstRate; ++i)
        {
            counter = 0.0f;
            objCounter++;
            shooterPos.x = rand() % 100 + 200;
            KinematicBody *tBody =
                new KinematicBody(shooterPos, ballRadius, getRainbow(t));
            tBody->setVelocity(sf::Vector2f(0, 0), sub_dt);
            tBody->setTexture(&blur);
            bodies.push_back(tBody);
        }
    }
    for (int i = 0; i < sub_steps; i++)
    {
        applyGravity();
        applyConstraint();
        switch (type)
        {
        case NAIVE:
            resolveCollisionNaive();
            break;
        case SORT:
            resolveCollisionSort();
            break;
        case GRID:
            resolveCollisionGrid();
            break;
        case GRID_MULTI:
            resolveCollisionMultithread();
            break;
        default:
            resolveCollisionMultithread();
            break;
        }
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
        b->wallCollide(winWidth, winHeight);
    }
}

void World::draw(sf::RenderWindow &window)
{
    sf::VertexArray vertices;
    vertices.resize(4 * bodies.size());
    vertices.setPrimitiveType(sf::Quads);
    for (int i = 0; i < bodies.size(); ++i)
    {
        int x = bodies[i]->getPosition().x - ballRadius;
        int y = bodies[i]->getPosition().y - ballRadius;
        int r = ballRadius * 2;
        // Define the position and texture coordinates for each vertex
        sf::Vertex topLeft(sf::Vector2f(x, y), bodies[i]->getColor());
        sf::Vertex topRight(sf::Vector2f(x + r, y), bodies[i]->getColor());
        sf::Vertex bottomRight(sf::Vector2f(x + r, y + r), bodies[i]->getColor());
        sf::Vertex bottomLeft(sf::Vector2f(x, y + r), bodies[i]->getColor());
        topLeft.texCoords = sf::Vector2f(0.0f, 0.0f);
        topRight.texCoords = sf::Vector2f(blur.getSize().x, 0.0f);
        bottomRight.texCoords = sf::Vector2f(blur.getSize().x, blur.getSize().y);
        bottomLeft.texCoords = sf::Vector2f(0.0f, blur.getSize().y);
        vertices[4 * i] = topLeft;
        vertices[4 * i + 1] = topRight;
        vertices[4 * i + 2] = bottomRight;
        vertices[4 * i + 3] = bottomLeft;
    }
    window.draw(vertices, &blur);
    // old slow code
    // for (PhysicsBody *b : bodies)
    // {
    //     b->draw(window);
    // }
}

int World::getBodyCount() { return objCounter; }

void World::setSubStep(int count) { sub_steps = count; }

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
    int diameter = ballRadius * 2;
    int numberOfBody = bodies.size();
    for (int i = 0; i < gridHeight + 2; i++)
        for (int j = 0; j < gridWidth + 2; j++)
            grid[i][j].clear();

    for (int i = 0; i < numberOfBody; i++)
    {
        sf::Vector2f pos = bodies[i]->getPosition();
        int y = pos.y / diameter;
        int x = pos.x / diameter;
        grid[y + 1][x + 1].push_back(i);
    }

    for (int i = 1; i <= gridHeight; i++)
    {
        for (int j = 1; j <= gridWidth; j++)
        {
            for (int k : grid[i][j])
            {
                handleLocalGridCollision(k, i, j);
            }
        }
    }
}

void World::resolveCollisionMultithread()
{
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

    for (int i = 1; i <= sections; i++)
    {
        pool->addTask([this, i]
                      { this->solveCollisionGridInRange(gridWidth / (i - 1) + 1, gridWidth / i); });
    }

    pool->waitForCompletion();

    // method to create thread every frame

    // std::vector<std::thread> workers;
    // for (int i = 1; i <= sections; i++)
    // {
    //     workers.emplace_back([this, i]
    //                          { this->solveCollisionGridInRange(gridWidth / sections * (i - 1) + 1, gridWidth / sections * i); });
    // }
    // for (std::thread &w : workers)
    // {
    //     w.join();
    // }
}

void World::solveCollisionGridInRange(int start, int end)
{
    for (int i = 1; i <= gridHeight; i++)
    {
        for (int j = start; j <= end; j++)
        {
            for (int k : grid[i][j])
            {
                handleLocalGridCollision(k, i, j);
            }
        }
    }
}

void World::handleLocalGridCollision(int k, int y, int x)
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
