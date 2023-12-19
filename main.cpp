#include <SFML/Graphics.hpp>

#include <vector>
#include <cstring>
#include <iostream>

#include "physics/World.hpp"

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 1.0;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Performance balls", sf::Style::Default, settings);
    window.setFramerateLimit(60);
    sf::Clock dtClock;

    // Font + Text
    sf::Font font;
    font.loadFromFile("resource/robot.ttf");
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(12);

    // Physics
    World world;
    world.init();

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }
        sf::Time deltaTime = dtClock.restart();
        float dt = deltaTime.asSeconds();
        int fps = 1.0f / dt;

        world.update();
        int bCount = world.getBodyCount();
        text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));

        window.clear(sf::Color::Black);
        world.draw(window);
        window.draw(text);
        window.display();
    }

    return 0;
}