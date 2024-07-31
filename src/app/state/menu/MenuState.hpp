#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>
#include <iostream>

#include "../state/State.hpp"
#include "../Library.hpp"

class MenuState : public State
{
public:
    MenuState();
    void init(sf::RenderWindow *t_window, sf::Font &t_font) override;
    void update() override;
    void fixed_update() override;
    void draw() override;
    void event(sf::Event ev) override;

private:
    // SFML
    sf::ContextSettings m_settings;
    sf::RenderWindow *m_window;
    sf::View m_view;
    sf::Text m_text;
    sf::Clock dtClock;

    // UI
    Button *chain_button, *shapes_button, *blank_button;

    // PHYSICS
    World m_world;
};