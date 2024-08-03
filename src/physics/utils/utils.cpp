#include "utils.hpp"

sf::VertexArray generate_vertices(std::vector<PhysicsBody *> &bodies, sf::Texture &blur)
{
    sf::VertexArray vertices;
    vertices.resize(4 * (int)bodies.size());
    vertices.setPrimitiveType(sf::Quads);
    for (int i = 0; i < (int)bodies.size(); ++i)
    {
        float x = bodies[i]->getPosition().x - bodies[i]->getRadius();
        float y = bodies[i]->getPosition().y - bodies[i]->getRadius();
        float r = bodies[i]->getRadius() * 2;
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
    return vertices;
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