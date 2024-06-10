#ifndef ENDGAMEMENU_H
#define ENDGAMEMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <functional>
#include "Menu.h"

// Klasa EndGameMenu reprezentująca menu końca gry
class EndGameMenu {
private:
    sf::Font font;                  // Czcionka używana w menu
    sf::Text gameOverText;          // Tekst informujący o końcu gry
    sf::Text survivalTimeText;      // Tekst informujący o czasie przetrwania
    sf::Text killCountText;           // Tekst informujący o liczbie zabitych wrogów
    sf::Texture background_texture; // Tekstura tła
    sf::Sprite background_sprite;   // Sprite tła
    sf::SoundBuffer buffer;         // Bufor dźwięku
    sf::Sound sound;                // Dźwięk
    sf::Music endGameMusic;         // Muzyka końca gry
    sf::Text backToMenuText;        // Tekst przycisku powrotu do menu
    sf::Text playAgainText;         // Tekst przycisku zagrania ponownie
    sf::Text exitText;              // Tekst przycisku wyjścia
    bool selectedBackToMenu;        // Flaga zaznaczenia przycisku powrotu do menu

    // Aktualizacja zaznaczenia przycisków
    void updateSelection();

public:
    sf::RectangleShape backToMenuButton; // Przycisk powrotu do menu
    sf::RectangleShape playAgainButton;  // Przycisk zagrania ponownie
    sf::RectangleShape exitButton;       // Przycisk wyjścia

    // Konstruktor klasy EndGameMenu
    EndGameMenu();

    // Odtwarzanie dźwięku
    void playSound();

    // Zatrzymanie dźwięku
    void stopSound();

    // Odtwarzanie muzyki
    void playMusic();

    // Zatrzymanie muzyki
    void stopMusic();

    // Rysowanie menu końca gry
    void draw(sf::RenderWindow& window);

    // Przesunięcie zaznaczenia w górę
    void moveSelectionUp();

    // Przesunięcie zaznaczenia w dół
    void moveSelectionDown();

    // Sprawdzenie, czy przycisk powrotu do menu jest zaznaczony
    bool isBackToMenuSelected() const;

    // Sprawdzenie, czy mysz znajduje się nad przyciskiem
    bool isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePosition) const;

    // Aktualizacja stanu menu na podstawie pozycji myszy
    void update(const sf::Vector2i& mousePosition);

    // Aktualizacja statystyk gry
    void updateStats(float survivalTime, int killCount);

    // Ustawienie wiadomości końcowej
    void setEndMessage(const std::string& message);

    // Ustawienie etykiety czasu przetrwania
    void setTimeLabel(const std::string& label);

    // Wyświetlanie menu końca gry
    void showEndGameMenu(bool playerWon, float survivalTime, int killCount, sf::RenderWindow& window, bool& gameStarted, bool& gameEnded, bool& survivalMode, sf::Music& gameMusic, sf::Clock& survivalClock, Menu& menu, std::function<void()> resetGame, std::function<void(int)> startRound);
};

#endif // ENDGAMEMENU_H

