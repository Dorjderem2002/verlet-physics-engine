#include "ParticleState.hpp"

ParticleState::ParticleState()
{
}

void ParticleState::init(sf::RenderWindow *t_window, sf::Font &t_font)
{
    m_window = t_window;
    m_view = sf::View(sf::FloatRect(0, 0, 1000, 1000));
    m_window->setFramerateLimit(60);
    m_window->setView(m_view);

    // Font + m_text
    m_text.setFont(t_font);
    m_text.setFillColor(sf::Color::Red);
    m_text.setCharacterSize(20);

    // Physics

    for (int i = 0; i < 5000; i++)
    {
        sf::Vector2f tp(rand() % 1000, rand() % 1000);
        sf::Color tc(rand() % 255, rand() % 255, rand() % 255, 255);
        KinematicBody *b = new KinematicBody(tp, rand() % 4 + 3, tc);
        int force = 1;
        b->accelerate(sf::Vector2f(rand() % force - force / 2, rand() % force - force / 2));
        m_world.add_body(b);
    }

    m_world.setSubStep(16);
    m_world.sections = 128;
    m_world.algorithm = ALGORITHM::GRID;
    m_world.set_gravity(sf::Vector2f(0, 0));
    m_world.init();
}

void ParticleState::update()
{
    sf::Time deltaTime = dtClock.restart();
    float dt = deltaTime.asSeconds();
    int fps = 1.0f / dt;

    int bCount = m_world.getBodyCount();
    m_text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));

    move_camera(m_window, m_view);
}

void ParticleState::fixed_update()
{
    m_world.update();
}

void ParticleState::draw()
{
    m_window->clear(sf::Color::Black);
    m_world.draw(*m_window);
    m_window->draw(m_text);
    m_window->display();
}

void ParticleState::event(sf::Event ev)
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
