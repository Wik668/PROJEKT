#include "EndGameMenu.h"
#include <iomanip>

EndGameMenu::EndGameMenu() : selectedBackToMenu(true) {
    if (!font.loadFromFile("arial.ttf")) {
        std::cout << "Nie udało się wczytać czcionki" << std::endl;
    }

    if (!background_texture.loadFromFile("endgame.png")) {
        std::cout << "Nie udało się wczytać tekstury tła menu" << std::endl;
    }

    if (!buffer.loadFromFile("menu_music.wav")) {
        std::cout << "Nie udało się wczytać dźwięku menu" << std::endl;
    } else {
        sound.setBuffer(buffer);
        sound.setLoop(true);
    }

    background_sprite.setTexture(background_texture);

    // Ustawienie elementów tekstu i przycisków po lewej stronie
    float xPosition = 50; // Pozycja X dla tekstu i przycisków
    float initialYPosition = 100; // Początkowa pozycja Y
    float buttonYOffset = 100; // Odstęp Y dla przycisków

    backToMenuText.setFont(font);
    backToMenuText.setString("Back to Menu");
    backToMenuText.setCharacterSize(24);
    backToMenuText.setFillColor(sf::Color::White);
    backToMenuText.setPosition(xPosition, initialYPosition + 50);

    playAgainText.setFont(font);
    playAgainText.setString("Play Again");
    playAgainText.setCharacterSize(24);
    playAgainText.setFillColor(sf::Color::White);
    playAgainText.setPosition(xPosition, initialYPosition + 100);

    exitText.setFont(font);
    exitText.setString("Exit");
    exitText.setCharacterSize(24);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(xPosition, initialYPosition + 150);

    backToMenuButton.setSize(sf::Vector2f(200, 50));
    backToMenuButton.setPosition(xPosition, initialYPosition + 45);
    backToMenuButton.setFillColor(sf::Color::Transparent);
    backToMenuButton.setOutlineThickness(2);
    backToMenuButton.setOutlineColor(sf::Color::White);

    playAgainButton.setSize(sf::Vector2f(200, 50));
    playAgainButton.setPosition(xPosition, initialYPosition + 95);
    playAgainButton.setFillColor(sf::Color::Transparent);
    playAgainButton.setOutlineThickness(2);
    playAgainButton.setOutlineColor(sf::Color::White);

    exitButton.setSize(sf::Vector2f(200, 50));
    exitButton.setPosition(xPosition, initialYPosition + 145);
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

void EndGameMenu::setEndMessage(const std::string& message) {
    gameOverText.setFont(font); // Ensure the font is set
    gameOverText.setString(message);
    gameOverText.setCharacterSize(30); // Set a suitable size for visibility
    gameOverText.setFillColor(sf::Color::Yellow); // Set color to white

    // Center the text on the screen
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    gameOverText.setPosition(sf::Vector2f(400, 100)); // Assuming window size is 800x600 for example
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

void EndGameMenu::setTimeLabel(const std::string& label) {
    std::string currentText = survivalTimeText.getString();
    std::string newText = label + currentText.substr(currentText.find(":"));
    survivalTimeText.setString(newText);
}

void EndGameMenu::showEndGameMenu(bool playerWon, float survivalTime, int killCount, sf::RenderWindow& window, bool& gameStarted, bool& gameEnded, bool& survivalMode, sf::Music& gameMusic, sf::Clock& survivalClock, Menu& menu, std::function<void()> resetGame, std::function<void(int)> startRound) {
    if (!survivalMode) {
        if (playerWon) {
            setEndMessage("Congratulations! You Won!");
        } else {
            setEndMessage("");
        }
    } else {
        setEndMessage(""); // In survival mode, perhaps you want to display nothing or customize this part
    }

    updateStats(survivalTime, killCount);
    playSound();
    playMusic();

    // Handle event and rendering loop already correctly provided

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
                        stopSound();
                        return;
                    } else if (isMouseOverButton(playAgainButton, sf::Mouse::getPosition(window))) {
                        // Handle playing again
                        resetGame(); // Reset the game state
                        gameStarted = true;
                        gameMusic.setLoop(true);
                        gameMusic.play();
                        stopMusic();  // Stop the end game music
                        stopSound();
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
                        stopMusic();
                        stopSound();
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
                        stopSound();
                        return;
                    } else if (isMouseOverButton(playAgainButton, mousePos)) {
                        stopSound();
                        stopMusic();  // Zatrzymaj muzykę końcowej gry
                        resetGame(); // Resetuj stan gry
                        gameStarted = true;
                        gameMusic.setLoop(true);
                        gameMusic.play();
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
                        stopMusic();
                        stopSound();
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
