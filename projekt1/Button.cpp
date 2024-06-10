#include "Button.h"

// Konstruktor klasy Button inicjalizujący przycisk z podanymi rozmiarem, czcionką, tekstem i pozycją
Button::Button(const sf::Vector2f& size, const sf::Font& font, const std::string& textString, const sf::Vector2f& position)
    : isSelected(false) {
    shape.setSize(size);          // Ustawienie rozmiaru przycisku
    shape.setFillColor(sf::Color::White);  // Ustawienie koloru przycisku
    shape.setPosition(position);  // Ustawienie pozycji przycisku

    text.setFont(font);           // Ustawienie czcionki tekstu
    text.setString(textString);   // Ustawienie tekstu
    text.setCharacterSize(24);    // Ustawienie rozmiaru tekstu
    text.setFillColor(sf::Color::Black);  // Ustawienie koloru tekstu
    // Ustawienie pozycji tekstu w środku przycisku
    text.setPosition(
        position.x + (size.x - text.getGlobalBounds().width) / 2,
        position.y + (size.y - text.getGlobalBounds().height) / 2
        );
}

// Metoda rysująca przycisk na oknie
void Button::draw(sf::RenderWindow& window) {
    window.draw(shape);
    window.draw(text);
}

// Metoda ustawiająca stan zaznaczenia przycisku
void Button::setSelected(bool selected) {
    isSelected = selected;
    // Zmiana koloru przycisku w zależności od stanu zaznaczenia
    shape.setFillColor(selected ? sf::Color::Red : sf::Color::White);
    text.setFillColor(selected ? sf::Color::White : sf::Color::Black);
}

// Metoda sprawdzająca, czy punkt znajduje się wewnątrz przycisku
bool Button::contains(const sf::Vector2f& point) const {
    return shape.getGlobalBounds().contains(point);
}

// Metoda zwracająca stan zaznaczenia przycisku
bool Button::isSelectedButton() const {
    return isSelected;
}

// Metoda zwracająca tekst przycisku
std::string Button::getText() const {
    return text.getString();
}
