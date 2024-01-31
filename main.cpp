#include <SFML/Graphics.hpp>

#include <vector>
#include <cstring>
#include <iostream>

#include "physics/World.hpp"

int main()
{
    sf::ContextSettings settings;
    settings.antialiasingLevel = 16.0;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Performance balls", sf::Style::Default, settings);
    sf::View view(sf::FloatRect(0, 0, 800, 800));
    window.setFramerateLimit(120);
    window.setView(view);
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
            {
                window.close();
            }
            if (event.type == sf::Event::Resized)
            {
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                view.setSize(event.size.width, event.size.height);
                window.setView(sf::View(visibleArea));
            }
            if (event.type == sf::Event::MouseWheelScrolled)
            {
                if (event.mouseWheelScroll.delta > 0)
                {
                    view.zoom(0.75f); // Zoom in
                    window.setView(view);
                }
                else
                {
                    view.zoom(1.1f); // Zoom out
                    window.setView(view);
                }
            }
        }
        sf::Time deltaTime = dtClock.restart();
        float dt = deltaTime.asSeconds();
        int fps = 1.0f / dt;

        world.update();
        int bCount = world.getBodyCount();
        text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            view.move(-5, 0);
            window.setView(view);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            view.move(5, 0);
            window.setView(view);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            view.move(0, -5);
            window.setView(view);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            view.move(0, 5);
            window.setView(view);
        }

        window.clear(sf::Color::Black);
        world.draw(window);
        window.draw(text);
        window.display();
    }

    return 0;
}