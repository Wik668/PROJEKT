#include "EndGameMenu.h"
#include <iomanip>

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

    if (!endGameMusic.openFromFile("Fisticuffs and Ale.ogg")) {
        std::cout << "Nie udało się wczytać muzyki końcowej gry" << std::endl;
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

    survivalTimeText.setFont(font);
    survivalTimeText.setCharacterSize(24);
    survivalTimeText.setFillColor(sf::Color::White);
    survivalTimeText.setPosition(500, 200); // Adjust position as needed

    killCountText.setFont(font);
    killCountText.setCharacterSize(24);
    killCountText.setFillColor(sf::Color::White);
    killCountText.setPosition(500, 250); // Adjust position as needed

    updateSelection();
}

void EndGameMenu::playSound() {
    sound.play();
}

void EndGameMenu::stopSound() {
    sound.stop();
}

void EndGameMenu::playMusic() {
    std::cout << "Odtwarzanie muzyki końcowej gry." << std::endl;
    endGameMusic.setLoop(true);
    endGameMusic.play();
}

void EndGameMenu::stopMusic() {
    endGameMusic.stop();
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
    window.draw(survivalTimeText);
    window.draw(killCountText);
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

void EndGameMenu::updateStats(float survivalTime, int killCount) {
    std::stringstream ss;
    ss << "Time: " << std::fixed << std::setprecision(1) << survivalTime << " seconds";
    survivalTimeText.setString(ss.str());

    ss.str("");
    ss << "Kills: " << killCount;
    killCountText.setString(ss.str());
}

void EndGameMenu::setEndMessage(const std::string& message) {
    gameOverText.setString(message);
    gameOverText.setPosition(250, 100); // Adjust position as needed
}

void EndGameMenu::setTimeLabel(const std::string& label) {
    std::string currentText = survivalTimeText.getString();
    std::string newText = label + currentText.substr(currentText.find(":"));
    survivalTimeText.setString(newText);
}

void EndGameMenu::showEndGameMenu(bool playerWon, float survivalTime, int killCount, sf::RenderWindow& window, bool& gameStarted, bool& gameEnded, bool& survivalMode, sf::Music& gameMusic, sf::Clock& survivalClock, Menu& menu, std::function<void()> resetGame, std::function<void(int)> startRound) {
    if (playerWon) {
        setEndMessage("Congratulations");
    } else {
        setEndMessage("Game Over");
    }
    updateStats(survivalTime, killCount);
    playSound();
    playMusic();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Up) {
                    moveSelectionUp();
                } else if (event.key.code == sf::Keyboard::Down) {
                    moveSelectionDown();
                } else if (event.key.code == sf::Keyboard::Enter) {
                    if (isBackToMenuSelected()) {
                        // Handle going back to the menu
                        gameEnded = false;
                        gameStarted = false;
                        menu.playSound();
                        resetGame(); // Reset the game state
                        stopMusic();  // Stop the end game music
                        return;
                    } else if (isMouseOverButton(playAgainButton, sf::Mouse::getPosition(window))) {
                        // Handle playing again
                        resetGame(); // Reset the game state
                        gameStarted = true;
                        gameMusic.setLoop(true);
                        gameMusic.play();
                        stopMusic();  // Stop the end game music
                        if (survivalMode) {
                            // Restart survival mode
                            survivalMode = true;
                            survivalClock.restart();
                        } else {
                            // Restart stage mode
                            survivalMode = false; // Ensure we start in stage mode
                            startRound(1); // Start the first round
                        }
                        return;
                    } else if (isMouseOverButton(exitButton, sf::Mouse::getPosition(window))) {
                        window.close(); // Exit the game
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (isMouseOverButton(backToMenuButton, mousePos)) {
                        // Handle going back to the menu
                        gameEnded = false;
                        gameStarted = false;
                        menu.playSound();
                        resetGame(); // Reset the game state
                        stopMusic();  // Stop the end game music
                        return;
                    } else if (isMouseOverButton(playAgainButton, mousePos)) {
                        // Handle playing again
                        resetGame(); // Reset the game state
                        gameStarted = true;
                        gameMusic.setLoop(true);
                        gameMusic.play();
                        stopMusic();  // Stop the end game music
                        if (survivalMode) {
                            // Restart survival mode
                            survivalMode = true;
                            survivalClock.restart();
                        } else {
                            // Restart stage mode
                            survivalMode = false; // Ensure we start in stage mode
                            startRound(1); // Start the first round
                        }
                        return;
                    } else if (isMouseOverButton(exitButton, mousePos)) {
                        window.close(); // Exit the game
                    }
                }
            }
        }

        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        update(mousePosition);

        window.clear();
        draw(window);
        window.display();
    }

    stopSound();
}
