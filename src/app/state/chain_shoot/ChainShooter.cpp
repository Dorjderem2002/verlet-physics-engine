#include "ChainShooter.hpp"

ChainShooter::ChainShooter()
{
}

void ChainShooter::init(sf::RenderWindow *t_window, sf::Font &t_font)
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

    StaticBody *anch1 = new StaticBody(sf::Vector2f(100, 500), 20, sf::Color::White);
    StaticBody *anch2 = new StaticBody(sf::Vector2f(900, 500), 20, sf::Color::White);
    m_world.add_body(anch1);
    m_world.add_body(anch2);

    float chain_size = 20;
    float freedom = 30;
    int n_chains = (anch2->get_position().x - anch1->get_position().x) / (chain_size * 2);
    PhysicsBody *prev = anch1;
    std::vector<PhysicsBody *> bodies;
    std::vector<Linker *> linkers;
    for (int i = 0; i < n_chains; i++)
    {
        KinematicBody *b = new KinematicBody(prev->get_position() + sf::Vector2f(chain_size * 2 + 5, 0), chain_size, sf::Color::Transparent);
        Linker *l = new Linker(prev, b, freedom);
        bodies.push_back(b);
        linkers.push_back(l);
        prev = b;
    }
    Linker *final = new Linker(prev, anch2, freedom);
    linkers.push_back(final);
    m_world.add_bodies(bodies, linkers);

    m_world.setSubStep(10);
    m_world.init();
}

void ChainShooter::update()
{
    sf::Time deltaTime = dtClock.restart();
    float dt = deltaTime.asSeconds();
    int fps = 1.0f / dt;

    int bCount = m_world.getBodyCount();
    m_text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));

    if (m_world.getBodyCount() < 100)
    {
        cnt += 1;
        if (cnt % 20 == 0)
        {
            KinematicBody *b = new KinematicBody(sf::Vector2f(500, 100), 20, sf::Color::Red);
            b->accelerate(sf::Vector2f(rand() % 1000000 - 500000, 0));
            m_world.add_body(b);
            cnt = 0;
        }
    }

    move_camera(m_window, m_view);
}

void ChainShooter::fixed_update()
{
    m_world.update();
}

void ChainShooter::draw()
{
    m_window->clear(sf::Color::Black);
    m_world.draw(*m_window);
    m_window->draw(m_text);
    m_window->display();
}

void ChainShooter::event(sf::Event ev)
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
