#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>
#include <iostream>

#include "../state/State.hpp"
#include <physics/World.hpp>
#include <utils/utils.hpp>

class ChainState : public State
{
public:
    ChainState();
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

    // PHYSICS
    World m_world;
};