#include "../body/PhysicsBody.hpp"
#include <vector>
#include <algorithm>

class Collision
{
public:
    static void resolveCollisionSort(std::vector<PhysicsBody*> &bodies);
    static void resolveCollisionNaive(std::vector<PhysicsBody*> &bodies);
};