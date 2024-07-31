#include "App.hpp"
#include "state/shapes/ShapesState.hpp"

App::App()
{
}

void App::run()
{
    m_curr_state = new ShapesState();
    init(800, 800);
    while (window->isOpen())
    {
        sf::Event ev;
        while (window->pollEvent(ev))
        {
            event(ev);
        }
        fixed_update();
        update();
        draw();
    }
}

void App::init(int w_size, int h_size)
{
    // settings.antialiasingLevel = 16.0;
    window = new sf::RenderWindow(sf::VideoMode(w_size, h_size), "Physics Engine", sf::Style::Default, settings);
    window->setFramerateLimit(60);

    // Font + Text
    font.loadFromFile("resource/robot.ttf");

    // State
    m_curr_state->init(window, font);
}

void App::update()
{
    m_curr_state->update();
}

void App::fixed_update()
{
    m_curr_state->fixed_update();
}

void App::draw()
{
    m_curr_state->draw();
}

void App::event(sf::Event ev)
{
    m_curr_state->event(ev);
}
