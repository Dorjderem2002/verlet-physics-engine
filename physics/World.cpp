#include "World.hpp"
#include "KinematicBody.hpp"
#include "StaticBody.hpp"

#include <iostream>
#include <algorithm>
#include <cmath>

bool intersect(float x1, float x2, float y1, float y2) {
    float a = std::max(x1, y1);
    float b = std::min(x2, y2);
    if(a > b) {
        return false;
    }
    return true;
}

void insertionSort(std::vector<PhysicsBody*> arr, int flag)
{
    int n = arr.size();
    int key, j;
    for (int i = 1; i < n; i++) {
        PhysicsBody* obj = arr[i];
        key = (flag) ? arr[i]->getPosition().x : arr[i]->getPosition().y;
        j = i - 1;
        int otherKey = (flag) ? arr[j]->getPosition().x : arr[j]->getPosition().y;
        while (j >= 0 && otherKey > key) {
            arr[j + 1] = arr[j];
            j--;
            otherKey = (flag) ? arr[j]->getPosition().x : arr[j]->getPosition().y;
        }
        arr[j + 1] = obj;
    }
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

World::World() {

}

void World::init() {

    blur.loadFromFile("resource/blur.png");

    frame_dt = 1.0f / 60.0f;

    //static body init
    // float tr = 20.0f;
    // for(int i=0;i<5;i++) {
    //     bodies.push_back(new StaticBody(sf::Vector2f(200 + i * tr * 2.0f, 300 - i * tr), tr));
    // }
    // for(int i=0;i<5;i++) {
    //     bodies.push_back(new StaticBody(sf::Vector2f(250 - i * tr * 2.0f, 600 - i * tr), tr));
    // }
    // for(int i=0;i<5;i++) {
    //     bodies.push_back(new StaticBody(sf::Vector2f(500 + i * tr * 2.0f, 500), tr));
    // }
}

void World::update() {
    float sub_dt = frame_dt / (float)sub_steps;

    counter += frame_dt;
    t += 0.001f;
    if(counter > interval && objCounter < maxObject) 
    {
        for (int i = 0; i < 2; ++i)
        {
            counter = 0.0f;
            objCounter++;
            shooterPos.x = rand() % 500 + 100;
            int tr = 10;
            KinematicBody* tBody = new KinematicBody(shooterPos, rand() % tr + tr, getRainbow(t));
            tBody->setVelocity(sf::Vector2f(rand() % 2000 - 1000,0), sub_dt);
            tBody->setTexture(&blur);
            bodies.push_back(tBody);
        }   
    }
    for (int i = 0; i < sub_steps; i++)
    {
        applyGravity();
        applyConstraint();
        resolveCollisionSort();
    }
    updatePosition(sub_dt);
}

void World::applyGravity() {
    for(PhysicsBody *b : bodies)
    {
        b->accelerate(gravity);
    }
}

void World::updatePosition(float dt) 
{
    for(PhysicsBody *b : bodies) 
    {
        b->update(dt);
    }
}

void World::applyConstraint() 
{
    for(PhysicsBody *b : bodies) 
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

int World::getBodyCount() {
    return objCounter;
}


void World::resolveCollisionSort() {
    int numberOfBody = bodies.size();
    if(sortAlgo == 0) 
    {
        sort(bodies.begin(), bodies.end(), [](PhysicsBody* lhs, PhysicsBody* rhs )
        {
           return (lhs->getPosition().x < rhs->getPosition().x);
        });
    }
    else 
    {
        insertionSort(bodies, 1);
    }
    
    for(int i = 0;i < numberOfBody; i++) {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            float pos_i = bodies[i]->getPosition().x;
            float pos_j = bodies[j]->getPosition().x;
            float ri = bodies[i]->getRadius();
            float rj = bodies[j]->getRadius();
            if(intersect(pos_i, pos_i + ri * 2, pos_j, pos_j + rj * 2)) {
                if (bodies[i]->isColliding(bodies[j]))
                {
                    if(!bodies[i]->isKinematic())
                        bodies[i]->resolveCollision(bodies[j]);
                    else 
                        bodies[j]->resolveCollision(bodies[i]);
                }
            }
            else {
                break;
            }
        }
    }
    if(sortAlgo == 0) 
    {
        sort(bodies.begin(), bodies.end(), [](PhysicsBody* lhs, PhysicsBody* rhs )
        {
           return (lhs->getPosition().y < rhs->getPosition().y);
        });
    }
    else 
    {
        insertionSort(bodies, 0);
    }
    for(int i = 0;i < numberOfBody; i++) {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            float pos_i = bodies[i]->getPosition().x;
            float pos_j = bodies[j]->getPosition().x;
            float ri = bodies[i]->getRadius();
            float rj = bodies[j]->getRadius();
            if(intersect(pos_i, pos_i + ri * 2, pos_j, pos_j + rj * 2)) {
                if (bodies[i]->isColliding(bodies[j]))
                {
                    if(!bodies[i]->isKinematic())
                        bodies[i]->resolveCollision(bodies[j]);
                    else 
                        bodies[j]->resolveCollision(bodies[i]);
                }
            }
            else {
                break;
            }
        }
    }
}

void World::resolveCollisionNaive() {
    int numberOfBody = bodies.size();
    for(int i = 0;i < numberOfBody; i++) {
        for (int j = i + 1; j < numberOfBody; j++)
        {
            float pos_i = bodies[i]->getPosition().x;
            float pos_j = bodies[j]->getPosition().x;
            float ri = bodies[i]->getRadius();
            float rj = bodies[j]->getRadius();
            if (bodies[i]->isColliding(bodies[j]))
            {
                if(!bodies[i]->isKinematic())
                    bodies[i]->resolveCollision(bodies[j]);
                else 
                    bodies[j]->resolveCollision(bodies[i]);
            }
        }
    }
}