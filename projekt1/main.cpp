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

    void move(float offsetX, float offsetY) {
        Sprite::move(offsetX, offsetY);
        if (offsetX > 0) direction = Right;
        else if (offsetX < 0) direction = Left;
        else if (offsetY > 0) direction = Down;
        else if (offsetY < 0) direction = Up;
    }

private:
    const vector<IntRect>& getFrames() const {
        switch (direction) {
        case Right: return framesRight;
        case Left: return framesLeft;
        case Up: return framesUp;
        case Down: return framesDown;
        }
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

public:
    Menu() {
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
    }

    void playSound() {
        sound.play();
    }

    void draw(sf::RenderWindow& window) {
        window.draw(background_sprite);
        window.draw(text);
    }
    void stopSound() {
        sound.stop();
    }
};

int main() {
    int window_width = 800;
    int window_height = 600;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "Nasza GRA");

    Texture background_image;
    if (!background_image.loadFromFile("background.png")) {
        cout << "Nie udało się wczytać tekstury tła" << endl;
        return 1;
    }

    // Ustawienie skalowania tekstury tła na rozmiar okna
    background_image.setRepeated(true);
    Sprite background_sprite(background_image);
    background_sprite.setTextureRect(IntRect(0, 0, window_width, window_height));

    // Załaduj teksturę postaci
    Texture character_texture;
    if (!character_texture.loadFromFile("walk.png")) {
        cout << "Nie udało się wczytać tekstury postaci" << endl;
        return 1;
    }

    // Ustawienia postaci
    AnimatedSprite hero(7); // 7 klatek na sekundę
    hero.setTexture(character_texture);
    // Dodaj klatki animacji dla każdego kierunku
    // Prawo
    hero.add_animation_frame_right(IntRect(9, 70, 25, 25));
    hero.add_animation_frame_right(IntRect(41, 70, 25, 25));
    hero.add_animation_frame_right(IntRect(73, 70, 25, 25));
    hero.add_animation_frame_right(IntRect(105, 70, 25, 25));
    hero.add_standing_frame_right(IntRect(136, 70, 25, 25)); // Dodajemy klatkę stojącą dla kierunku prawo
    // Lewo
    hero.add_animation_frame_left(IntRect(10, 102, 25, 25));
    hero.add_animation_frame_left(IntRect(41, 102, 25, 25));
    hero.add_animation_frame_left(IntRect(73, 102, 25, 25));
    hero.add_animation_frame_left(IntRect(105, 102, 25, 25));
    hero.add_standing_frame_left(IntRect(136, 102, 25, 25)); // Dodajemy klatkę stojącą dla kierunku lewo
    // Góra
    hero.add_animation_frame_up(IntRect(10, 38, 25, 25));
    hero.add_animation_frame_up(IntRect(41, 38, 25, 25));
    hero.add_animation_frame_up(IntRect(73, 38, 25, 25));
    hero.add_animation_frame_up(IntRect(105, 38, 25, 25));
    hero.add_standing_frame_up(IntRect(136, 38, 25, 25)); // Dodajemy klatkę stojącą dla kierunku góra
    // Dół
    hero.add_animation_frame_down(IntRect(9, 5, 25, 25));//9,5,23,31 bylo
    hero.add_animation_frame_down(IntRect(41, 5, 25, 25));
    hero.add_animation_frame_down(IntRect(73, 5, 25, 25));
    hero.add_animation_frame_down(IntRect(105, 5, 25, 25));
    hero.add_standing_frame_down(IntRect(134, 5, 25, 25)); // Dodajemy klatkę stojącą dla kierunku dół
    hero.setPosition(200, 200);
    hero.setScale(2, 2);
    hero.setTextureRect(IntRect(134, 5, 149, 31)); // Ustawienie domyślnego wyglądu postaci

    // Prędkość poruszania się postaci
    float move_speed = 0.1f;

    Menu menu;
    bool gameStarted = false;
    menu.playSound();

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                gameStarted = true;
                menu.stopSound();  // Zatrzymaj muzykę, gdy gra się rozpocznie
            }
        }

        hero.step(); // Aktualizacja animacji

        // Poruszanie postacią
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            hero.move(-move_speed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            hero.move(move_speed, 0);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            hero.move(0, -move_speed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            hero.move(0, move_speed);
        }

        window.clear(sf::Color::Black);

        if (!gameStarted) {
            menu.draw(window);

        } else {
            // Rysuj tło
            window.draw(background_sprite);

            // Rysuj postać
            window.draw(hero);
        }

        window.display();
    }

    return 0;
}
