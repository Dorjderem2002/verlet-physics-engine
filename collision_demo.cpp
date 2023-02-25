#include <SFML/Graphics.hpp>
#include <vector>
#include <cstring>
#include <iostream>
#include "Ball.h"
using namespace sf;


int main()
{
    RenderWindow window(VideoMode(800, 800), "My window", Style::Default, ContextSettings(24, 8, 8));

    std::vector<Ball> balls;
    Vector2f gravity = Vector2f(0, 1000.0f);

    Clock dtClock;
    Vector2f shooterPos = Vector2f(400,0);
    int sub_steps = 30;
    float timeCounter = 0;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();
        }
        float dt = dtClock.restart().asSeconds();
        float fps = 1.0f / dt;
        timeCounter += dt;
        if (timeCounter > 0.5f)
        {
            Ball ball(shooterPos.x + (rand() % 400 - 200), shooterPos.y, 30.0f);
            balls.push_back(ball);
            timeCounter = 0;
        }
        if (fps < 25.0f)
        {
            std::cout <<"Your score: " << balls.size()<<"\n";
            window.close();
            std::cin >> fps;
            return 0;
        }
        dt /= (float)sub_steps;
        for (int i = 0; i < sub_steps; i++)
        {
            for (int i = 0; i < balls.size(); i++)
            {
                balls[i].accelerate(gravity);
                balls[i].update(dt);
                balls[i].wallCollide(800, 800);
                for (int j = 0; j < balls.size(); j++)
                {
                    if (i != j && balls[i].isColliding(balls[j]))
                    {
                        balls[i].resolveCollision(balls[j]);
                    }
                }
            }
        }
        
        window.clear(Color::Black);

        for (auto &i : balls)
        {
            i.draw(window);
        }
        window.display();
    }

    return 0;
}