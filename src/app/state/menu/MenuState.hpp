#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>
#include <iostream>

#include "../state/State.hpp"
#include <physics/World.hpp>
#include <utils/utils.hpp>
#include <ui/button.hpp>
#include <memory>

class MenuState : public State
{
public:
    MenuState();
    void init(std::shared_ptr<sf::RenderWindow> t_window, sf::Font &t_font) override;
    void update() override;
    void fixed_update() override;
    void draw() override;
    void event(sf::Event ev) override;

private:
    // SFML
    sf::ContextSettings m_settings;
    std::shared_ptr<sf::RenderWindow> m_window;
    sf::View m_view;
    sf::Text m_text;
    sf::Clock dtClock;
    sf::Font *font;

    // UI
    Button *chain_button;
    Button *blank_button;
    Button *shapes_button;
    Button *chain_shoot_button;
    Button *truss_button;
    Button *particle_button;
    Button *quad_button;

    // PHYSICS
    World m_world;
};