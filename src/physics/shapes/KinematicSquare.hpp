#include <vector>
#include "../body/KinematicBody.hpp"
#include "../body/PhysicsBody.hpp"
#include "../linker/Linker.hpp"

class KinematicSquare
{
public:
    KinematicSquare(int x, int y, int w, int h, int radius);
    KinematicSquare(int x, int y, int w, int h, int radius, sf::Color c);
    KinematicSquare(sf::Vector2f pos, sf::Vector2f size, int radius);
    KinematicSquare(sf::Vector2f pos, sf::Vector2f size, int radius, sf::Color c);
    std::vector<PhysicsBody *> m_bodies;
    std::vector<Linker *> m_linkers;
};