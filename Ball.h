#pragma once

#include <SFML/Graphics.hpp>

using namespace sf;

class Ball
{
public:
	Ball(float x, float y, float radius);
	void update(float dt);
	void draw(RenderWindow& win);
	bool isColliding(Ball target);
	void wallCollide(int w, int h);
	void resolveCollision(Ball &target);
	void setPosition(Vector2f newPos);
	void accelerate(Vector2f a);
	Vector2f getPosition();
	float r;

private:
	CircleShape shape;
	Vector2f pos;
	Vector2f vel;
	Vector2f acc;
};

