    #include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <sstream>
#include <iomanip>
#include <SFML/System/Vector2.hpp> // Include the necessary header
#include "Zombie.h" // Include the Zombie header
#include "Bullet.h"

#include "utils.h"
using namespace std;
using namespace sf;





class AnimatedSprite : public Sprite {
private:
    vector<IntRect> framesRight;
    vector<IntRect> framesLeft;
    vector<IntRect> framesUp;
    vector<IntRect> framesDown;
    int currentFrame;
    int animationFps;
    Time frameTime;
    Clock clock;
    enum Direction { Up, Down, Left, Right } direction;

public:
    AnimatedSprite(int fps) : currentFrame(0), animationFps(fps), direction(Right) {}

    void add_animation_frame_right(const IntRect& frame) {
        framesRight.push_back(frame);
    }

    void add_animation_frame_left(const IntRect& frame) {
        framesLeft.push_back(frame);
    }

    void add_animation_frame_up(const IntRect& frame) {
        framesUp.push_back(frame);
    }

    void add_animation_frame_down(const IntRect& frame) {
        framesDown.push_back(frame);
    }

    void add_standing_frame_right(const IntRect& frame) {
        framesRight.push_back(frame);
    }

    void add_standing_frame_left(const IntRect& frame) {
        framesLeft.push_back(frame);
    }

    void add_standing_frame_up(const IntRect& frame) {
        framesUp.push_back(frame);
    }

    void add_standing_frame_down(const IntRect& frame) {
        framesDown.push_back(frame);
    }
    //naprawione warunki animacji od kiedy przeszliśmy na WSAD wcześniej były strzałki
    void step() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::D) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::W) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
            frameTime += clock.restart();
            Time timePerFrame = seconds(1.0f / animationFps);

            while (frameTime >= timePerFrame) {
                frameTime -= timePerFrame;
                currentFrame = (currentFrame + 1) % (getFrames().size() - 1); // Skip standing frame
                setTextureRect(getFrames()[currentFrame]);
            }
        } else {
            setTextureRect(getFrames().back()); // Set the last frame as standing frame
        }
    }

    void moveWithCollision(const FloatRect& bounds, float offsetX, float offsetY) {
        Vector2f oldPosition = getPosition();
        Sprite::move(offsetX, offsetY);
        FloatRect spriteBounds = getGlobalBounds();

        if (spriteBounds.left < bounds.left ||
            spriteBounds.top < bounds.top ||
            spriteBounds.left + spriteBounds.width > bounds.left + bounds.width ||
            spriteBounds.top + spriteBounds.height > bounds.top + bounds.height) {
            setPosition(oldPosition);
        } else {
            if (offsetX > 0) direction = Right;
            else if (offsetX < 0) direction = Left;
            else if (offsetY > 0) direction = Down;
            else if (offsetY < 0) direction = Up;
        }
    }

private:
    const vector<IntRect>& getFrames() const {
        switch (direction) {
        case Right: return framesRight;
        case Left: return framesLeft;
        case Up: return framesUp;
        case Down: return framesDown;
        }
        // To avoid warning, returning framesRight as a default case.
        return framesRight;
    }
};
class Button {
private:
    sf::RectangleShape shape;
    sf::Text text;
    bool isSelected;

public:
    Button(const sf::Vector2f& size, const sf::Font& font, const std::string& textString, const sf::Vector2f& position)
        : isSelected(false) {
        shape.setSize(size);
        shape.setFillColor(sf::Color::White);
        shape.setPosition(position);

        text.setFont(font);
        text.setString(textString);
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::Black);
        text.setPosition(
            position.x + (size.x - text.getGlobalBounds().width) / 2,
            position.y + (size.y - text.getGlobalBounds().height) / 2
            );
    }

    void draw(sf::RenderWindow& window) {
        window.draw(shape);
        window.draw(text);
    }

    void setSelected(bool selected) {
        isSelected = selected;
        shape.setFillColor(selected ? sf::Color::Red : sf::Color::White);
        text.setFillColor(selected ? sf::Color::White : sf::Color::Black);
    }

    bool contains(const sf::Vector2f& point) const {
        return shape.getGlobalBounds().contains(point);
    }

    bool isSelectedButton() const {
        return isSelected;
    }

    std::string getText() const {
        return text.getString();
    }
};

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

public:
    sf::RectangleShape survivalButton;
    sf::RectangleShape stageButton;

public:
    Menu() : selectedSurvival(true) {
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
        text.setString("Press ENTER to start game");
        text.setCharacterSize(24);
        text.setFillColor(sf::Color::White);
        text.setPosition(250, 300);

        survivalText.setFont(font);
        survivalText.setString("Survival Mode");
        survivalText.setCharacterSize(24);
        survivalText.setFillColor(sf::Color::White);
        survivalText.setPosition(250, 350);

        stageText.setFont(font);
        stageText.setString("Stage Mode");
        stageText.setCharacterSize(24);
        stageText.setFillColor(sf::Color::White);
        stageText.setPosition(250, 400);

        survivalButton.setSize(sf::Vector2f(200, 50));
        survivalButton.setPosition(240, 345);
        survivalButton.setFillColor(sf::Color::Transparent);
        survivalButton.setOutlineThickness(2);
        survivalButton.setOutlineColor(sf::Color::White);

        stageButton.setSize(sf::Vector2f(200, 50));
        stageButton.setPosition(240, 395);
        stageButton.setFillColor(sf::Color::Transparent);
        stageButton.setOutlineThickness(2);
        stageButton.setOutlineColor(sf::Color::White);

        updateSelection();
    }

    void playSound() {
        sound.play();
    }

    void stopSound() {
        sound.stop();
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background_sprite);
        window.draw(text);
        window.draw(survivalText);
        window.draw(stageText);
        window.draw(survivalButton);
        window.draw(stageButton);
    }

    void moveSelectionUp() {
        selectedSurvival = true;
        updateSelection();
    }

    void moveSelectionDown() {
        selectedSurvival = false;
        updateSelection();
    }

    bool isSurvivalSelected() const {
        return selectedSurvival;
    }

    bool isMouseOverButton(const sf::RectangleShape& button, const sf::Vector2i& mousePosition) const {
        return button.getGlobalBounds().contains(sf::Vector2f(mousePosition));
    }

    void update(const sf::Vector2i& mousePosition) {
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

private:
    void updateSelection() {
        if (selectedSurvival) {
            survivalText.setFillColor(sf::Color::Red);
            stageText.setFillColor(sf::Color::White);
        } else {
            survivalText.setFillColor(sf::Color::White);
            stageText.setFillColor(sf::Color::Red);
        }
    }
};


class Game {
    // Other class members
    vector<Zombie> zombies;
    sf::Texture zombie_texture;
private:
    float health;  // Add this line for the health attribute
    sf::Text healthText;
    int window_width = 800;
    int window_height = 600;
    sf::RenderWindow window;
    AnimatedSprite hero;
    sf::Texture character_texture;
    sf::Texture background_texture;
    sf::Sprite background_sprite;
    sf::Music gameMusic;//dodane do muzyki
    sf::SoundBuffer buffer;
    sf::Sound sound;
    Menu menu;
    bool gameStarted;
    bool survivalMode;
    bool gameEnded;    // New variable to store the selected game mode
    float move_speed;
    sf::Clock survivalClock; // Clock for the Survival Mode timer
    sf::Text timerText;
    sf::Font font;
    sf::Texture bullet_texture;
    std::vector<Bullet> bullets;
    float bullet_speed;
    sf::Text killCounterText; // Add this line
    int zombiesKilled;
    sf::Texture survivalButtonTexture;
    sf::Texture survivalButtonTextureRed; // For highlighted version
    sf::Texture stageButtonTexture;
    sf::Texture stageButtonTextureRed;
    // Other class members




    sf::Text endGameText; // Text for the end game message


    void loadResources() {
        if (!background_texture.loadFromFile("puste.png")) {
            cout << "Nie udało się wczytać tekstury tła" << endl;
            exit(1);
        }
        if (!character_texture.loadFromFile("Walk.png")) {
            cout << "Nie udało się wczytać tekstury bohatera" << endl;
            exit(1);
        }
        if (!zombie_texture.loadFromFile("Walk_Zombie.png")) {
            cout << "Nie udało się wczytać tekstury zombie" << endl;
            exit(1);
        }
        if (!buffer.loadFromFile("menu_music.wav")) {
            std::cout << "Nie udało się wczytać dźwięku menu" << std::endl;
        }
        if (!bullet_texture.loadFromFile("bullet.png")) {
            std::cout << "Nie udało się wczytać tekstury pocisku" << std::endl;
            exit(1);
        }


        sound.setBuffer(buffer);
        sound.setLoop(true); // Set the sound to loop
        // Dodaj ten kod //dodany do muzyki
        if (!gameMusic.openFromFile("blanka.wav")) { // Zmień ścieżkę na rzeczywistą ścieżkę do pliku muzycznego
            std::cout << "Nie udało się wczytać muzyki" << std::endl;
            exit(1);
        }

        // No need to scale the background sprite anymore
        background_sprite.setTexture(background_texture);
    }


    void playSound() {
        sound.play();
    }

    void stopSound() {
        sound.stop();
    }
    void initializeHero() {
        hero.setTexture(character_texture);
        hero.add_animation_frame_right(IntRect(9, 70, 14, 25));
        hero.add_animation_frame_right(IntRect(41, 70, 14, 25));
        hero.add_animation_frame_right(IntRect(73, 70, 14, 25));
        hero.add_animation_frame_right(IntRect(105, 70, 14, 25));
        hero.add_standing_frame_right(IntRect(134, 70, 14, 25));

        hero.add_animation_frame_left(IntRect(10, 102, 14, 25));
        hero.add_animation_frame_left(IntRect(41, 102, 14, 25));
        hero.add_animation_frame_left(IntRect(73, 102, 14, 25));
        hero.add_animation_frame_left(IntRect(105, 102, 14, 25));
        hero.add_standing_frame_left(IntRect(134, 102, 14, 25));

        hero.add_animation_frame_up(IntRect(10, 38, 14, 25));
        hero.add_animation_frame_up(IntRect(41, 38, 14, 25));
        hero.add_animation_frame_up(IntRect(73, 38, 14, 25));
        hero.add_animation_frame_up(IntRect(105, 38, 14, 25));
        hero.add_standing_frame_up(IntRect(134, 38, 14, 25));

        hero.add_animation_frame_down(IntRect(9, 6, 14, 25));
        hero.add_animation_frame_down(IntRect(41, 6, 14, 25));
        hero.add_animation_frame_down(IntRect(73, 6, 14, 25));
        hero.add_animation_frame_down(IntRect(105, 6, 14, 25));
        hero.add_standing_frame_down(IntRect(134, 6, 14, 25));

        hero.setScale(2,2);
        hero.setPosition(0, 0);

    }
    void initializeZombies() {
        for (int i = 0; i < 5; ++i) {
            createZombie(i * 100, 200);  // Example positions
        }
    }

    void createZombie(float x, float y) {
        Zombie zombie(5);
        zombie.setTexture(zombie_texture);

        zombie.add_animation_frame_right(IntRect(11, 69, 14, 26));
        zombie.add_animation_frame_right(IntRect(42, 69, 14, 26));
        zombie.add_standing_frame_right(IntRect(74, 69, 14, 26));
        zombie.add_animation_frame_right(IntRect(107, 69, 14, 26));
        zombie.add_animation_frame_right(IntRect(140, 69, 14, 26));
        zombie.add_animation_frame_right(IntRect(173, 69, 14, 26));
        zombie.add_animation_frame_right(IntRect(204, 69, 14, 26));
        zombie.add_standing_frame_right(IntRect(236, 69, 14, 26));
        zombie.add_animation_frame_right(IntRect(269, 69, 14, 26));
        zombie.add_animation_frame_right(IntRect(300, 69, 14, 26));

        zombie.add_standing_frame_left(IntRect(9, 101, 12, 26));
        zombie.add_animation_frame_left(IntRect(41, 101, 12, 26));
        zombie.add_animation_frame_left(IntRect(72, 101, 12, 26));
        zombie.add_standing_frame_left(IntRect(104, 101, 12, 26));
        zombie.add_animation_frame_left(IntRect(136, 101, 12, 26));
        zombie.add_animation_frame_left(IntRect(166, 101, 12, 26));
        zombie.add_animation_frame_left(IntRect(199, 101, 12, 26));
        zombie.add_standing_frame_left(IntRect(230, 101, 12, 26));
        zombie.add_animation_frame_left(IntRect(262, 101, 12, 26));
        zombie.add_animation_frame_left(IntRect(296, 101, 12, 26));

        zombie.add_standing_frame_up(IntRect(10, 0, 12, 26));
        zombie.add_animation_frame_up(IntRect(42, 0, 12, 26));
        zombie.add_animation_frame_up(IntRect(74, 0, 12, 26));
        zombie.add_standing_frame_up(IntRect(106, 0, 12, 26));
        zombie.add_animation_frame_up(IntRect(137, 0, 12, 26));
        zombie.add_animation_frame_up(IntRect(168, 0, 12, 26));
        zombie.add_animation_frame_up(IntRect(200, 0, 12, 26));
        zombie.add_standing_frame_up(IntRect(231, 0, 12, 26));
        zombie.add_animation_frame_up(IntRect(264, 0, 12, 26));
        zombie.add_animation_frame_up(IntRect(297, 0, 12, 26));

        zombie.add_animation_frame_down(IntRect(10, 37, 12, 26));
        zombie.add_animation_frame_down(IntRect(42, 37, 12, 26));
        zombie.add_standing_frame_down(IntRect(74, 37, 12, 26));
        zombie.add_animation_frame_down(IntRect(106, 37, 12, 26));
        zombie.add_animation_frame_down(IntRect(137, 37, 12, 26));
        zombie.add_animation_frame_down(IntRect(168, 37, 12, 26));
        zombie.add_animation_frame_down(IntRect(200, 37, 12, 26));
        zombie.add_standing_frame_down(IntRect(231, 37, 12, 26));
        zombie.add_animation_frame_down(IntRect(264, 37, 12, 26));
        zombie.add_animation_frame_down(IntRect(297, 37, 12, 26));

        zombie.setScale(2, 2);
        zombie.setPosition(x, y);
        zombies.push_back(zombie);
    }



public:
    Game()
        : health(100), window(sf::VideoMode(window_width, window_height), "Game Window"),
        hero(5), gameStarted(false), gameEnded(false), move_speed(0.1f), bullet_speed(0.5f), zombiesKilled(0) {
        loadResources();
        initializeHero();
        initializeZombies();
        background_sprite.setTexture(background_texture);

        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Nie udało się wczytać czcionki" << std::endl;
            exit(1);
        }
        timerText.setFont(font);
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color::White);
        timerText.setPosition(10, 10);

        healthText.setFont(font);
        healthText.setCharacterSize(24);
        healthText.setFillColor(sf::Color::White);
        healthText.setPosition(10, window_height - 30);
        updateHealthText();

        endGameText.setFont(font);
        endGameText.setCharacterSize(48);
        endGameText.setFillColor(sf::Color::Red);
        endGameText.setString("Game Over");
        endGameText.setPosition(window_width / 2 - endGameText.getGlobalBounds().width / 2, window_height / 2 - endGameText.getGlobalBounds().height / 2);

        killCounterText.setFont(font);
        killCounterText.setCharacterSize(24);
        killCounterText.setFillColor(sf::Color::White);
        killCounterText.setPosition(window_width - 150, 10);
        updateKillCounterText();


    }


    void updateHealthText() {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(1) << health;
        healthText.setString("HP: " + ss.str());
    }


    float damage = 0.01;

    void checkHeroZombieCollisions() {
        for (auto& zombie : zombies) {
            if (checkCollision(hero, zombie)) {
                health -= damage;
                updateHealthText();
                if (health <= 0) {
                    gameEnded = true;
                    gameMusic.stop();
                }
            }
        }
    }

    void run() {
        menu.playSound();
        while (window.isOpen()) {

            handleEvents();
            update();
            render();
        }

        gameMusic.stop();
    }

    void shootBullet() {
        sf::Vector2f heroCenter = hero.getPosition() + sf::Vector2f(hero.getGlobalBounds().width / 2, hero.getGlobalBounds().height / 2);
        sf::Vector2i mousePosition = sf::Mouse::getPosition(window);
        sf::Vector2f target = window.mapPixelToCoords(mousePosition);

        sf::Vector2f direction = normalize(target - heroCenter);

        // Obliczanie kąta w stopniach
        float angle = std::atan2(direction.y, direction.x) * 180 / 3.14159265;

        Bullet bullet(bullet_texture, direction, bullet_speed, angle);
        bullet.setPosition(heroCenter);

        bullets.push_back(bullet);
    }


    void checkBulletCollisions() {
        bool allZombiesKilled = true; // Flaga informująca, czy wszystkie zombie zostały zabite
        for (auto it = bullets.begin(); it != bullets.end();) {
            bool bulletErased = false;
            for (auto zombieIt = zombies.begin(); zombieIt != zombies.end(); ++zombieIt) {
                if (checkCollision(*it, *zombieIt)) {
                    it = bullets.erase(it);
                    zombieIt->takeDamage(50);
                    bulletErased = true;
                    if (zombieIt->getHealth() <= 0) {
                        zombieIt = zombies.erase(zombieIt);
                        zombiesKilled++;
                        updateKillCounterText();
                    } else {
                        ++zombieIt;
                    }
                    break;
                }
            }
            if (!bulletErased) {
                ++it;
            }
        }

        // Sprawdzamy, czy wszystkie zombie zostały zabite
        for (auto zombieIt = zombies.begin(); zombieIt != zombies.end(); ++zombieIt) {
            if (zombieIt->getHealth() > 0) {
                allZombiesKilled = false;
                break;
            }
        }

        // Jeśli wszystkie zombie zostały zabite, tworzymy nowe
        if (allZombiesKilled && zombies.empty()) {
            int newZombieCount = zombiesKilled * 2;
            for (int i = 0; i < newZombieCount; ++i) {
                createZombie(rand() % window_width, rand() % window_height);
            }
        }
    }
    //Koniec fragmentu respienia zombie przy zabijaniu



    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            } else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Escape) {
                    window.close();
                } else if (event.key.code == sf::Keyboard::Space) {
                    shootBullet();
                } else if (gameEnded && event.key.code == sf::Keyboard::Enter) {
                    window.close(); // Optionally restart the game or close the window
                }
            } else if (event.type == sf::Event::MouseMoved) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                updateButtonColors(mousePos);
            } else if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                    if (!gameStarted && !gameEnded) {
                        if (menu.isMouseOverButton(menu.survivalButton, mousePos)) {
                            gameStarted = true;
                            survivalMode = true;
                            menu.stopSound();
                            survivalClock.restart();
                            gameMusic.setLoop(true);
                            gameMusic.play();
                        } else if (menu.isMouseOverButton(menu.stageButton, mousePos)) {
                            gameStarted = true;
                            survivalMode = false;
                            menu.stopSound();
                            survivalClock.restart();
                            gameMusic.setLoop(true);
                            gameMusic.play();
                        }
                    }
                }
            }
        }
    }
    void updateButtonColors(const sf::Vector2i& mousePos) {
        if (menu.isMouseOverButton(menu.survivalButton, mousePos)) {
            menu.survivalButton.setTexture(&survivalButtonTextureRed); // Highlighted version
        } else {
            menu.survivalButton.setTexture(&survivalButtonTexture); // Default version
        }

        if (menu.isMouseOverButton(menu.stageButton, mousePos)) {
            menu.stageButton.setTexture(&stageButtonTextureRed); // Highlighted version
        } else {
            menu.stageButton.setTexture(&stageButtonTexture); // Default version
        }
    }





    void update() {
        if (gameStarted && !gameEnded) {
            FloatRect bounds(0, 0, window_width, window_height);
            if (Keyboard::isKeyPressed(Keyboard::A)) {
                hero.moveWithCollision(bounds, -move_speed, 0);
            } else if (Keyboard::isKeyPressed(Keyboard::D)) {
                hero.moveWithCollision(bounds, move_speed, 0);
            } else if (Keyboard::isKeyPressed(Keyboard::W)) {
                hero.moveWithCollision(bounds, 0, -move_speed);
            } else if (Keyboard::isKeyPressed(Keyboard::S)) {
                hero.moveWithCollision(bounds, 0, move_speed);
            }
            hero.step();

            Vector2f playerPosition = hero.getPosition();
            for (auto& zombie : zombies) {
                Vector2f zombiePosition = zombie.getPosition();
                Vector2f direction = playerPosition - zombiePosition;
                direction = normalize(direction);

                float moveX = direction.x * move_speed * 0.2;
                float moveY = direction.y * move_speed * 0.2;
                zombie.moveWithCollision(bounds, moveX, moveY);
                zombie.step();
            }

            for (size_t i = 0; i < zombies.size(); ++i) {
                for (size_t j = i + 1; j < zombies.size(); ++j) {
                    if (checkCollision(zombies[i], zombies[j])) {
                        resolveCollision(zombies[i], zombies[j]);
                    }
                }
            }

            checkHeroZombieCollisions();

            if (survivalMode) {
                Time elapsed = survivalClock.getElapsedTime();
                std::stringstream ss;
                ss << "Survival Time: " << std::fixed << std::setprecision(1) << elapsed.asSeconds() << " seconds";
                timerText.setString(ss.str());
            }

            for (auto& bullet : bullets) {
                bullet.update();
            }
            if (!gameStarted && !gameEnded) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                menu.update(mousePos);
            }
            checkBulletCollisions();
        }
    }


    void render() {
        window.clear();
        if (gameStarted) {
            for (int x = 0; x < window_width; x += background_texture.getSize().x) {
                for (int y = 0; y < window_height; y += background_texture.getSize().y) {
                    background_sprite.setPosition(x, y);
                    window.draw(background_sprite);
                }
            }
            window.draw(hero);
            for (auto& zombie : zombies) {
                window.draw(zombie);
            }
            if (survivalMode) {
                window.draw(timerText);
            }
            window.draw(healthText);
            window.draw(killCounterText); // Draw the kill counter text
            if (gameEnded) {
                window.draw(endGameText);
            }
            for (const auto& bullet : bullets) {
                window.draw(bullet);
            }
        } else {
            menu.draw(window);
        }
        window.display();
    }
    void updateKillCounterText() {
        std::stringstream ss;
        ss << "Kills: " << zombiesKilled;
        killCounterText.setString(ss.str());
    }



    bool checkCollision(const sf::Sprite& sprite1, const sf::Sprite& sprite2) {
        return sprite1.getGlobalBounds().intersects(sprite2.getGlobalBounds());
    }

    void resolveCollision(sf::Sprite& sprite1, sf::Sprite& sprite2) {
        sf::FloatRect bounds1 = sprite1.getGlobalBounds();
        sf::FloatRect bounds2 = sprite2.getGlobalBounds();

        float overlapLeft = bounds1.left + bounds1.width - bounds2.left;
        float overlapRight = bounds2.left + bounds2.width - bounds1.left;
        float overlapTop = bounds1.top + bounds1.height - bounds2.top;
        float overlapBottom = bounds2.top + bounds2.height - bounds1.top;

        bool fromLeft = abs(overlapLeft) < abs(overlapRight);
        bool fromTop = abs(overlapTop) < abs(overlapBottom);

        float overlapX = fromLeft ? overlapLeft : overlapRight;
        float overlapY = fromTop ? overlapTop : overlapBottom;

        if (abs(overlapX) < abs(overlapY)) {
            sprite1.move(-overlapX / 2, 0);
            sprite2.move(overlapX / 2, 0);
        } else {
            sprite1.move(0, -overlapY / 2);
            sprite2.move(0, overlapY / 2);
        }
    }

};
//do testu commita
int main() {
    Game game;
    game.run();
    return 0;
}
