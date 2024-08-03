#include "Collision.hpp"

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

void Collision::resolveCollisionSort(std::vector<PhysicsBody *> &bodies)
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

void Collision::resolveCollisionNaive(std::vector<PhysicsBody *> &bodies)
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
                {
                    bodies[j]->resolveCollision(bodies[i]);
                }
            }
        }
    }
}