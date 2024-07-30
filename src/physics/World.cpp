#include "World.hpp"
#include "body/KinematicBody.hpp"
#include "body/StaticBody.hpp"

#include <algorithm>
#include <cmath>
#include <iostream>
#include <thread>
#include <utility>

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

sf::Color getRainbow(float m_t)
{
    const float r = std::sin(m_t);
    const float g = std::sin(m_t + 0.33f * 2.0f * 3.142f);
    const float b = std::sin(m_t + 0.66f * 2.0f * 3.142f);
    return {static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b)};
}

World::World()
{
}

void World::init()
{
    m_pool = new ThreadPool(sections);

    m_blur.loadFromFile("resource/circle.png");

    m_frame_dt = 1.0f / 60.0f;

    m_bodies.reserve(20000);
    m_linkers.reserve(1000);

    // static body init

    // float tr = ballRadius;
    // for (int cnt = 0; cnt < 6; cnt++)
    // {
    //     sf::Vector2f basePos = sf::Vector2f(rand() % m_winWidth, rand() % m_winHeight);
    //     for (int i = 0; i < 4; i++)
    //     {
    //         for (int j = 0; j < 15; j++)
    //         {
    //             StaticBody *staticTemp = new StaticBody(sf::Vector2f(basePos.x + j * tr * 2.0f, basePos.y + i * tr), tr);
    //             staticTemp->setTexture(&m_blur);
    //             m_bodies.push_back(staticTemp);
    //         }
    //     }
    // }

    // linker init

    // {
    //     m_shooterPos.x = rand() % 4000;
    //     m_shooterPos.y = rand() % 4000;
    //     StaticBody *tBody1 =
    //         new StaticBody(m_shooterPos, ballRadius, getRainbow(m_t));
    //     tBody1->setTexture(&m_blur);
    //     m_bodies.push_back(tBody1);
    //     m_shooterPos.x = rand() % 4000;
    //     m_shooterPos.y = rand() % 4000;
    //     KinematicBody *tBody2 =
    //         new KinematicBody(m_shooterPos, ballRadius, getRainbow(m_t));
    //     tBody2->setTexture(&m_blur);
    //     m_bodies.push_back(tBody2);
    //     Linker *tempLink = new Linker(tBody1, tBody2, 400);
    //     m_linkers.push_back(tempLink);
    //     KinematicBody *last = tBody2;
    //     for (int i = 0; i < 10; i++)
    //     {
    //         m_shooterPos.x = rand() % 4000;
    //         m_shooterPos.y = rand() % 4000;
    //         KinematicBody *tBody3 =
    //             new KinematicBody(m_shooterPos, ballRadius, getRainbow(m_t));
    //         tBody3->setTexture(&m_blur);
    //         m_bodies.push_back(tBody3);
    //         Linker *tempLink1 = new Linker(last, tBody3, 400);
    //         m_linkers.push_back(tempLink1);
    //         last = tBody3;
    //     }
    // }

    type = ALGORITHM::NAIVE;
    int diameter = ballRadius * 2;
    m_gridWidth = m_winWidth / diameter + 1;
    m_gridHeight = m_winHeight / diameter + 1;
    m_grid = triple_vector(m_gridHeight + 2, double_vector(m_gridWidth + 2));
}

void World::update()
{
    float sub_dt = m_frame_dt / (float)m_sub_steps;

    m_counter += m_frame_dt;
    m_t += 0.001f;
    if (genBodies && m_counter > m_interval && (int)m_bodies.size() < maxObject)
    {
        for (int i = 0; i < burstRate; ++i)
        {
            m_counter = 0.0f;
            ;
            m_shooterPos.x = rand() % 4000;
            m_shooterPos.y = rand() % 4000;
            KinematicBody *tBody =
                new KinematicBody(m_shooterPos, ballRadius, getRainbow(m_t));
            tBody->setVelocity(sf::Vector2f(0, 0), sub_dt);
            tBody->setTexture(&m_blur);
            m_bodies.push_back(tBody);
        }
    }
    for (int i = 0; i < m_sub_steps; i++)
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
    for (Linker *i_linker : m_linkers)
    {
        i_linker->update();
    }
}

void World::applyGravity()
{
    for (PhysicsBody *b : m_bodies)
    {
        b->accelerate(m_gravity);
    }
}

void World::updatePosition(float dt)
{
    for (PhysicsBody *b : m_bodies)
    {
        b->update(dt);
    }
}

void World::applyConstraint()
{
    for (PhysicsBody *b : m_bodies)
    {
        b->wallCollide(m_winWidth, m_winHeight);
    }
}

void World::draw(sf::RenderWindow &window)
{
    sf::VertexArray vertices;
    vertices.resize(4 * m_bodies.size());
    vertices.setPrimitiveType(sf::Quads);
    for (int i = 0; i < m_bodies.size(); ++i)
    {
        int x = m_bodies[i]->getPosition().x - ballRadius;
        int y = m_bodies[i]->getPosition().y - ballRadius;
        int r = ballRadius * 2;
        // Define the position and texture coordinates for each vertex
        sf::Vertex topLeft(sf::Vector2f(x, y), m_bodies[i]->getColor());
        sf::Vertex topRight(sf::Vector2f(x + r, y), m_bodies[i]->getColor());
        sf::Vertex bottomRight(sf::Vector2f(x + r, y + r), m_bodies[i]->getColor());
        sf::Vertex bottomLeft(sf::Vector2f(x, y + r), m_bodies[i]->getColor());
        topLeft.texCoords = sf::Vector2f(0.0f, 0.0f);
        topRight.texCoords = sf::Vector2f(m_blur.getSize().x, 0.0f);
        bottomRight.texCoords = sf::Vector2f(m_blur.getSize().x, m_blur.getSize().y);
        bottomLeft.texCoords = sf::Vector2f(0.0f, m_blur.getSize().y);
        vertices[4 * i] = topLeft;
        vertices[4 * i + 1] = topRight;
        vertices[4 * i + 2] = bottomRight;
        vertices[4 * i + 3] = bottomLeft;
    }
    window.draw(vertices, &m_blur);
    // old slow code
    // for (PhysicsBody *b : m_bodies)
    // {
    //     b->draw(window);
    // }
}

int World::getBodyCount() { return (int)m_bodies.size(); }

void World::setSubStep(int count) { m_sub_steps = count; }

void World::controlBody(sf::Vector2f mousePos)
{
    for (PhysicsBody *t_body : m_bodies)
    {
        if (t_body->contains(mousePos))
        {
            t_body->setPosition(mousePos);
            break;
        }
    }
}

void World::add_body(std::vector<PhysicsBody *> &t_bodies, std::vector<Linker *> &t_linkers)
{
    for (auto t_body : t_bodies)
    {
        m_bodies.push_back(t_body);
    }
    for (auto t_link : t_linkers)
    {
        m_linkers.push_back(t_link);
    }
}

void World::resolveCollisionSort()
{
    int numberOfBody = m_bodies.size();
    sort(m_bodies.begin(), m_bodies.end(), [](PhysicsBody *lhs, PhysicsBody *rhs)
         { return (lhs->getPosition().x < rhs->getPosition().x); });

    for (int i = 0; i < numberOfBody; i++)
    {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            float pos_i = m_bodies[i]->getPosition().x;
            float pos_j = m_bodies[j]->getPosition().x;
            float ri = m_bodies[i]->getRadius();
            float rj = m_bodies[j]->getRadius();
            if (intersect(pos_i, pos_i + ri * 2, pos_j, pos_j + rj * 2))
            {
                if (m_bodies[i]->isColliding(m_bodies[j]))
                {
                    if (!m_bodies[i]->isKinematic())
                        m_bodies[i]->resolveCollision(m_bodies[j]);
                    else
                        m_bodies[j]->resolveCollision(m_bodies[i]);
                }
            }
            else
            {
                break;
            }
        }
    }
    sort(m_bodies.begin(), m_bodies.end(), [](PhysicsBody *lhs, PhysicsBody *rhs)
         { return (lhs->getPosition().y < rhs->getPosition().y); });
    for (int i = 0; i < numberOfBody; i++)
    {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            float pos_i = m_bodies[i]->getPosition().x;
            float pos_j = m_bodies[j]->getPosition().x;
            float ri = m_bodies[i]->getRadius();
            float rj = m_bodies[j]->getRadius();
            if (intersect(pos_i, pos_i + ri * 2, pos_j, pos_j + rj * 2))
            {
                if (m_bodies[i]->isColliding(m_bodies[j]))
                {
                    if (!m_bodies[i]->isKinematic())
                        m_bodies[i]->resolveCollision(m_bodies[j]);
                    else
                        m_bodies[j]->resolveCollision(m_bodies[i]);
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
    int numberOfBody = m_bodies.size();
    for (int i = 0; i < numberOfBody; i++)
    {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            if (m_bodies[i]->isColliding(m_bodies[j]))
            {
                if (!m_bodies[i]->isKinematic())
                    m_bodies[i]->resolveCollision(m_bodies[j]);
                else
                {
                    m_bodies[j]->resolveCollision(m_bodies[i]);
                }
            }
        }
    }
}

void World::resolveCollisionGrid()
{
    int diameter = ballRadius * 2;
    int numberOfBody = m_bodies.size();
    for (int i = 0; i < m_gridHeight + 2; i++)
        for (int j = 0; j < m_gridWidth + 2; j++)
            m_grid[i][j].clear();

    for (int i = 0; i < numberOfBody; i++)
    {
        sf::Vector2f pos = m_bodies[i]->getPosition();
        int y = pos.y / diameter;
        int x = pos.x / diameter;
        m_grid[y + 1][x + 1].push_back(i);
    }

    for (int i = 1; i <= m_gridHeight; i++)
    {
        for (int j = 1; j <= m_gridWidth; j++)
        {
            for (int k : m_grid[i][j])
            {
                handleLocalGridCollision(k, i, j);
            }
        }
    }
}

void World::resolveCollisionMultithread()
{
    int diameter = ballRadius * 2;
    int numberOfBody = m_bodies.size();
    for (int i = 0; i < m_gridHeight; i++)
        for (int j = 0; j < m_gridWidth; j++)
            m_grid[i][j].clear();

    for (int i = 0; i < numberOfBody; i++)
    {
        sf::Vector2f pos = m_bodies[i]->getPosition();
        int y = pos.y / diameter;
        int x = pos.x / diameter;
        m_grid[y + 1][x + 1].push_back(i);
    }

    for (int i = 1; i <= sections; i++)
    {
        m_pool->addTask([this, i]
                        { this->solveCollisionGridInRange(m_gridWidth / (i - 1) + 1, m_gridWidth / i); });
    }

    m_pool->waitForCompletion();

    // method to create thread every frame

    // std::vector<std::thread> workers;
    // for (int i = 1; i <= sections; i++)
    // {
    //     workers.emplace_back([this, i]
    //                          { this->solveCollisionGridInRange(m_gridWidth / (i - 1) + 1, m_gridWidth / i); });
    // }
    // for (std::thread &w : workers)
    // {
    //     w.join();
    // }
}

void World::solveCollisionGridInRange(int start, int end)
{
    for (int i = 1; i <= m_gridHeight; i++)
    {
        for (int j = start; j <= end; j++)
        {
            for (int k : m_grid[i][j])
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
            for (int h : m_grid[y + i][x + j])
            {
                if (k != h && m_bodies[k]->isColliding(m_bodies[h]))
                {
                    if (!m_bodies[k]->isKinematic())
                    {
                        m_bodies[k]->resolveCollision(m_bodies[h]);
                    }
                    else
                    {
                        if (recordPositions)
                        {
                            m_csv << m_bodies[k]->getPrevPosition().x << m_bodies[k]->getPrevPosition().y;
                            m_csv << m_bodies[h]->getPrevPosition().x << m_bodies[h]->getPrevPosition().y;
                            m_csv << m_bodies[k]->getPosition().x << m_bodies[k]->getPosition().y;
                            m_csv << m_bodies[h]->getPosition().x << m_bodies[h]->getPosition().y;
                        }
                        m_bodies[h]->resolveCollision(m_bodies[k]);
                        if (recordPositions)
                        {
                            m_csv << m_bodies[k]->getPosition().x << m_bodies[k]->getPosition().y;
                            m_csv << m_bodies[h]->getPosition().x << m_bodies[h]->getPosition().y;
                        }
                    }
                }
            }
        }
    }
}

World::~World()
{
    // m_csv.writeToFile("data.csv");
}