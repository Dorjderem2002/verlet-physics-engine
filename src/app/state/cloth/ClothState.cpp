#include "ClothState.hpp"

ClothState::ClothState()
{
}

void ClothState::init(std::shared_ptr<sf::RenderWindow> t_window, sf::Font &t_font)
{
    m_window = t_window;
    m_view = sf::View(sf::FloatRect(0, 0, 1000, 1000));
    m_window->setFramerateLimit(60);
    m_window->setView(m_view);

    // Font + m_text
    m_text.setFont(t_font);
    m_text.setFillColor(sf::Color::White);
    m_text.setCharacterSize(20);

    int n = 19, m = 19;
    const sf::Vector2f p_offset = {100.0f, 100.0f};

    constexpr float rad = 5, p_dist = 5;
    std::vector<PhysicsBody *> bodies;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            sf::Vector2f pos = sf::Vector2f(j * rad * p_dist, i * rad * p_dist) + p_offset;
            if (i == 0 && j % 2 == 0)
            {
                bodies.push_back(new StaticBody(pos, rad));
            }
            else
            {
                bodies.push_back(new KinematicBody(pos, rad, sf::Color::White));
            }
        }
    }
    std::vector<Linker *> linkers;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (j + 1 < m)
            {
                Linker *link = new Linker(bodies[i * m + j], bodies[i * m + j + 1], rad * p_dist);
                m_world.add_linker(link);
            }
            if (i + 1 < n)
            {
                Linker *link = new Linker(bodies[i * m + j], bodies[i * m + j + n], rad * p_dist);
                m_world.add_linker(link);
            }
        }
    }
    m_world.add_bodies(bodies, linkers);

    // Physics
    m_world.init();
    m_world.sections = 256;
    m_world.setSubStep(50);
    m_world.algorithm = ALGORITHM::QUAD;
    m_world.draw_grid = true;
}

void ClothState::update()
{
    sf::Time deltaTime = dtClock.restart();
    float dt = deltaTime.asSeconds();
    int fps = 1.0f / dt;

    int bCount = m_world.getBodyCount();
    m_text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));
    
    move_camera(m_window, m_view);
}

void ClothState::fixed_update()
{
    m_world.update();
}

void ClothState::draw()
{
    m_window->clear(sf::Color::Black);
    m_world.draw(*m_window);
    m_window->draw(m_text);
    m_window->display();
}

void ClothState::event(sf::Event ev)
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
