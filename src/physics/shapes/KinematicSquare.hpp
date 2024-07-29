#include <vector>
#include "../body/KinematicBody.hpp"
#include "../body/PhysicsBody.hpp"
#include "../body/Linker.hpp"

class KinematicSquare
{
public:
    KinematicSquare(int x, int y, int w, int h, int step);
    std::vector<PhysicsBody *> m_bodies;
    std::vector<Linker *> m_linkers;
};