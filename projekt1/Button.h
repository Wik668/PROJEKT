#ifndef BUTTON_H
#define BUTTON_H

#include <SFML/Graphics.hpp>
#include <string>

// Klasa Button reprezentująca przycisk
class Button {
private:
    sf::RectangleShape shape; // Kształt przycisku
    sf::Text text;            // Tekst przycisku
    bool isSelected;          // Flaga zaznaczenia

public:
    // Konstruktor klasy Button
    Button(const sf::Vector2f& size, const sf::Font& font, const std::string& textString, const sf::Vector2f& position);

    // Rysowanie przycisku
    void draw(sf::RenderWindow& window);

    // Ustawienie stanu zaznaczenia
    void setSelected(bool selected);

    // Sprawdzenie, czy punkt znajduje się w obszarze przycisku
    bool contains(const sf::Vector2f& point) const;

    // Sprawdzenie, czy przycisk jest zaznaczony
    bool isSelectedButton() const;

    // Pobranie tekstu przycisku
    std::string getText() const;
};

#endif // BUTTON_H
