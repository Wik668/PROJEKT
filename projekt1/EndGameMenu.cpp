#include "EndGameMenu.h"

EndGameMenu::EndGameMenu() : selectedBackToMenu(true) {
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

    gameOverText.setFont(font);
    gameOverText.setString("Game Over");
    gameOverText.setCharacterSize(48);
    gameOverText.setFillColor(sf::Color::Red);
    gameOverText.setPosition(250, 100);

    backToMenuText.setFont(font);
    backToMenuText.setString("Back to Menu");
    backToMenuText.setCharacterSize(24);
    backToMenuText.setFillColor(sf::Color::White);
    backToMenuText.setPosition(250, 200);

    playAgainText.setFont(font);
    playAgainText.setString("Play Again");
    playAgainText.setCharacterSize(24);
    playAgainText.setFillColor(sf::Color::White);
    playAgainText.setPosition(250, 250);

    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(250, 300);

    backToMenuButton.setSize(sf::Vector2f(200, 50));
    backToMenuButton.setPosition(240, 195);
    backToMenuButton.setFillColor(sf::Color::Transparent);
    backToMenuButton.setOutlineThickness(2);
    backToMenuButton.setOutlineColor(sf::Color::White);

    playAgainButton.setSize(sf::Vector2f(200, 50));
    playAgainButton.setPosition(240, 245);
    playAgainButton.setFillColor(sf::Color::Transparent);
    playAgainButton.setOutlineThickness(2);
    playAgainButton.setOutlineColor(sf::Color::White);

    exitButton.setSize(sf::Vector2f(200, 50));
    exitButton.setPosition(240, 295);
    exitButton.setFillColor(sf::Color::Transparent);
    exitButton.setOutlineThickness(2);
    exitButton.setOutlineColor(sf::Color::White);

    updateSelection();
}

void EndGameMenu::playSound() {
    sound.play();
}

void EndGameMenu::stopSound() {
    sound.stop();
}

void EndGameMenu::draw(sf::RenderWindow& window) {
    window.draw(background_sprite);
    window.draw(gameOverText);
    window.draw(backToMenuText);
    window.draw(playAgainText);
    window.draw(exitText);
    window.draw(backToMenuButton);
    window.draw(playAgainButton);
    window.draw(exitButton);
}

void EndGameMenu::moveSelectionUp() {
    selectedBackToMenu = true;
    updateSelection();
}

void EndGameMenu::moveSelectionDown() {
    selectedBackToMenu = false;
    updateSelection();
}

bool EndGameMenu::isBackToMenuSelected() const {
    return selectedBackToMenu;
}

bool EndGameMenu::isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePosition) const {
    return button.getGlobalBounds().contains(sf::Vector2f(mousePosition));
}

void EndGameMenu::update(const sf::Vector2i& mousePosition) {
    if (isMouseOverButton(backToMenuButton, mousePosition)) {
        backToMenuText.setFillColor(sf::Color::Red);
    } else {
        backToMenuText.setFillColor(sf::Color::White);
    }

    if (isMouseOverButton(playAgainButton, mousePosition)) {
        playAgainText.setFillColor(sf::Color::Red);
    } else {
        playAgainText.setFillColor(sf::Color::White);
    }

    if (isMouseOverButton(exitButton, mousePosition)) {
        exitText.setFillColor(sf::Color::Red);
    } else {
        exitText.setFillColor(sf::Color::White);
    }
}

void EndGameMenu::updateSelection() {
    if (selectedBackToMenu) {
        backToMenuText.setFillColor(sf::Color::Red);
        playAgainText.setFillColor(sf::Color::White);
        exitText.setFillColor(sf::Color::White);
    } else {
        backToMenuText.setFillColor(sf::Color::White);
        playAgainText.setFillColor(sf::Color::Red);
        exitText.setFillColor(sf::Color::White);
    }
}
