#include "MenuState.hpp"
#include "../States.hpp"
#include <app/StateManager.hpp>

MenuState::MenuState()
{
}

void MenuState::init(std::shared_ptr<sf::RenderWindow> t_window, sf::Font &t_font)
{
    m_window = t_window;
    m_view = sf::View(sf::FloatRect(0, 0, 1000, 1000));
    m_window->setFramerateLimit(60);
    m_window->setView(m_view);

    // Font + m_text
    font = &t_font;
    m_text.setFont(t_font);
    m_text.setFillColor(sf::Color::White);
    m_text.setCharacterSize(200);

    blank_button = new Button(50, 100, sf::Color::White, sf::Color::Red, t_font, "Blank");
    chain_button = new Button(50, 200, sf::Color::White, sf::Color::Red, t_font, "Chains");
    shapes_button = new Button(50, 300, sf::Color::White, sf::Color::Red, t_font, "Shapes");
    chain_shoot_button = new Button(50, 400, sf::Color::White, sf::Color::Red, t_font, "Chain Shoot");
    truss_button = new Button(50, 500, sf::Color::White, sf::Color::Red, t_font, "Truss");
    particle_button = new Button(50, 600, sf::Color::White, sf::Color::Red, t_font, "Particle");
    quad_button = new Button(50, 700, sf::Color::White, sf::Color::Red, t_font, "Quad Particle");

    // Physics
    m_world.init();
}

void MenuState::update()
{
    sf::Time deltaTime = dtClock.restart();
    float dt = deltaTime.asSeconds();
    int fps = 1.0f / dt;

    int bCount = m_world.getBodyCount();
    m_text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));

    if (chain_button->update(m_window))
    {
        StateManager::curr_state = new ChainState();
        StateManager::curr_state->init(m_window, *font);
    }
    if (blank_button->update(m_window))
    {
        StateManager::curr_state = new BlankState();
        StateManager::curr_state->init(m_window, *font);
    }
    if (shapes_button->update(m_window))
    {
        StateManager::curr_state = new ShapesState();
        StateManager::curr_state->init(m_window, *font);
    }
    if (chain_shoot_button->update(m_window))
    {
        StateManager::curr_state = new ChainShooter();
        StateManager::curr_state->init(m_window, *font);
    }
    if (truss_button->update(m_window))
    {
        StateManager::curr_state = new TrussState();
        StateManager::curr_state->init(m_window, *font);
    }
    if (particle_button->update(m_window))
    {
        StateManager::curr_state = new ParticleState();
        StateManager::curr_state->init(m_window, *font);
    }
    if (quad_button->update(m_window))
    {
        StateManager::curr_state = new QuadParticleState();
        StateManager::curr_state->init(m_window, *font);
    }

    move_camera(m_window, m_view);
}

void MenuState::fixed_update()
{
    m_world.update();
}

void MenuState::draw()
{
    m_window->clear(sf::Color::Black);
    chain_button->draw(m_window);
    blank_button->draw(m_window);
    shapes_button->draw(m_window);
    chain_shoot_button->draw(m_window);
    truss_button->draw(m_window);
    particle_button->draw(m_window);
    quad_button->draw(m_window);
    m_window->display();
}

void MenuState::event(sf::Event ev)
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
