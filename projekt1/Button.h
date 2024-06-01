#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    bool isSelected;

public:
    Button(const sf::Vector2f& size, const sf::Font& font, const std::string& textString, const sf::Vector2f& position);

    void draw(sf::RenderWindow& window);
    void setSelected(bool selected);
    bool contains(const sf::Vector2f& point) const;
    bool isSelectedButton() const;
    std::string getText() const;
};

#endif // BUTTON_H
