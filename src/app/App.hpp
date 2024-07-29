#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>
#include <iostream>

#include "../physics/World.hpp"

class App
{
public:
    App();
    void run();
    void init(int w_size, int h_size);
    void update();
    void fixed_update();
    void draw();
    void event(sf::Event ev);

private:
    // SFML
    sf::ContextSettings settings;
    sf::RenderWindow *window;
    sf::View view;
    sf::Text text;
    sf::Font font;
    sf::Clock dtClock;

    // PHYSICS
    World world;
};