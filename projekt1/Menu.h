#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

// Klasa Menu reprezentująca menu gry
class Menu {
private:
    sf::Font font;
    sf::Text text;
    sf::Texture background_texture;
    sf::Sprite background_sprite;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    sf::Text survivalText;
    sf::Text stageText;
    bool selectedSurvival;

    // Metoda aktualizująca wybór opcji w menu
    void updateSelection();

public:
    sf::RectangleShape survivalButton;
    sf::RectangleShape stageButton;

    // Konstruktor inicjalizujący menu
    Menu();

    // Metoda odtwarzająca dźwięk
    void playSound();

    // Metoda zatrzymująca dźwięk
    void stopSound();

    // Metoda rysująca menu na oknie
    void draw(sf::RenderWindow& window);

    // Metoda przesuwająca wybór w górę
    void moveSelectionUp();

    // Metoda przesuwająca wybór w dół
    void moveSelectionDown();

    // Metoda sprawdzająca, czy wybrano tryb przetrwania
    bool isSurvivalSelected() const;

    // Metoda sprawdzająca, czy mysz znajduje się nad przyciskiem
    bool isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePosition) const;

    // Metoda aktualizująca stan menu na podstawie pozycji myszy
    void update(const sf::Vector2i& mousePosition);
};

#endif // MENU_H
