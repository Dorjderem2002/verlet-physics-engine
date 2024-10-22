#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>
#include <iostream>

#include "../state/State.hpp"
#include <physics/World.hpp>
#include <utils/utils.hpp>
#include <physics/body/KinematicBody.hpp>
#include <physics/body/StaticBody.hpp>

class QuadParticleState : public State
{
public:
    QuadParticleState();
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

    //
    bool follow_without_click = false;
    // PHYSICS
    StaticBody *mouse_body;
    World m_world;
};