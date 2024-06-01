#ifndef MENU_H
#define MENU_H


#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

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

    void updateSelection();

public:
    sf::RectangleShape survivalButton;
    sf::RectangleShape stageButton;

    Menu();
    void playSound();
    void stopSound();
    void draw(sf::RenderWindow& window);
    void moveSelectionUp();
    void moveSelectionDown();
    bool isSurvivalSelected() const;
    bool isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePosition) const;
    void update(const sf::Vector2i& mousePosition);
};



#endif // MENU_H
