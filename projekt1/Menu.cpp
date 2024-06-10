#include "Menu.h"
#include <iostream>

// Konstruktor klasy Menu inicjalizujący menu gry
Menu::Menu() : selectedSurvival(true) {
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Nie udało się wczytać czcionki" << std::endl;
    }

    if (!background_texture.loadFromFile("menu.png")) {
        std::cout << "Nie udało się wczytać tekstury tła menu" << std::endl;
    }

    if (!buffer.loadFromFile("menu_music.wav")) {
        std::cout << "Nie udało się wczytać dźwięku menu" << std::endl;
    }

    sound.setBuffer(buffer);
    sound.setLoop(true);

    background_sprite.setTexture(background_texture);

    text.setFont(font);
    text.setString("Select game");
    text.setCharacterSize(30);
    text.setFillColor(sf::Color::Yellow);
    text.setPosition(300, 200);

    survivalText.setFont(font);
    survivalText.setString("Survival Mode");
    survivalText.setCharacterSize(24);
    survivalText.setFillColor(sf::Color::White);
    survivalText.setPosition(300, 260);

    stageText.setFont(font);
    stageText.setString("Stage Mode");
    stageText.setCharacterSize(24);
    stageText.setFillColor(sf::Color::White);
    stageText.setPosition(300, 310);

    survivalButton.setSize(sf::Vector2f(200, 50));
    survivalButton.setPosition(290, 255);
    survivalButton.setFillColor(sf::Color::Transparent);
    survivalButton.setOutlineThickness(2);
    survivalButton.setOutlineColor(sf::Color::White);

    stageButton.setSize(sf::Vector2f(200, 50));
    stageButton.setPosition(290, 305);
    stageButton.setFillColor(sf::Color::Transparent);
    stageButton.setOutlineThickness(2);
    stageButton.setOutlineColor(sf::Color::White);

    updateSelection();
}

// Metoda odtwarzająca dźwięk menu
void Menu::playSound() {
    sound.play();
}

// Metoda zatrzymująca dźwięk menu
void Menu::stopSound() {
    sound.stop();
}

// Metoda rysująca elementy menu na oknie
void Menu::draw(sf::RenderWindow& window) {
    window.draw(background_sprite);
    window.draw(text);
    window.draw(survivalText);
    window.draw(stageText);
    window.draw(survivalButton);
    window.draw(stageButton);
}

// Metoda przesuwająca wybór w menu do góry
void Menu::moveSelectionUp() {
    selectedSurvival = true;
    updateSelection();
}

// Metoda przesuwająca wybór w menu do dołu
void Menu::moveSelectionDown() {
    selectedSurvival = false;
    updateSelection();
}

// Metoda sprawdzająca, czy wybrano tryb przetrwania
bool Menu::isSurvivalSelected() const {
    return selectedSurvival;
}

// Metoda sprawdzająca, czy mysz znajduje się nad przyciskiem
bool Menu::isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePosition) const {
    return button.getGlobalBounds().contains(sf::Vector2f(mousePosition));
}

// Metoda aktualizująca stan menu na podstawie pozycji myszy
void Menu::update(const sf::Vector2i& mousePosition) {
    if (isMouseOverButton(survivalButton, mousePosition)) {
        survivalText.setFillColor(sf::Color::Red);
    } else {
        survivalText.setFillColor(sf::Color::White);
    }

    if (isMouseOverButton(stageButton, mousePosition)) {
        stageText.setFillColor(sf::Color::Red);
    } else {
        stageText.setFillColor(sf::Color::White);
    }
}

// Metoda aktualizująca wybór w menu
void Menu::updateSelection() {
    if (selectedSurvival) {
        survivalText.setFillColor(sf::Color::Red);
        stageText.setFillColor(sf::Color::White);
    } else {
        survivalText.setFillColor(sf::Color::White);
        stageText.setFillColor(sf::Color::Red);
    }
}
