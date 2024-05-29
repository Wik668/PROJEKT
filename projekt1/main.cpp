#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>
#include <sstream>
#include <iomanip>
#include <SFML/System/Vector2.hpp> // Include the necessary header
#include <cmath>
using namespace std;
using namespace sf;


sf::Vector2f normalize(sf::Vector2f vec) {
    float length = std::sqrt(vec.x * vec.x + vec.y * vec.y);
    if (length != 0) {
        return sf::Vector2f(vec.x / length, vec.y / length);
    } else {
        return sf::Vector2f(0, 0);
    }
}

class Zombie : public Sprite {
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
    void moveWithCollision(const sf::FloatRect& bounds, float offsetX, float offsetY) {
        sf::Vector2f oldPosition = getPosition();
        sf::Sprite::move(offsetX, offsetY);
        sf::FloatRect spriteBounds = getGlobalBounds();

        if (spriteBounds.left < bounds.left ||
            spriteBounds.top < bounds.top ||
            spriteBounds.left + spriteBounds.width > bounds.left + bounds.width ||
            spriteBounds.top + spriteBounds.height > bounds.top + bounds.height) {
            setPosition(oldPosition);
        }
    }

    Zombie(int fps) : currentFrame(0), animationFps(fps), direction(Right) {}

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

    void step() {
        frameTime += clock.restart();
        Time timePerFrame = seconds(1.0f / animationFps);

        while (frameTime >= timePerFrame) {
            frameTime -= timePerFrame;
            currentFrame = (currentFrame + 1) % (getFrames().size() - 1); // Skip standing frame
            setTextureRect(getFrames()[currentFrame]);
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

    void step() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
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

class Menu {
private:
    sf::Font font;//czcionka
    sf::Text text;
    sf::Texture background_texture;
    sf::Sprite background_sprite;
    sf::SoundBuffer buffer;
    sf::Sound sound;
    sf::Text survivalText;
    sf::Text stageText;
    bool selectedSurvival;

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
        sound.setLoop(true); // Set the sound to loop

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
    }

    void moveSelectionUp() {
        selectedSurvival = true;
        updateSelection();
    }

    void moveSelectionDown()
    {
        selectedSurvival = false;
        updateSelection();
    }

    bool isSurvivalSelected() const {
        return selectedSurvival;
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
    bool survivalMode; // New variable to store the selected game mode
    float move_speed;
    sf::Clock survivalClock; // Clock for the Survival Mode timer
    sf::Text timerText;
    sf::Font font;

    void loadResources() {
        if (!background_texture.loadFromFile("background.png")) {
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
            Zombie zombie(5);
            zombie.setTexture(zombie_texture);

            zombie.add_animation_frame_right(IntRect(11, 69, 12, 26));
            zombie.add_animation_frame_right(IntRect(42, 69, 12, 26));
            zombie.add_standing_frame_right(IntRect(74, 69, 12, 26));
            zombie.add_animation_frame_right(IntRect(107, 69, 12, 26));
            zombie.add_animation_frame_right(IntRect(140, 69, 12, 26));
            zombie.add_animation_frame_right(IntRect(173, 69, 12, 26));
            zombie.add_animation_frame_right(IntRect(204, 69, 12, 26));
            zombie.add_standing_frame_right(IntRect(236, 69, 12, 26));
            zombie.add_animation_frame_right(IntRect(269, 69, 12, 26));
            zombie.add_animation_frame_right(IntRect(300, 69, 12, 26));

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





            // Repeat for other directions...

            zombie.setScale(2, 2);
            zombie.setPosition(i * 100, 200);  // Example positions
            zombies.push_back(zombie);
        }
    }

public:

    Game()
        : window(sf::VideoMode(window_width, window_height), "Game Window"),
        hero(5), gameStarted(false), move_speed(0.1f) { // Initialize move_speed here
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

    void handleEvents() {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::Escape) {
                    window.close();
                } else if (!gameStarted) {
                    if (event.key.code == Keyboard::Enter) {
                        gameStarted = true;
                        survivalMode = menu.isSurvivalSelected(); // Set the selected game mode
                        menu.stopSound();
                        survivalClock.restart();
                        gameMusic.setLoop(true); // Ustaw muzykę na zapętlanie //DODANY
                        gameMusic.play();//dodany
                    } else if (event.key.code == Keyboard::Up) {
                        menu.moveSelectionUp();
                    } else if (event.key.code == Keyboard::Down) {
                        menu.moveSelectionDown();
                    }
                }
            }
        }
    }

    void update() {
        if (gameStarted) {
            FloatRect bounds(0, 0, window_width, window_height);
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                hero.moveWithCollision(bounds, -move_speed, 0);
            } else if (Keyboard::isKeyPressed(Keyboard::Right)) {
                hero.moveWithCollision(bounds, move_speed, 0);
            } else if (Keyboard::isKeyPressed(Keyboard::Up)) {
                hero.moveWithCollision(bounds, 0, -move_speed);
            } else if (Keyboard::isKeyPressed(Keyboard::Down)) {
                hero.moveWithCollision(bounds, 0, move_speed);
            }
            hero.step();

            // Calculate direction vector towards the player
            Vector2f playerPosition = hero.getPosition();
            for (auto& zombie : zombies) {
                Vector2f zombiePosition = zombie.getPosition();
                Vector2f direction = playerPosition - zombiePosition;
                direction = normalize(direction); // Normalize the vector to get a unit vector

                // Move the zombie towards the player
                float moveX = direction.x * move_speed*0.2;
                float moveY = direction.y * move_speed*0.2;
                zombie.moveWithCollision(bounds, moveX, moveY);

                // Update zombie animation
                zombie.step();
            }

            if (survivalMode) {
                Time elapsed = survivalClock.getElapsedTime();
                std::stringstream ss;
                ss << "Survival Time: " << std::fixed << std::setprecision(1) << elapsed.asSeconds() << " seconds";
                timerText.setString(ss.str());
            }
        }
    }

    void render() {
        window.clear();
        if (gameStarted) {
            // Draw the repeating background
            for (int x = 0; x < window_width; x += background_texture.getSize().x) {
                for (int y = 0; y < window_height; y += background_texture.getSize().y) {
                    background_sprite.setPosition(x, y);
                    window.draw(background_sprite);
                }
            }
            window.draw(hero);
            for (auto& zombie : zombies) {
                window.draw(zombie); // Draw each zombie
            }
            if (survivalMode) {
                window.draw(timerText); // Draw the timer text if in Survival Mode
            }
        } else {
            menu.draw(window);
        }
        window.display();
    }

};

int main() {
    Game game;
    game.run();
    return 0;
}
