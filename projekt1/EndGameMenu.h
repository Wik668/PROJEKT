#ifndef ENDGAMEMENU_H
#define ENDGAMEMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <functional>
#include <Menu.h>

class EndGameMenu {
private:
    sf::Font font;
    sf::Text gameOverText;
    sf::Text survivalTimeText;
    sf::Text killCountText;
    sf::Texture background_texture;
    sf::Sprite background_sprite;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    sf::Music endGameMusic;
    sf::Text backToMenuText;
    sf::Text playAgainText;
    sf::Text exitText;
    bool selectedBackToMenu;

    void updateSelection();

public:
    sf::RectangleShape backToMenuButton;
    sf::RectangleShape playAgainButton;
    sf::RectangleShape exitButton;

    EndGameMenu();
    void playSound();
    void stopSound();
    void playMusic();
    void stopMusic();
    void draw(sf::RenderWindow& window);
    void moveSelectionUp();
    void moveSelectionDown();
    bool isBackToMenuSelected() const;
    bool isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePosition) const;
    void update(const sf::Vector2i& mousePosition);
    void updateStats(float survivalTime, int killCount);
    void setEndMessage(const std::string& message);
    void setTimeLabel(const std::string& label);
    void showEndGameMenu(bool playerWon, float survivalTime, int killCount, sf::RenderWindow& window, bool& gameStarted, bool& gameEnded, bool& survivalMode, sf::Music& gameMusic, sf::Clock& survivalClock, Menu& menu, std::function<void()> resetGame, std::function<void(int)> startRound);
};

#endif // ENDGAMEMENU_H
