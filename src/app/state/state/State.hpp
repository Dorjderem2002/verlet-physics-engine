#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>
#include <iostream>

class State
{
public:
    virtual void init(std::shared_ptr<sf::RenderWindow> t_window, sf::Font &t_font) = 0;
    virtual void update() = 0;
    virtual void fixed_update() = 0;
    virtual void draw() = 0;
    virtual void event(sf::Event ev) = 0;
};