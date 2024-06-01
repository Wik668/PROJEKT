#include "Button.h"

Button::Button(const sf::Vector2f& size, const sf::Font& font, const std::string& textString, const sf::Vector2f& position)
    : isSelected(false) {
    shape.setSize(size);
    shape.setFillColor(sf::Color::White);
    shape.setPosition(position);

    text.setFont(font);
    text.setString(textString);
    text.setCharacterSize(24);
    text.setFillColor(sf::Color::Black);
    text.setPosition(
        position.x + (size.x - text.getGlobalBounds().width) / 2,
        position.y + (size.y - text.getGlobalBounds().height) / 2
        );
}

void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

void Button::setSelected(bool selected) {
    isSelected = selected;
    shape.setFillColor(selected ? sf::Color::Red : sf::Color::White);
    text.setFillColor(selected ? sf::Color::White : sf::Color::Black);
}

bool Button::contains(const sf::Vector2f& point) const {
    return shape.getGlobalBounds().contains(point);
}

bool Button::isSelectedButton() const {
    return isSelected;
}

std::string Button::getText() const {
    return text.getString();
}
