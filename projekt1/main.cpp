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
    if (!character_texture.loadFromFile("character.png")) {
        cout << "Nie udało się wczytać tekstury postaci" << endl;
        return 1;
    }

    // Utwórz sprite postaci i ustaw jego teksturę
    AnimatedSprite character_sprite(10); // 10 klatek na sekundę
    character_sprite.setTexture(character_texture);
    character_sprite.setScale(2,2);

    // Dodaj klatki animacji
    character_sprite.add_animation_frame(IntRect(0, 0, 32, 32)); // Przykładowa klatka animacji

    // Ustaw pozycję początkową postaci na środku ekranu
    Vector2f character_position(window_width / 2, window_height / 2);
    character_sprite.setPosition(character_position);

    // Prędkość poruszania się postaci
    float move_speed = 0.1f;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        character_sprite.step(); // Aktualizacja animacji

        // Poruszanie postacią
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            character_position.x -= move_speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            character_position.x += move_speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            character_position.y -= move_speed;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            character_position.y += move_speed;
        }

        // Ustaw nową pozycję postaci
        character_sprite.setPosition(character_position);

        window.clear(sf::Color::Black);

        // Rysuj tło
        window.draw(background_sprite);

        // Rysuj postać
        window.draw(character_sprite);

        window.display();
    }

    return 0;
}
