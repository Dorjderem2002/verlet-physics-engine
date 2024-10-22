#include "ShapesState.hpp"

ShapesState::ShapesState()
{
}

void ShapesState::init(std::shared_ptr<sf::RenderWindow> t_window, sf::Font &t_font)
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
    for (int i = 0; i < 500; i++)
    {
        square = new KinematicSquare(rand() % 800 + 100, rand() % 800 + 100, 20, 20, 10, sf::Color::Transparent);
        m_world.add_bodies(square->m_bodies, square->m_linkers);
    }
    // KinematicSquare square(600, 100, 30, 30, 10);
    // m_world.add_bodies(square.m_bodies, square.m_linkers);

    // KinematicTriangle t1(sf::Vector2f(100, 100), sf::Vector2f(200, 200), sf::Vector2f(100, 200), 10);
    // KinematicTriangle t2(sf::Vector2f(200, 150), sf::Vector2f(300, 300), sf::Vector2f(200, 300), 10);
    // KinematicTriangle t3(sf::Vector2f(50, 100), sf::Vector2f(50, 200), sf::Vector2f(100, 150), 10);
    // KinematicTriangle t4(sf::Vector2f(100, 100), sf::Vector2f(200, 200), sf::Vector2f(100, 200), 10);
    // m_world.add_bodies(t1.m_bodies, t1.m_linkers);
    // m_world.add_bodies(t2.m_bodies, t2.m_linkers);
    // m_world.add_bodies(t3.m_bodies, t3.m_linkers);
    // m_world.add_body(t4.m_bodies, t4.m_linkers);

    m_world.algorithm = ALGORITHM::GRID;
    m_world.draw_lines = true;
    m_world.setSubStep(30);
    m_world.init();
}

void ShapesState::update()
{
    sf::Time deltaTime = dtClock.restart();
    float dt = deltaTime.asSeconds();
    int fps = 1.0f / dt;

    int bCount = m_world.getBodyCount();
    m_text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));

    move_camera(m_window, m_view);
}

void ShapesState::fixed_update()
{
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
        sf::Vector2f m_worldPos = m_window->mapPixelToCoords(pixelPos);
        m_world.controlBody(m_worldPos);
    }
    m_world.update();
}

void ShapesState::draw()
{
    m_window->clear(sf::Color::Black);
    m_world.draw(*m_window);
    m_window->draw(m_text);
    m_window->display();
}

void ShapesState::event(sf::Event ev)
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
