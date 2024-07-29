#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>
#include <iostream>

#include "physics/World.hpp"

int main()
{
    sf::ContextSettings settings;
    // settings.antialiasingLevel = 16.0;
    sf::RenderWindow window(sf::VideoMode(800, 800), "Performance balls", sf::Style::Default, settings);
    sf::View view(sf::FloatRect(-1000, -1000, 10000, 10000));
    window.setFramerateLimit(60);
    window.setView(view);
    sf::Clock dtClock;

    // Font + Text
    sf::Font font;
    font.loadFromFile("resource/robot.ttf");
    sf::Text text;
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(200);

    // Physics
    World world;
    world.burstRate = 1;
    world.maxObject = 5000;
    world.sections = 5;
    world.type = ALGORITHM::NAIVE;
    world.genBodies = false;
    world.setSubStep(6);
    world.ballRadius = 200;

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

        if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            sf::Vector2i pixelPos = sf::Mouse::getPosition(window);
            sf::Vector2f worldPos = window.mapPixelToCoords(pixelPos);
            world.controlBody(worldPos);
        }

        world.update();
        int bCount = world.getBodyCount();
        text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));

        float cameraSpeed = 50;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            view.move(-cameraSpeed, 0);
            window.setView(view);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            view.move(cameraSpeed, 0);
            window.setView(view);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            view.move(0, -cameraSpeed);
            window.setView(view);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        {
            view.move(0, cameraSpeed);
            window.setView(view);
        }

        window.clear(sf::Color::Black);
        world.draw(window);
        window.draw(text);
        window.display();
    }

    return 0;
}