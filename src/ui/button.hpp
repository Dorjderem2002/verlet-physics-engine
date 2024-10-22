#pragma once

#include <SFML/Graphics.hpp>
#include <memory>

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

    void draw(std::weak_ptr<sf::RenderWindow> window)
    {
        if (!window.expired())
        {
            window.lock()->draw(shape);
            window.lock()->draw(text);
        }
    }

    bool isMouseOver(std::weak_ptr<sf::RenderWindow> window)
    {
        if (!window.expired())
        {
            sf::Vector2i mouse_pos = sf::Mouse::getPosition(*(window.lock()));
            sf::IntRect button_rect(shape.getPosition().x, shape.getPosition().y, shape.getSize().x, shape.getSize().y);
            sf::Vector2f mapped_mouse_pos = window.lock()->mapPixelToCoords(mouse_pos, window.lock()->getView());
            return button_rect.contains(mapped_mouse_pos.x, mapped_mouse_pos.y);
        }
        return false;
    }

    bool update(std::weak_ptr<sf::RenderWindow> window)
    {
        if (!window.expired() && isMouseOver(window) && sf::Mouse::isButtonPressed(sf::Mouse::Left))
        {
            shape.setFillColor(hoverColor);
            return true;
        }
        shape.setFillColor(idleColor);
        return false;
    }

private:
    sf::Text text;
    sf::RectangleShape shape;
    sf::Color idleColor;
    sf::Color hoverColor;
};