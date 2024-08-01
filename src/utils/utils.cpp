#include <SFML/Graphics.hpp>
#include "utils.hpp"

void move_camera(sf::RenderWindow *m_window, sf::View &m_view)
{
    float cameraSpeed = 50;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        m_view.move(-cameraSpeed, 0);
        m_window->setView(m_view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        m_view.move(cameraSpeed, 0);
        m_window->setView(m_view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        m_view.move(0, -cameraSpeed);
        m_window->setView(m_view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        m_view.move(0, cameraSpeed);
        m_window->setView(m_view);
    }
}

void draw_line(sf::RenderWindow *m_window, sf::Vector2f a, sf::Vector2f b)
{
    sf::Vertex line[2] = {sf::Vertex(a), sf::Vertex(b)};

    m_window->draw(line, 2, sf::Lines);
}
