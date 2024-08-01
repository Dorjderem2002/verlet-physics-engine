#include "App.hpp"
#include "state/States.hpp"
#include <app/StateManager.hpp>

App::App()
{
}

void App::run()
{
    StateManager::curr_state = new MenuState();
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
    StateManager::curr_state->init(window, font);
}

void App::update()
{
    StateManager::curr_state->update();
}

void App::fixed_update()
{
    StateManager::curr_state->fixed_update();
}

void App::draw()
{
    StateManager::curr_state->draw();
}

void App::event(sf::Event ev)
{
    StateManager::curr_state->event(ev);
}
