#include "EndGameMenu.h"
#include <iomanip>
#include <iostream>

// Konstruktor klasy EndGameMenu inicjalizujący menu końcowe
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

    if (!endGameMusic.openFromFile("end_game_music.ogg")) {
        std::cout << "Nie udało się wczytać muzyki końcowej" << std::endl;
    }

    background_sprite.setTexture(background_texture);

    // Ustawienie elementów tekstu i przycisków po lewej stronie
    float xPosition = 50; // Pozycja X dla tekstu i przycisków
    float initialYPosition = 100; // Początkowa pozycja Y

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
    survivalTimeText.setPosition(500, 200);

    killCountText.setFont(font);
    killCountText.setCharacterSize(24);
    killCountText.setFillColor(sf::Color::White);
    killCountText.setPosition(500, 250);

    updateSelection();
}

// Metoda ustawiająca wiadomość końcową
void EndGameMenu::setEndMessage(const std::string& message) {
    gameOverText.setFont(font); // Upewnij się, że czcionka jest ustawiona
    gameOverText.setString(message);
    gameOverText.setCharacterSize(30); // Ustaw odpowiedni rozmiar dla widoczności
    gameOverText.setFillColor(sf::Color::Yellow); // Ustaw kolor na żółty

    // Wyśrodkowanie tekstu na ekranie
    sf::FloatRect textRect = gameOverText.getLocalBounds();
    gameOverText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
    gameOverText.setPosition(sf::Vector2f(400, 100)); // Zakładając, że rozmiar okna to 800x600
}

// Metoda odtwarzająca dźwięk menu
void EndGameMenu::playSound() {
    sound.play();
}

// Metoda zatrzymująca dźwięk menu
void EndGameMenu::stopSound() {
    sound.stop();
}

// Metoda odtwarzająca muzykę końcową
void EndGameMenu::playMusic() {
    endGameMusic.setLoop(true);
    endGameMusic.play();
}

// Metoda zatrzymująca muzykę końcową
void EndGameMenu::stopMusic() {
    endGameMusic.stop();
}

// Metoda rysująca elementy menu na oknie
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

// Metoda przesuwająca wybór w menu do góry
void EndGameMenu::moveSelectionUp() {
    selectedBackToMenu = true;
    updateSelection();
}

// Metoda przesuwająca wybór w menu do dołu
void EndGameMenu::moveSelectionDown() {
    selectedBackToMenu = false;
    updateSelection();
}

// Metoda sprawdzająca, czy wybrano powrót do menu
bool EndGameMenu::isBackToMenuSelected() const {
    return selectedBackToMenu;
}

// Metoda sprawdzająca, czy mysz znajduje się nad przyciskiem
bool EndGameMenu::isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePosition) const {
    return button.getGlobalBounds().contains(sf::Vector2f(mousePosition));
}

// Metoda aktualizująca stan menu na podstawie pozycji myszy
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

// Metoda aktualizująca wybór w menu
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

// Metoda aktualizująca statystyki końcowe (czas przetrwania i liczba zabójstw)
void EndGameMenu::updateStats(float survivalTime, int killCount) {
    std::stringstream ss;
    ss << "Time: " << std::fixed << std::setprecision(1) << survivalTime << " seconds";
    survivalTimeText.setString(ss.str());

    ss.str("");
    ss << "Kills: " << killCount;
    killCountText.setString(ss.str());
}

// Metoda ustawiająca etykietę czasu
void EndGameMenu::setTimeLabel(const std::string& label) {
    std::string currentText = survivalTimeText.getString();
    std::string newText = label + currentText.substr(currentText.find(":"));
    survivalTimeText.setString(newText);
}

// Metoda wyświetlająca menu końcowe gry
void EndGameMenu::showEndGameMenu(bool playerWon, float survivalTime, int killCount, sf::RenderWindow& window, bool& gameStarted, bool& gameEnded, bool& survivalMode, sf::Music& gameMusic, sf::Clock& survivalClock, Menu& menu, std::function<void()> resetGame, std::function<void(int)> startRound) {
    if (!survivalMode) {
        if (playerWon) {
            setEndMessage("Congratulations! You Won!");
        } else {
            setEndMessage("");
        }
    } else {
        setEndMessage(""); // W trybie przetrwania możesz dostosować tę część
    }

    updateStats(survivalTime, killCount);
    playSound();
    playMusic();

    // Obsługa zdarzeń i renderowania pętli
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
                        // Obsługa powrotu do menu
                        gameEnded = false;
                        gameStarted = false;
                        menu.playSound();
                        resetGame(); // Resetowanie stanu gry
                        stopMusic();  // Zatrzymanie muzyki końcowej
                        stopSound();
                        return;
                    } else if (isMouseOverButton(playAgainButton, sf::Mouse::getPosition(window))) {
                        // Obsługa ponownego grania
                        resetGame(); // Resetowanie stanu gry
                        gameStarted = true;
                        gameMusic.setLoop(true);
                        gameMusic.play();
                        stopMusic();  // Zatrzymanie muzyki końcowej
                        stopSound();
                        if (survivalMode) {
                            // Restart trybu przetrwania
                            survivalMode = true;
                            survivalClock.restart();
                        } else {
                            // Restart trybu etapowego
                            survivalMode = false; // Upewnienie się, że rozpoczynamy w trybie etapowym
                            startRound(1); // Rozpoczęcie pierwszej rundy
                        }
                        return;
                    } else if (isMouseOverButton(exitButton, sf::Mouse::getPosition(window))) {
                        stopMusic();
                        stopSound();
                        window.close(); // Zakończenie gry
                    }
                }
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (isMouseOverButton(backToMenuButton, mousePos)) {
                        // Obsługa powrotu do menu
                        gameEnded = false;
                        gameStarted = false;
                        menu.playSound();
                        resetGame(); // Resetowanie stanu gry
                        stopMusic();  // Zatrzymanie muzyki końcowej
                        stopSound();
                        return;
                    } else if (isMouseOverButton(playAgainButton, mousePos)) {
                        stopSound();
                        stopMusic();  // Zatrzymanie muzyki końcowej
                        resetGame(); // Resetowanie stanu gry
                        gameStarted = true;
                        gameMusic.setLoop(true);
                        gameMusic.play();
                        if (survivalMode) {
                            // Restart trybu przetrwania
                            survivalMode = true;
                            survivalClock.restart();
                        } else {
                            // Restart trybu etapowego
                            survivalMode = false; // Upewnienie się, że rozpoczynamy w trybie etapowym
                            startRound(1); // Rozpoczęcie pierwszej rundy
                        }
                        return;
                    } else if (isMouseOverButton(exitButton, mousePos)) {
                        stopMusic();
                        stopSound();
                        window.close(); // Zakończenie gry
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

