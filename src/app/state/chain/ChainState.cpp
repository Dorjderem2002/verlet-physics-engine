#include "ChainState.hpp"

ChainState::ChainState()
{
}

void ChainState::init(sf::RenderWindow *t_window, sf::Font &t_font)
{
    m_window = t_window;
    m_view = sf::View(sf::FloatRect(0, 0, 1000, 1000));
    m_window->setFramerateLimit(60);
    m_window->setView(m_view);

    // Font + m_text
    m_text.setFont(t_font);
    m_text.setFillColor(sf::Color::White);
    m_text.setCharacterSize(20);

    // Physics
    m_world.burstRate = 1;
    m_world.maxObject = 5000;
    m_world.sections = 5;
    m_world.algorithm = ALGORITHM::NAIVE;
    m_world.genBodies = false;
    m_world.setSubStep(6);

    m_world.init();
}

void ChainState::update()
{
    sf::Time deltaTime = dtClock.restart();
    float dt = deltaTime.asSeconds();
    int fps = 1.0f / dt;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
        sf::Vector2f m_worldPos = m_window->mapPixelToCoords(pixelPos);
        m_world.controlBody(m_worldPos);
    }

    int bCount = m_world.getBodyCount();
    m_text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));

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

void ChainState::fixed_update()
{
    m_world.update();
}

void ChainState::draw()
{
    m_window->clear(sf::Color::Black);
    m_world.draw(*m_window);
    m_window->draw(m_text);
    m_window->display();
}

void ChainState::event(sf::Event ev)
{
    if (ev.type == sf::Event::Closed)
    {
        m_window->close();
    }
    if (ev.type == sf::Event::Resized)
    {
        sf::FloatRect visibleArea(0, 0, ev.size.width, ev.size.height);
        m_view.setSize(ev.size.width, ev.size.height);
        m_window->setView(sf::View(visibleArea));
    }
    if (ev.type == sf::Event::MouseWheelScrolled)
    {
        if (ev.mouseWheelScroll.delta > 0)
        {
            m_view.zoom(0.75f); // Zoom in
            m_window->setView(m_view);
        }
        else
        {
            m_view.zoom(1.1f); // Zoom out
            m_window->setView(m_view);
        }
    }
}
