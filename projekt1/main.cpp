#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <SFML/Audio.hpp>
#include <SFML/Audio/SoundBuffer.hpp>

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

    void step() {
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Up) ||
            sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            frameTime += clock.restart();
            Time timePerFrame = seconds(1.0f / animationFps);

            while (frameTime >= timePerFrame) {
                frameTime -= timePerFrame;
                currentFrame = (currentFrame + 1) % (getFrames().size() - 1); // Odejmujemy 1, aby pominąć klatkę stojącą
                setTextureRect(getFrames()[currentFrame]);
            }
        } else {
            setTextureRect(getFrames().back()); // Ustawiamy ostatnią klatkę jako klatkę stojącą
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
        survivalText.setFillColor(sf::Color::Red);
        stageText.setFillColor(sf::Color::White);
    }

    void moveSelectionDown() {
        selectedSurvival = false;
        survivalText.setFillColor(sf::Color::White);
        stageText.setFillColor(sf::Color::Red);
    }

    bool isSurvivalSelected() const {
        return selectedSurvival;
    }
};

class Game {
private:
    int window_width = 800;
    int window_height = 600;
    sf::RenderWindow window;
    AnimatedSprite hero;
    sf::Texture character_texture;
    sf::Texture background_image;
    sf::Sprite background_sprite;
    Menu menu;
    bool gameStarted;
    bool survivalMode; // Nowa zmienna do przechowywania wybranego trybu gry
    float move_speed;

    void loadResources() {
        if (!background_image.loadFromFile("background.png")) {
            cout << "Nie udało się wczytać tekstury tła" << endl;
            exit(1);
        }

        background_image.setRepeated(true);
        background_sprite.setTexture(background_image);
        background_sprite.setTextureRect(IntRect(0, 0, window_width, window_height));

        if (!character_texture.loadFromFile("walk.png")) {
            cout << "Nie udało się wczytać tekstury postaci" << endl;
            exit(1);
        }

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

    void handleInput() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                gameStarted = true;
                survivalMode = menu.isSurvivalSelected();
                menu.stopSound();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Up) {
                menu.moveSelectionUp();
            }

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Down) {
                menu.moveSelectionDown();
            }
        }

        if (gameStarted) {
            FloatRect bounds(0, 0, window_width, window_height);
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                hero.moveWithCollision(bounds, -move_speed, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                hero.moveWithCollision(bounds, move_speed, 0);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                hero.moveWithCollision(bounds, 0, -move_speed);
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                hero.moveWithCollision(bounds, 0, move_speed);
            }
        }
    }

    void update() {
        if (gameStarted) {
            hero.step();
        }
    }

    void render() {
        window.clear(sf::Color::Black);
        if (!gameStarted) {
            menu.draw(window);
        } else {
            window.draw(background_sprite);
            window.draw(hero);
        }
        window.display();
    }

public:
    Game() : window(sf::VideoMode(window_width, window_height), "Nasza GRA"), hero(7), gameStarted(false), move_speed(0.1f) {
        loadResources();
        menu.playSound();
    }

    void run() {
        while (window.isOpen()) {
            handleInput();
            update();
            render();
        }
    }
};

int main() {
    Game game;
    game.run();
    return 0;
}
