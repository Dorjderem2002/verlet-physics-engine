#include "World.hpp"
#include "KinematicBody.hpp"
#include "StaticBody.hpp"

#include <torch/torch.h>

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

    int diameter = ballRadius * 2;
    m_gridWidth = m_winWidth / diameter + 1;
    m_gridHeight = m_winHeight / diameter + 1;
    m_grid = triple_vector(m_gridHeight + 2, double_vector(m_gridWidth + 2));

    m_csv.enableAutoNewRow(12);
    m_csv << "body_1_old_x" << "body_1_old_y" << "body_2_old_x" << "body_2_old_y";
    m_csv << "body_1_x" << "body_1_y" << "body_2_x" << "body_2_y";
    m_csv << "body_1_new_x" << "body_1_new_y" << "body_2_new_x" << "body_2_new_y";
}

void World::update()
{
    float sub_dt = m_frame_dt / (float)m_sub_steps;

    m_counter += m_frame_dt;
    m_t += 0.001f;
    if (m_counter > m_interval && m_objCounter < maxObject)
    {
        for (int i = 0; i < burstRate; ++i)
        {
            m_counter = 0.0f;
            m_objCounter++;
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
            if (!useML)
                resolveCollisionNaive();
            else
                resolveCollisionNaiveML();
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

int World::getBodyCount() { return m_objCounter; }

void World::setSubStep(int count) { m_sub_steps = count; }

void World::controlBody(sf::Vector2f mousePos)
{
    for (PhysicsBody *t_body : m_bodies)
    {
        if(t_body->contains(mousePos))
        {
            t_body->setPosition(mousePos);
            break;
        }
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

void World::resolveCollisionNaiveML()
{
    std::vector<torch::Tensor> tensors;
    std::vector<std::pair<int, int>> index_pairs;
    int numberOfBody = m_bodies.size();

    auto dist = [](sf::Vector2f a, sf::Vector2f b) -> float
    {
        float dx = a.x - b.x, dy = a.y - b.y;
        return sqrt(dx * dx + dy * dy);
    };

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
                    sf::Vector2f old_k = m_bodies[i]->getPrevPosition();
                    sf::Vector2f pos_k = m_bodies[i]->getPosition();
                    sf::Vector2f old_h = m_bodies[j]->getPrevPosition();
                    sf::Vector2f pos_h = m_bodies[j]->getPosition();

                    torch::Tensor input_tensor = torch::tensor({old_k.x,
                                                                old_k.y,
                                                                old_h.x,
                                                                old_h.y,
                                                                pos_k.x,
                                                                pos_k.y,
                                                                pos_h.x,
                                                                pos_h.y,
                                                                dist(pos_k, pos_h)})
                                                     .reshape({1, 9});
                    tensors.push_back(input_tensor);
                    index_pairs.emplace_back(i, j);
                }
            }
        }
    }

    if (!tensors.empty())
    {
        torch::Tensor batch_input_tensor = torch::cat(tensors, 0); // Concatenate on the first dimension
        torch::Tensor output = torchModule.forward({batch_input_tensor}).toTensor();

        // Apply outputs
        for (size_t i = 0; i < index_pairs.size(); ++i)
        {
            int k = index_pairs[i].first;
            int h = index_pairs[i].second;
            auto output_slice = output.slice(0, i, i + 1);
            std::vector<float> output_vector(output_slice.data_ptr<float>(), output_slice.data_ptr<float>() + output_slice.numel());

            m_bodies[k]->setPosition(sf::Vector2f(output_vector[0], output_vector[1]));
            m_bodies[h]->setPosition(sf::Vector2f(output_vector[2], output_vector[3]));
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
                if (!useML)
                    handleLocalGridCollision(k, i, j);
                else
                    handleLocalGridCollisionWithML(k, i, j);
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

void World::handleLocalGridCollisionWithML(int k, int y, int x)
{
    std::vector<torch::Tensor> tensors;
    std::vector<std::pair<int, int>> index_pairs;

    auto dist = [](sf::Vector2f a, sf::Vector2f b) -> float
    {
        float dx = a.x - b.x, dy = a.y - b.y;
        return sqrt(dx * dx + dy * dy);
    };
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
                        sf::Vector2f old_k = m_bodies[k]->getPrevPosition();
                        sf::Vector2f pos_k = m_bodies[k]->getPosition();
                        sf::Vector2f old_h = m_bodies[h]->getPrevPosition();
                        sf::Vector2f pos_h = m_bodies[h]->getPosition();

                        torch::Tensor input_tensor = torch::tensor({old_k.x, old_k.y, old_h.x, old_h.y, pos_k.x, pos_k.y, pos_h.x, pos_h.y, dist(pos_k, pos_h)}).reshape({1, 9});
                        tensors.push_back(input_tensor);
                        index_pairs.emplace_back(k, h);

                        // std::vector<torch::jit::IValue> inputs;
                        // sf::Vector2f old_k = m_bodies[k]->getPrevPosition();
                        // sf::Vector2f pos_k = m_bodies[k]->getPosition();
                        // sf::Vector2f old_h = m_bodies[k]->getPrevPosition();
                        // sf::Vector2f pos_h = m_bodies[h]->getPosition();

                        // auto push_item = torch::tensor({old_k.x, old_k.y, old_h.x, old_h.y, pos_k.x, pos_k.y, pos_h.x, pos_h.y}).reshape({1, 8});
                        // inputs.push_back(push_item);

                        // output
                        // torch::Tensor output;
                        // output = torchModule.forward(inputs).toTensor();
                        // std::vector<float> output_vector(output.data_ptr<float>(), output.data_ptr<float>() + output.numel());
                        // m_bodies[k]->setPosition(sf::Vector2f(output_vector[0], output_vector[1]));
                        // m_bodies[h]->setPosition(sf::Vector2f(output_vector[2], output_vector[3]));
                    }
                }
            }
            if (!tensors.empty())
            {
                torch::Tensor batch_input_tensor = torch::cat(tensors, 0); // Concatenate on the first dimension
                torch::Tensor output = torchModule.forward({batch_input_tensor}).toTensor();

                // Apply outputs
                for (size_t i = 0; i < index_pairs.size(); ++i)
                {
                    int k = index_pairs[i].first;
                    int h = index_pairs[i].second;
                    auto output_slice = output.slice(0, i, i + 1);
                    std::vector<float> output_vector(output_slice.data_ptr<float>(), output_slice.data_ptr<float>() + output_slice.numel());

                    m_bodies[k]->setPosition(sf::Vector2f(output_vector[0], output_vector[1]));
                    m_bodies[h]->setPosition(sf::Vector2f(output_vector[2], output_vector[3]));
                }
            }
        }
    }
}

World::~World()
{
    m_csv.writeToFile("data.csv");
}