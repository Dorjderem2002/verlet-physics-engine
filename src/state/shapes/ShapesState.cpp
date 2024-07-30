#include "ShapesState.hpp"
#include "../../utils/utils.hpp"

ShapesState::ShapesState()
{
}

void ShapesState::init(sf::RenderWindow *t_window, sf::Font &t_font)
{
    m_window = t_window;
    m_view = sf::View(sf::FloatRect(0, 0, 1000, 1000));
    m_window->setFramerateLimit(60);
    m_window->setView(m_view);

    // Font + m_text
    m_text.setFont(t_font);
    m_text.setFillColor(sf::Color::White);
    m_text.setCharacterSize(200);

    // Physics

    // KinematicSquare square(100, 100, 100, 100, 10);
    KinematicSquare square2(150, 0, 100, 100, 10);

    // m_world.add_body(square.m_bodies, square.m_linkers);
    m_world.add_body(square2.m_bodies, square2.m_linkers);

    KinematicTriangle t1(sf::Vector2f(100, 100), sf::Vector2f(200, 200), sf::Vector2f(100, 200), 10);
    KinematicTriangle t2(sf::Vector2f(200, 150), sf::Vector2f(300, 300), sf::Vector2f(200, 300), 10);
    KinematicTriangle t3(sf::Vector2f(50, 100), sf::Vector2f(50, 200), sf::Vector2f(100, 150), 10);
    // KinematicTriangle t4(sf::Vector2f(100, 100), sf::Vector2f(200, 200), sf::Vector2f(100, 200), 10);
    m_world.add_body(t1.m_bodies, t1.m_linkers);
    m_world.add_body(t2.m_bodies, t2.m_linkers);
    m_world.add_body(t3.m_bodies, t3.m_linkers);
    // m_world.add_body(t4.m_bodies, t4.m_linkers);

    m_world.setSubStep(60);
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
    m_world.update();
}

void ShapesState::draw()
{
    m_window->clear(sf::Color::Black);
    m_world.draw(*m_window);
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
