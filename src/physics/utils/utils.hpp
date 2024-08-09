#pragma once

#include <SFML/Graphics.hpp>
#include "../body/PhysicsBody.hpp"

sf::VertexArray generate_vertices(std::vector<PhysicsBody *> &bodies, sf::Texture &blur);
sf::Color getRainbow(float m_t);