#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace sf;

class AnimatedSprite : public Sprite {
private:
    vector<IntRect> frames;
    int currentFrame;
    int animationFps;
    Time frameTime;
    Clock clock;

public:
    AnimatedSprite(int fps) : currentFrame(0), animationFps(fps) {}

    void add_animation_frame(const IntRect& frame) {
        frames.push_back(frame);
    }

    void step() {
        frameTime += clock.restart();
        Time timePerFrame = seconds(1.0f / animationFps);

        while (frameTime >= timePerFrame) {
            frameTime -= timePerFrame;
            currentFrame = (currentFrame + 1) % frames.size();
            setTextureRect(frames[currentFrame]);
        }
    }

    void reset() {
        currentFrame = 0;
        setTextureRect(frames[currentFrame]);
    }
};

int main() {
    int window_width = 800;
    int window_height = 600;
    sf::RenderWindow window(sf::VideoMode(window_width, window_height), "StreetFighter");

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
    if (!character_texture.loadFromFile("character.png")) {
        cout << "Nie udało się wczytać tekstury postaci" << endl;
        return 1;
    }

    // Ustawienia postaci
    AnimatedSprite hero(7); // 7 klatek na sekundę
    hero.setTexture(character_texture);
    hero.add_animation_frame(IntRect(215, 0, 25, 37)); // Ruch postaci
    hero.add_animation_frame(IntRect(265, 0, 25, 37)); // Ruch postaci
    hero.add_animation_frame(IntRect(315, 0, 25, 37)); // Ruch postaci
    hero.add_animation_frame(IntRect(365, 0, 25, 37)); // Ruch postaci
    hero.add_animation_frame(IntRect(10, 5, 20, 35));  // Spoczynek postaci
    hero.setPosition(100, 0);
    hero.setScale(2, 2);
    hero.setTextureRect(IntRect(200, 0, 37, 37)); // Ustawienie domyślnego wyglądu postaci

    // Prędkość poruszania się postaci
    float move_speed = 0.1f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        bool isMoving = false;

        // Poruszanie postacią
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            hero.move(-move_speed, 0);
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            hero.move(move_speed, 0);
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            hero.move(0, -move_speed);
            isMoving = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            hero.move(0, move_speed);
            isMoving = true;
        }

        if (isMoving) {
            hero.step(); // animacja tylko podczas ruchu
        } else {
            hero.reset(); // domyslny wygląd postaci
        }

        window.clear(sf::Color::Black);

        // wyswietla tlo tło
        window.draw(background_sprite);

        // wyswietla postać
        window.draw(hero);

        window.display();
    }

    return 0;
}
