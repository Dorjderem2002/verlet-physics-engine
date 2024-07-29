#include "App.hpp"

App::App()
{
}

void App::run()
{
    init(800, 800);
    while (window->isOpen())
    {
        sf::Event ev;
        while (window->pollEvent(ev))
        {
            event(ev);
        }
        fixed_update();
        update();
        draw();
    }
}

void App::init(int w_size, int h_size)
{
    // settings.antialiasingLevel = 16.0;
    window = new sf::RenderWindow(sf::VideoMode(w_size, h_size), "Physics Engine", sf::Style::Default, settings);
    view = sf::View(sf::FloatRect(-1000, -1000, 10000, 10000));
    window->setFramerateLimit(60);
    window->setView(view);

    // Font + Text
    font.loadFromFile("resource/robot.ttf");
    text.setFont(font);
    text.setFillColor(sf::Color::White);
    text.setCharacterSize(200);

    // Physics
    world.burstRate = 1;
    world.maxObject = 5000;
    world.sections = 5;
    world.type = ALGORITHM::NAIVE;
    world.genBodies = false;
    world.setSubStep(6);
    world.ballRadius = 200;

    world.init();
}

void App::update()
{
    sf::Time deltaTime = dtClock.restart();
    float dt = deltaTime.asSeconds();
    int fps = 1.0f / dt;

    if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
    {
        sf::Vector2i pixelPos = sf::Mouse::getPosition(*window);
        sf::Vector2f worldPos = window->mapPixelToCoords(pixelPos);
        world.controlBody(worldPos);
    }

    int bCount = world.getBodyCount();
    text.setString("FPS: " + std::to_string(fps) + "\nBody count: " + std::to_string(bCount));

    float cameraSpeed = 50;
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        view.move(-cameraSpeed, 0);
        window->setView(view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        view.move(cameraSpeed, 0);
        window->setView(view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        view.move(0, -cameraSpeed);
        window->setView(view);
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        view.move(0, cameraSpeed);
        window->setView(view);
    }
}

void App::fixed_update()
{
    world.update();
}

void App::draw()
{
    window->clear(sf::Color::Black);
    world.draw(*window);
    window->draw(text);
    window->display();
}

void App::event(sf::Event ev)
{
    if (ev.type == sf::Event::Closed)
    {
        window->close();
    }
    if (ev.type == sf::Event::Resized)
    {
        sf::FloatRect visibleArea(0, 0, ev.size.width, ev.size.height);
        view.setSize(ev.size.width, ev.size.height);
        window->setView(sf::View(visibleArea));
    }
    if (ev.type == sf::Event::MouseWheelScrolled)
    {
        if (ev.mouseWheelScroll.delta > 0)
        {
            view.zoom(0.75f); // Zoom in
            window->setView(view);
        }
        else
        {
            view.zoom(1.1f); // Zoom out
            window->setView(view);
        }
    }
}
