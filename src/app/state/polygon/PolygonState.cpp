#include "PolygonState.hpp"

PolygonState::PolygonState()
{
}

void PolygonState::init(sf::RenderWindow *t_window, sf::Font &t_font)
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

    StaticBody *body = new StaticBody(sf::Vector2f(400, 400), 5, 50, sf::Color::Red);
    StaticBody *body2 = new StaticBody(sf::Vector2f(400, 450), 4, 50, sf::Color::Blue);
    std::vector<PhysicsBody *> bodies{body, body2};
    m_world.add_body(bodies);
    m_world.init();
}

void PolygonState::update()
{
    sf::Time deltaTime = dtClock.restart();
    float dt = deltaTime.asSeconds();
    int fps = 1.0f / dt;

    int bCount = m_world.getBodyCount();
    m_text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*m_window);
        sf::Vector2f m_worldPos = m_window->mapPixelToCoords(pixelPos);
        m_world.controlBody(m_worldPos);
    }

    move_camera(m_window, m_view);
}

void PolygonState::fixed_update()
{
    m_world.update();
}

void PolygonState::draw()
{
    m_window->clear(sf::Color::Black);
    m_window->draw(m_text);
    m_world.draw(*m_window);
    m_window->display();
}

void PolygonState::event(sf::Event ev)
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
