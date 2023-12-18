#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>
#include <iostream>
#include "World.h"
using namespace sf;

int main()
{
    RenderWindow window(VideoMode(800, 800), "Performance balls");
    window.setFramerateLimit(60);
    
    Clock dtClock;

    World world;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        Time deltaTime = dtClock.restart();
        float dt = deltaTime.asSeconds();

        world.update(dt);

        window.clear(Color::Black);
        world.draw(window);
        window.display();
    }

    return 0;
}