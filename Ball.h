#pragma once

#include <SFML/Graphics.hpp>

class Ball
{
public:
    Ball();
	Ball(sf::Vector2f p, float radius, sf::Color c);
	void update(float dt);
	void draw(sf::RenderWindow& win);
	bool isColliding(Ball* target);
	void wallCollide(int w, int h);
	void resolveCollision(Ball* target);
	void setPosition(sf::Vector2f newPos);
	void accelerate(sf::Vector2f a);
	sf::Vector2f getPosition();
	float r;

private:
	sf::CircleShape shape;
	sf::Vector2f pos, old;
	sf::Vector2f vel, acc;
};

