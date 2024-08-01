#pragma once

#include <SFML/Graphics.hpp>

class Button
{
public:
    Button(float x, float y, sf::Color idleColor, sf::Color hoverColor, const sf::Font &font, const std::string &textStr)
        : idleColor(idleColor), hoverColor(hoverColor)
    {

        shape.setPosition(x, y);
        shape.setSize(sf::Vector2f(150, 60));
        shape.setFillColor(idleColor);

        text.setFont(font);
        text.setString(textStr);
        text.setCharacterSize(20);           // Set font size
        text.setFillColor(sf::Color::Black); // Set text color
        text.setPosition(shape.getPosition().x + (shape.getSize().x - text.getLocalBounds().width) / 2, shape.getPosition().y + (shape.getSize().y - text.getLocalBounds().height) / 2);
    }

    void draw(sf::RenderWindow *window)
    {
        window->draw(shape);
        window->draw(text);
    }

    bool isMouseOver(sf::RenderWindow *window)
    {
        sf::Vector2i mouse_pos = sf::Mouse::getPosition(*window);
        sf::IntRect button_rect(shape.getPosition().x, shape.getPosition().y, shape.getSize().x, shape.getSize().y);
        sf::Vector2f mapped_mouse_pos = window->mapPixelToCoords(mouse_pos, window->getView());

        return button_rect.contains(mapped_mouse_pos.x, mapped_mouse_pos.y);
    }

    bool update(sf::RenderWindow *window)
    {
        if (isMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            shape.setFillColor(hoverColor);
            return true;
        }
        else
        {
            shape.setFillColor(idleColor);
            return false;
        }
    }

private:
    sf::Text text;
    sf::RectangleShape shape;
    sf::Color idleColor;
    sf::Color hoverColor;
};