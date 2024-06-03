#ifndef ENDGAMEMENU_H
#define ENDGAMEMENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

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
    void draw(sf::RenderWindow& window);
    void moveSelectionUp();
    void moveSelectionDown();
    bool isBackToMenuSelected() const;
    bool isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePosition) const;
    void update(const sf::Vector2i& mousePosition);
    void updateStats(float survivalTime, int killCount);
    void setEndMessage(const std::string& message); // Add this method
    void setTimeLabel(const std::string& label);    // Add this method
};

#endif // ENDGAMEMENU_H
